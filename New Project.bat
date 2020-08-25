@echo off
set /p name="Enter Project name: "
mkdir "projects/%name%/src"
mkdir "projects/%name%/res"
@echo int main() { return 0; } > "projects/%name%/src/main.cpp"
echo "Added project: %name%"
call premake_build.bat