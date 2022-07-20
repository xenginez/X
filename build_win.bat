#.\3rdparty\build_win.bat

mkdir build
cd .\build
cmake -DX_BUILD_XE=ON -DX_BUILD_XS=ON -DX_BUILD_XV=ON -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false

pause