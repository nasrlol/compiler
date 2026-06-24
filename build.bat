@echo off

set opts=-FC -GR- -EHa- -nologo -Zi -Od -RTC1 /std:c++14
set libs=user32.lib gdi32.lib kernel32.lib
set includes=  -Iw:\compiler\source -Iw:\compiler\source\base
pushd build
cl %opts% w:\compiler\source\compiler.cpp -Fecompiler  %includes%
popd
