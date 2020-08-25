@echo off
FOR /d /r . %%d IN (obj) DO @IF EXIST "%%d" rd /s /q "%%d"
FOR /d /r . %%d IN (bin) DO @IF EXIST "%%d" rd /s /q "%%d"
FOR /d /r . %%d IN (.vs) DO @IF EXIST "%%d" rd /s /q "%%d"
del /S /Q *.vcxproj
del /S /Q *.vcxproj.user
del /S /Q *.vcxproj.filters
del /S /Q *.sln
rd  /s /q shared_assets
PAUSE