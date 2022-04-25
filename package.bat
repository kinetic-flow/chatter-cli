for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /format:list') do set datetime=%%I
set datetime=%datetime:~0,8%-%datetime:~8,6%

cd /d %~dp0
del /q /s dist\chatter
mkdir dist\chatter\pdb
mkdir dist\chatter\src
dir chatter\Release\chatter.exe
dir chatter\Release\chatter.pdb
copy /y chatter.bat dist\chatter
copy /y chatter\Release\chatter.exe dist\chatter
copy /y chatter\Release\chatter.pdb dist\chatter\pdb
copy /y LICENSE dist\chatter\LICENSE.txt
copy /y README.md dist\chatter
git archive --format zip --output .\dist\chatter\src\main.zip main
pushd dist
7z a -tzip -r chatter-%datetime%.zip chatter
popd