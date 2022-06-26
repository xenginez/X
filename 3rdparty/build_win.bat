set MSBUILD=()

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set MSBUILD="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto :FOUND_MSBUILD
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    set MSBUILD="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
    goto :FOUND_MSBUILD
)
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
	set MSBUILD="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    goto :FOUND_MSBUILD
)

if %MSBUILD%==() (
    echo "I couldn't find MSBuild on your PC. Make sure it's installed somewhere, and if it's not in the above if statements (in build.bat), add it."
    goto :EXIT
) 

:FOUND_MSBUILD
call %%MSBUILD%%
set RD3_PATH=%cd%





echo "update git submodule"
git submodule update --init --recursive



:BUILD_GOOGLETEST
cd %RD3_PATH%
mkdir .\googletest\build
echo "build gtest debug"
cd %RD3_PATH%
cd .\googletest\build
cmake -DBUILD_GMOCK=OFF -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy gtest debug file to depends"
xcopy %cd%\install\bin\gtest.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\gtest.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
echo "build gtest release"
cd %RD3_PATH%
cd .\googletest\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy gtest release file to depends"
xcopy %cd%\install\bin\gtest.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\gtest.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y



:BUILD_CPUINFO
cd %RD3_PATH%
mkdir .\cpuinfo\build
echo "build cpuinfo debug"
cd %RD3_PATH%
cd .\cpuinfo\build
cmake -DBENCHMARK_ENABLE_EXCEPTIONS=OFF -DBUILD_GMOCK=OFF -DCLOG_LOG_TO_STDIO=OFF -DCPUINFO_BUILD_BENCHMARKS=OFF -DCPUINFO_BUILD_MOCK_TESTS=OFF -DCPUINFO_BUILD_PKG_CONFIG=OFF -DCPUINFO_BUILD_TOOLS=OFF -DCPUINFO_BUILD_UNIT_TESTS=OFF -DINSTALL_GTEST=OFF -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy cpuinfo release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build cpuinfo release"
cd %RD3_PATH%
cd .\cpuinfo\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy cpuinfo release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\cpuinfo\ /s /y



:BUILD_OPENAL
cd %RD3_PATH%
mkdir .\openal-soft\build
echo "build openal-soft debug"
cd %RD3_PATH%
cd .\openal-soft\build
cmake -DALSOFT_EXAMPLES=OFF -DALSOFT_EMBED_HRTF_DATA=OFF -DALSOFT_INSTALL_CONFIG=OFF -DALSOFT_INSTALL_EXAMPLES=OFF -DALSOFT_INSTALL_AMBDEC_PRESETS=OFF -DALSOFT_INSTALL_UTILS=OFF -DALSOFT_INSTALL_NO_CONFIG_UTIL=ON -DALSOFT_INSTALL_HRTF_DATA=OFF -DALSOFT_UPDATE_BUILD_VERSION=OFF -DALSOFT_UTILS=OFF -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy openal-soft release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build openal-soft release"
cd %RD3_PATH%
cd .\openal-soft\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy openal-soft release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\openal-soft\ /s /y



:BUILD_SPIRV_CROSS
cd %RD3_PATH%
mkdir .\SPIRV-Cross\build
echo "build SPIRV-Cross debug"
cd %RD3_PATH%
cd .\SPIRV-Cross\build
cmake -DSPIRV_CROSS_ENABLE_TESTS=OFF -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy SPIRV-Cross release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build SPIRV-Cross release"
cd %RD3_PATH%
cd .\SPIRV-Cross\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy SPIRV-Cross release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y



:BUILD_ASSIMP
cd %RD3_PATH%
mkdir .\assimp\build
echo "build assimp debug"
cd %RD3_PATH%
cd .\assimp\build
cmake -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DASSIMP_INSTALL_PDB=OFF -DCMAKE_DEBUG_PREFIX="" -DCMAKE_DEBUG_POSTFIX="" -DLIBRARY_SUFFIX="" -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy assimp release file to depends"
xcopy %cd%\install\bin\assimp.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\assimp.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build assimp release"
cd %RD3_PATH%
cd .\assimp\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy assimp release file to depends"
xcopy %cd%\install\bin\assimp.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\assimp.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y





:BUILD_STB
echo "copy stb head file to depends"
cd %RD3_PATH%
xcopy %cd%\stb\*.h %RD3_PATH%\..\depends\include\stb\ /s /y

:BUILD_PUGIXML
echo "copy pugixml head file to depends"
cd %RD3_PATH%
xcopy %cd%\pugixml\src\ %RD3_PATH%\..\depends\include\pugixml\ /s /y

:BUILD_RAPIDJSON
echo "copy rapidjson head file to depends"
cd %RD3_PATH%
xcopy %cd%\rapidjson\include\ %RD3_PATH%\..\depends\include\ /s /y

:BUILD_CSV2
echo "copy csv2 head file to depends"
cd %RD3_PATH%
xcopy %cd%\csv2\single_include\csv2\csv2.hpp %RD3_PATH%\..\depends\include\csv2\ /s /y

:BUILD_D3DX12
echo "copy d3dx12 head file to depends"
cd %RD3_PATH%
xcopy %cd%\D3DX12\d3dx12.h %RD3_PATH%\..\depends\include\D3DX12\ /s /y

:BUILD_VKBIND
echo "copy vkbind head file to depends"
cd %RD3_PATH%
xcopy %cd%\vkbind\vkbind.h %RD3_PATH%\..\depends\include\vkbind\ /y

:BUILD_MICROPROFILE
echo "copy microprofile head file to depends"
cd %RD3_PATH%
xcopy %cd%\microprofile\microprofile.h %RD3_PATH%\..\depends\include\microprofile\ /y
xcopy %cd%\microprofile\microprofile.cpp %RD3_PATH%\..\depends\include\microprofile\ /y
xcopy %cd%\microprofile\microprofile_html.h %RD3_PATH%\..\depends\include\microprofile\ /y

:BUILD_ASIO
echo "copy asio head file to depends"
cd %RD3_PATH%
xcopy %cd%\asio\asio\include\asio\*.* %RD3_PATH%\..\depends\include\asio\ /s /y
xcopy %cd%\asio\asio\include\asio.hpp %RD3_PATH%\..\depends\include\asio\ /s /y

:BUILD_KCP
echo "copy kcp head file to depends"
cd %RD3_PATH%
xcopy %cd%\kcp\ikcp.* %RD3_PATH%\..\depends\include\kcp\ /s /y

:BUILD_WEBGPU
echo "copy webgpu head file to depends"
cd %RD3_PATH%
xcopy %cd%\webgpu-headers\webgpu.h %RD3_PATH%\..\depends\include\webgpu\ /s /y

:BUILD_IMGUI
echo "copy imgui head file to depends"
cd %RD3_PATH%
xcopy %cd%\imgui\imgui.h %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imconfig.h %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imstb_textedit.h %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imstb_rectpack.h %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imstb_truetype.h %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imgui_internal.h %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imgui.cpp %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imgui_draw.cpp %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imgui_tables.cpp %RD3_PATH%\..\XE\source\GUI\ /y
xcopy %cd%\imgui\imgui_widgets.cpp %RD3_PATH%\..\XE\source\GUI\ /y

:BUILD_MINIZ_CPP
echo "copy miniz-cpp head file to depends"
cd %RD3_PATH%
xcopy %cd%\miniz-cpp\zip_file.hpp %RD3_PATH%\..\depends\include\miniz-cpp\ /s /y


:EXIT
pause