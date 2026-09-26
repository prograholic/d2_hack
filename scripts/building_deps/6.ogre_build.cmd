set "ORIGINAL_DIR=%CD%"

set LIB_NAME=Ogre
set SOURCE_DIR=src

set REL_DIR=build\%LIB_NAME%\rel
set DBG_DIR=build\%LIB_NAME%\debug

set VC_VER=14.5.2-vc17-x64

set GENERATOR="Visual Studio 17 2022"

set ARCH=x64

set DEPS_INSTALL_ROOT=e:\pixel\src\ogre\deps\install
set OGRE_INSTALL_ROOT=e:\pixel\src\ogre\install

set INSTALL_PREFIX_REL=%OGRE_INSTALL_ROOT%\%LIB_NAME%\%VC_VER%\rel
set INSTALL_PREFIX_DBG=%OGRE_INSTALL_ROOT%\%LIB_NAME%\%VC_VER%\debug


REM *****************************RELEASE*****************************************


rmdir /s /q %REL_DIR%
rmdir /s /q %INSTALL_PREFIX_REL%
mkdir %REL_DIR%
cd %REL_DIR%

set FREETYPE_DIR=%DEPS_INSTALL_ROOT%\freetype\%VC_VER%\rel

cmake %ORIGINAL_DIR%\%SOURCE_DIR% -G%GENERATOR% -A %ARCH% ^
    -DCMAKE_CONFIGURATION_TYPES=RelWithDebInfo ^
    -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ^
        -DOGRE_BUILD_DEPENDENCIES=OFF ^
        -DZLIB_ROOT=%DEPS_INSTALL_ROOT%\zlib\%VC_VER%\rel ^
        -Dpugixml_DIR=%DEPS_INSTALL_ROOT%\pugixml\%VC_VER%\rel\lib\cmake\pugixml ^
        -Dassimp_DIR=%DEPS_INSTALL_ROOT%\assimp\%VC_VER%\rel\lib\cmake\assimp-6.0 ^
        -DBullet_ROOT=%DEPS_INSTALL_ROOT%\bullet\%VC_VER%\rel ^
        -DSDL2_DIR=%DEPS_INSTALL_ROOT%\SDL2\%VC_VER%\rel\cmake ^
    -DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX_REL% > configure.log 2>&1
    
    
cmake --build . --config RelWithDebInfo --target INSTALL > install_release.log 2>&1

cd %ORIGINAL_DIR%


REM ***************************DEBUG**********************************************


rmdir /s /q %DBG_DIR%
rmdir /s /q %INSTALL_PREFIX_DBG%
mkdir %DBG_DIR%
cd %DBG_DIR%

set FREETYPE_DIR=%DEPS_INSTALL_ROOT%\freetype\%VC_VER%\debug

cmake %ORIGINAL_DIR%\%SOURCE_DIR% -G"Visual Studio 17 2022" -A x64 ^
    -DCMAKE_CONFIGURATION_TYPES=Debug ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ^
        -DOGRE_BUILD_DEPENDENCIES=OFF ^
        -DZLIB_ROOT=%DEPS_INSTALL_ROOT%\zlib\%VC_VER%\debug ^
        -Dpugixml_DIR=%DEPS_INSTALL_ROOT%\pugixml\%VC_VER%\debug\lib\cmake\pugixml ^
        -Dassimp_DIR=%DEPS_INSTALL_ROOT%\assimp\%VC_VER%\debug\lib\cmake\assimp-6.0 ^
        -DBullet_ROOT=%DEPS_INSTALL_ROOT%\bullet\%VC_VER%\debug ^
        -DSDL2_DIR=%DEPS_INSTALL_ROOT%\SDL2\%VC_VER%\debug\cmake ^
    -DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX_DBG% > configure.log 2>&1


cmake --build . --config Debug --target INSTALL > install_debug.log 2>&1

cd %ORIGINAL_DIR%
