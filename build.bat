@echo off

set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4505 -wd4100 -wd4189 -DGAME_INTERNAL=1 -DGAME_SLOW=1 -FC -Z7 /std:c++latest /EHsc
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib Gdi32.lib winmm.lib opengl32.lib /DEBUG

IF NOT EXIST build mkdir build
pushd build

cl %CommonCompilerFlags%  ../src/*.cpp ../src/glad.c /link %CommonLinkerFlags%
popd
