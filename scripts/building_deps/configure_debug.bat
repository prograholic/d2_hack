md .build_dbg
cd .build_dbg

set VC_VER=14.5.2-vc17-x64
set DEPS_INSTALL_ROOT=e:\pixel\src\ogre\deps\install
set OGRE_INSTALL_ROOT=e:\pixel\src\ogre\install

set FREETYPE_DIR=%DEPS_INSTALL_ROOT%\freetype\%VC_VER%\debug

cmake ../d2_hack ^
	-Ax64 ^
	-G"Visual Studio 17 2022" ^
	-DD2_ROOT_DIR=E:\virtual_cd\d2 ^
	-DBOOST_ROOT=E:\pixel\src\boost\install\1.87.0 ^
	-DOGRE_DIR=%OGRE_INSTALL_ROOT%\Ogre\%VC_VER%\debug ^
    -DZLIB_ROOT=%DEPS_INSTALL_ROOT%\zlib\%VC_VER%\debug ^
    -DSDL2_DIR=%DEPS_INSTALL_ROOT%\SDL2\%VC_VER%\debug\cmake ^
    -DBullet_ROOT=%DEPS_INSTALL_ROOT%\bullet\%VC_VER%\debug ^
	-DCMAKE_CONFIGURATION_TYPES=Debug > configure.log 2>&1

cd ../