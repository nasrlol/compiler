@echo off

set opts=-FC -GR- -EHa- -nologo -Zi -Od -RTC1
set libs=user32.lib gdi32.lib kernel32.lib
pushd build
cl %opts% w:\compiler\source\compiler\compiler.c -Fecompiler -Iw:\compiler\source\
popd
