set "ORIGINAL_DIR=%CD%"

set LIB_NAME=bullet
set SOURCE_DIR=%LIB_NAME%3-3.25

set REL_DIR=build\%LIB_NAME%\rel
set DBG_DIR=build\%LIB_NAME%\debug

set VC_VER=14.5.2-vc17-x64

set GENERATOR="Visual Studio 17 2022"

set ARCH=x64

set DEPS_INSTALL_ROOT=e:\pixel\src\ogre\deps\install

set INSTALL_PREFIX_REL=%DEPS_INSTALL_ROOT%\%LIB_NAME%\%VC_VER%\rel
set INSTALL_PREFIX_DBG=%DEPS_INSTALL_ROOT%\%LIB_NAME%\%VC_VER%\debug


REM *****************************RELEASE*****************************************


rmdir /s /q %REL_DIR%
rmdir /s /q %INSTALL_PREFIX_REL%
mkdir %REL_DIR%
cd %REL_DIR%



cmake %ORIGINAL_DIR%\%SOURCE_DIR% -G%GENERATOR% -A %ARCH% ^
    -DCMAKE_CONFIGURATION_TYPES=RelWithDebInfo ^
    -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ^
        -DBUILD_SHARED_LIBS=OFF ^
        -DINSTALL_LIBS=ON ^
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON ^
        -DBUILD_PYBULLET=OFF ^
        -DUSE_DOUBLE_PRECISION=OFF ^
        -DBUILD_CPU_DEMOS=OFF ^
        -DBUILD_BULLET2_DEMOS=OFF ^
        -DBUILD_EXTRAS=OFF ^
        -DBUILD_EGL=OFF ^
        -DBUILD_ENET=OFF ^
        -DBUILD_UNIT_TESTS=OFF ^
        -DCMAKE_RELWITHDEBINFO_POSTFIX= ^
        -DCMAKE_DEBUG_POSTFIX= ^
        -DBUILD_CLSOCKET=OFF ^
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
    -DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX_REL% > configure.log 2>&1
    
    
cmake --build . --config RelWithDebInfo --target INSTALL > install_release.log 2>&1

cd %ORIGINAL_DIR%


REM ***************************DEBUG**********************************************


rmdir /s /q %DBG_DIR%
rmdir /s /q %INSTALL_PREFIX_DBG%
mkdir %DBG_DIR%
cd %DBG_DIR%

cmake %ORIGINAL_DIR%\%SOURCE_DIR% -G"Visual Studio 17 2022" -A x64 ^
    -DCMAKE_CONFIGURATION_TYPES=Debug ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ^
        -DBUILD_SHARED_LIBS=OFF ^
        -DINSTALL_LIBS=ON ^
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON ^
        -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON ^
        -DBUILD_PYBULLET=OFF ^
        -DUSE_DOUBLE_PRECISION=OFF ^
        -DBUILD_CPU_DEMOS=OFF ^
        -DBUILD_BULLET2_DEMOS=OFF ^
        -DBUILD_EXTRAS=OFF ^
        -DBUILD_EGL=OFF ^
        -DBUILD_ENET=OFF ^
        -DBUILD_UNIT_TESTS=OFF ^
        -DCMAKE_RELWITHDEBINFO_POSTFIX= ^
        -DCMAKE_DEBUG_POSTFIX= ^
        -DBUILD_CLSOCKET=OFF ^
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
    -DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX_DBG% > configure.log 2>&1


cmake --build . --config Debug --target INSTALL > install_debug.log 2>&1

cd %ORIGINAL_DIR%
