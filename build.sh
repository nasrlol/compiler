#!/bin/bash

code="$PWD"
echo $code
opts=-g -I$code/source/base -fpermissive
cd $code/build > /dev/null
g++ $opts $code/source/compiler.cpp -o compiler
cd $code > /dev/null
