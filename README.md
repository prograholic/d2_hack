# How to build

## Dependencies

### Boost
No specific version, any recent version is acceptable
 
### Ogre
Project uses vanilla Ogre, version v14.
Build Ogre with original instructions

#### Recommendations for building Ogre for Win platform
It is recommended use VS Generator and build RelWithDebInfo and Debug builds, see sample scripts below

Configure script (Ogre):
```
mkdir build_64_rel_with_deb
cd build_64_rel_with_deb

cmake ^
    ../from_github ^
    -G"Visual Studio 16" ^
    -A X64 ^
    -DCMAKE_INSTALL_PREFIX=<SOME_DIR>\installed\vs16_64_rel_with_deb ^
    -DCMAKE_CONFIGURATION_TYPES=RelWithDebInfo ^
    -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    > configure.log 2>&1

cd ..

```

Build script (Ogre):
```
cd build_64_rel_with_deb

cmake --build . --target INSTALL --config RelWithDebInfo > build_release.log 2>&1

cd ..


```
