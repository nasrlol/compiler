#!/bin/bash
# Drive the RAD Debugger from 4coder (see the fkey_command table in project.4coder).
#
#   debug.sh run  [compiler|test]   run under the debugger; it closes again if the
#                                   program exits cleanly, stays open on a crash/breakpoint
#   debug.sh step [compiler|test]   start stopped at main (step into)
#   debug.sh open [compiler|test]   just open the debugger on this project
#   debug.sh over|out|halt|restart|kill
#                                   control an already open debugger over IPC
#
# If this project's debugger is already open, run/step are forwarded to it over IPC
# instead of launching a second one. It listens on its own IPC port, so other open
# raddbg instances are never touched. Targets and breakpoints live in project.raddbg.
set -eu

RADDBG=${RADDBG:-/opt/raddbg/raddbg}
root=$(cd "$(dirname "$0")" && pwd)
mode=${1:-run}
target=${2:-compiler}

project="$root/project.raddbg"
# per-project IPC port (raddbg's default is 7423)
port=$((7424 + $(printf '%s' "$root" | cksum | cut -d' ' -f1) % 1000))

ipc() { "$RADDBG" --ipc --ipc_port:"$port" "$@" >/dev/null; }

# this project's debugger instance
running() { pgrep -u "$USER" -f -- "--project:$project" >/dev/null; }

# enable only the requested target in project.raddbg (targets are matched by label)
select_target() {
	awk -v want="$1" '
		/^target:/        { in_target = 1; label = ""; n = 0 }
		in_target         { block[n++] = $0
		                    if ($1 == "label:") label = $2
		                    if ($0 == "}") {
		                      for (i = 0; i < n; i++) {
		                        if (block[i] ~ /^  enabled:/) block[i] = "  enabled: " (label == want ? 1 : 0)
		                        print block[i]
		                      }
		                      in_target = 0
		                    }
		                    next }
		                  { print }
	' "$project" > "$project.tmp"
	mv "$project.tmp" "$project"
}

launch() {
	select_target "$target"
	local wrap=()
	# the tests expect address randomization to be off (see test_run)
	[ "$target" = test ] && wrap=(setarch "$(uname -m)" -R)
	cd "$root"
	setsid -f "${wrap[@]}" "$RADDBG" --ipc_port:"$port" --project:"$project" "$@" >/dev/null 2>&1
}

case $mode in
run)     if running; then ipc run;       else launch --auto_run -q; fi ;;
step)    if running; then ipc step_into; else launch --auto_step;  fi ;;
open)    if running; then :;             else launch;              fi ;;
over)    ipc step_over ;;
out)     ipc step_out ;;
halt)    ipc halt ;;
restart) ipc restart ;;
kill)    ipc kill_all ;;
*) echo "debug.sh: unknown mode '$mode'" >&2; exit 1 ;;
esac
