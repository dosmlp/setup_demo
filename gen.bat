rmdir /S /Q dist
mkdir dist
cd dist
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars32.bat"
..\Qt6.5.0_x86_static_vcrt\bin\qmake.exe ..\setup.pro "CONFIG+=release"
nmake

set filename=%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%%time:~6,2%
set "filename=%filename: =0%"
..\7z\7za.exe a -r addons.7z -mx9 ..\addons\*
copy /B .\release\setup.exe+addons.7z setup_%filename%.exe

