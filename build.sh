#!/bin/bash

code="$PWD"
opts=-g
cd build > /dev/null
g++ $opts $code/compiler -o compiler
cd $code > /dev/null
