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




:BUILD_OZZ
cd %RD3_PATH%
echo "build ozz-animation debug"
mkdir .\ozz-animation\build
cd .\ozz-animation\build
cmake -Dozz_build_msvc_rt_dll=ON -Dozz_build_fbx=OFF -Dozz_build_gltf=OFF -Dozz_build_howtos=OFF -Dozz_build_postfix=OFF -Dozz_build_samples=OFF -Dozz_build_simd_ref=ON -Dozz_build_tests=OFF -Dozz_build_tools=OFF -Dozz_run_tests_headless=OFF -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019" 
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy ozz-animation debug file to depends"
xcopy %cd%\install\lib\ozz_animation.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\install\lib\ozz_base.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\install\lib\ozz_geometry.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\install\lib\ozz_options.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build ozz-animation release"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy ozz-animation release file to depends"
xcopy %cd%\install\lib\ozz_animation.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\lib\ozz_base.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\lib\ozz_geometry.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\lib\ozz_options.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y
git clean -fx -d


:BUILD_TBB
echo "build tbb debug"
cd %RD3_PATH%
mkdir .\tbb\build
cd .\tbb\build
cmake -DTBB_BUILD_STATIC=OFF -DTBB_BUILD_TESTS=OFF -DCMAKE_DEBUG_POSTFIX=_debug -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019" 
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy tbb debug file to depends"
xcopy %cd%\install\lib\*.* %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\install\bin\*.* %RD3_PATH%\..\depends\bin\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build tbb release"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy tbb release file to depends"
xcopy %cd%\install\lib\*.* %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\bin\*.* %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y
git clean -fx -d


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
echo "copy gtest release file to depends"
xcopy %cd%\install\bin\gtestd.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\gtestd.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
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
git clean -fx -d


:BUILD_GAINPUT
cd %RD3_PATH%
mkdir .\gainput\build
echo "build gainput debug"
cd %RD3_PATH%
cd .\gainput\build
cmake -DGAINPUT_SAMPLES=OFF -DGAINPUT_TESTS=OFF -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy gainput release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build gainput release"
cd %RD3_PATH%
cd .\gainput\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy gainput release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y
git clean -fx -d


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
git clean -fx -d


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
git clean -fx -d


:BUILD_BOX2D
cd %RD3_PATH%
mkdir .\box2d\build
echo "build box2d debug"
cd %RD3_PATH%
cd .\box2d\build
cmake -DBOX2D_BUILD_TESTBED=OFF -DBOX2D_BUILD_UNIT_TESTS=OFF -DCMAKE_INSTALL_PREFIX=.\install\ .. -G "Visual Studio 16 2019"
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Debug ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy box2d release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\debug\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\install\lib\*.pdb %RD3_PATH%\..\depends\lib\win\debug\ /y
del %cd%\install\ /f /s /q
echo "build box2d release"
cd %RD3_PATH%
cd .\box2d\build
msbuild.exe ".\INSTALL.vcxproj"  /m /nr:true ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:AppxBundlePlatforms=x64 ^
    /p:UseSubFolderForOutputDirDuringMultiPlatformBuild=false
echo "copy box2d release file to depends"
xcopy %cd%\install\bin\*.dll %RD3_PATH%\..\depends\bin\win\release\ /y
xcopy %cd%\install\lib\*.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\install\include\ %RD3_PATH%\..\depends\include\ /s /y
git clean -fx -d




:BUILD_BUNDLE
echo "build bundle debug"
cd %RD3_PATH%
cd .\bundle
cl /c /Zi /Od /D WIN32 /D _DEBUG /D _CONSOLE /EHsc /Fd"bundle.pdb" /MDd bundle.cpp
lib bundle.obj
echo "copy bundle debug file to depends"
xcopy %cd%\bundle.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\bundle.pdb %RD3_PATH%\..\depends\lib\win\debug\ /y
echo "build bundle release"
cd %RD3_PATH%
cd .\bundle
cl /c /EHsc /MDd bundle.cpp
lib bundle.obj
xcopy %cd%\bundle.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\bundle.h %RD3_PATH%\..\depends\include\bundle\ /s /y
git clean -fx -d


:BUILD_D3D12ALLOCATOR
echo "build D3D12MemoryAllocator debug"
cd %RD3_PATH%
cd .\D3D12MemoryAllocator\src
cl /c /Zi /Od /D WIN32 /D _DEBUG /D _CONSOLE /EHsc /Fd"D3D12MemAlloc.pdb" /MDd D3D12MemAlloc.cpp
lib D3D12MemAlloc.obj
echo "copy D3D12MemoryAllocator debug file to depends"
xcopy %cd%\D3D12MemAlloc.lib %RD3_PATH%\..\depends\lib\win\debug\ /y
xcopy %cd%\D3D12MemAlloc.pdb %RD3_PATH%\..\depends\lib\win\debug\ /y
echo "build D3D12MemoryAllocator release"
cd %RD3_PATH%
cd .\D3D12MemoryAllocator\src
cl /c /EHsc /MDd D3D12MemAlloc.cpp
lib D3D12MemAlloc.obj
echo "copy D3D12MemoryAllocator release file to depends"
xcopy %cd%\D3D12MemAlloc.lib %RD3_PATH%\..\depends\lib\win\release\ /y
xcopy %cd%\D3D12MemAlloc.h %RD3_PATH%\..\depends\include\D3D12MemAlloc\ /s /y
git clean -fx -d



:BUILD_STB
echo "copy stb head file to depends"
cd %RD3_PATH%
xcopy %cd%\stb\*.h %RD3_PATH%\..\depends\include\stb\ /s /y

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

:BUILD_VULKAN
echo "copy vulkan head file to depends"
cd %RD3_PATH%
xcopy %cd%\Vulkan-Headers\include\vulkan\*.* %RD3_PATH%\..\depends\include\vulkan\ /s /y

:BUILD_VOLK
echo "copy volk head file to depends"
cd %RD3_PATH%
xcopy %cd%\volk\volk.h %RD3_PATH%\..\depends\include\volk\ /s /y
xcopy %cd%\volk\volk.c %RD3_PATH%\..\depends\include\volk\ /s /y

:BUILD_VULKANALLOCATOR
echo "copy VulkanMemoryAllocator head file to depends"
cd %RD3_PATH%
xcopy %cd%\VulkanMemoryAllocator\include\vk_mem_alloc.h %RD3_PATH%\..\depends\include\VulkanMemAlloc\ /s /y
ren %RD3_PATH%\..\depends\include\VulkanMemAlloc\vk_mem_alloc.h VulkanMemAlloc.h

:BUILD_MICROPROFILE
echo "copy microprofile head file to depends"
cd %RD3_PATH%
xcopy %cd%\microprofile\microprofile.h %RD3_PATH%\..\depends\include\microprofile\ /y
xcopy %cd%\microprofile\microprofile.cpp %RD3_PATH%\..\depends\include\microprofile\ /y
xcopy %cd%\microprofile\microprofile_html.h %RD3_PATH%\..\depends\include\microprofile\ /y

:BUILD_IMGUI
echo "copy imgui head file to depends"
cd %RD3_PATH%
xcopy %cd%\imgui\*.h %RD3_PATH%\..\depends\include\imgui\ /y
xcopy %cd%\imgui\*.cpp %RD3_PATH%\..\depends\include\imgui\ /y


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




:EXIT
pause