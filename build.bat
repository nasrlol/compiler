@echo off
set opts=-FC -FS -GR- -EHa- -nologo -Zi -Od -RTC1 /std:c++17
set libs=user32.lib gdi32.lib kernel32.lib
set includes=  -Iw:\compiler\source -Iw:\compiler\source\base
pushd build
del *.obj *.exe 2>nul
cl %opts% w:\compiler\source\compiler.cpp -Fecompiler  %includes%
popd
