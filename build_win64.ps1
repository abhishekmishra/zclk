# create the build directory
New-Item -path . -name build -itemtype directory

# change to the build directory
Set-Location .\build

# generate the build files
cmake .. -DCMAKE_TOOLCHAIN_FILE=d:\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DENABLE_TESTS=On

# run msbuild to build the library
# see https://github.com/Microsoft/vswhere/wiki/Find-MSBuild
# and https://github.com/microsoft/vswhere/wiki/Installing
# for finding msbuild using vswhere (and installing vswhere if does not exist)
$msbuildExe = vswhere -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe"

&$msbuildExe cld.sln

# go back to parent directory
Set-Location ..
