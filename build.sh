#!/bin/bash
export CLUSTER_HMI_LIBRARY_PATH=$(pwd)/lib
if [ "$1" == "qnx" ]; then
	platform=qnx
	generator="Eclipse CDT4 - Unix Makefiles" 
	toolchain=toolchain_qnx.cmake
elif [ "$1" == "windows" ]; then
	platform=windows
	generator="Visual Studio 16 2019"  
	A=Win32
	T=v140
	toolchain=toolchain_windows.cmake
elif [ "$1" == "linux" ] ; then
	platform=linux
	generator="Unix Makefiles"
	toolchain=toolchain_linux.cmake
else
	echo "ERROR: Missing qnx/linux/windows as first parameter"
	exit 1
fi
if [ "$2" == "debug" ]; then
	config=Debug
elif [ "$2" == "release" ]; then
	config=Release
else
	echo "ERROR:Missing debug/release as second parameter"
	exit 1
fi

echo " "
echo "******************************** Build Env **********************************"
echo "[KANZI_PATH] " $KANZI_PATH
echo "[QNX_HOST] " $QNX_HOST
echo "[QNX_TARGET] " $QNX_TARGET
echo "[MULTICORE_COMPILE_COUNT] " $MULTICORE_COMPILE_COUNT
echo "[CLUSTER_HMI_LIBRARY_PATH] " $CLUSTER_HMI_LIBRARY_PATH
echo "*****************************************************************************"

if [ ! -d "build_${platform}" ]; then
	mkdir build_${platform}
fi
cd build_${platform}
if [ ! -d "${config}" ]; then
	mkdir ${config}
fi
cd ${config}
targettype=SHARED
if [ "$3" == "shared" ]; then
	targettype=SHARED
elif [ "$3" == "static" ]; then
	targettype=STATIC
fi 
if [ "$1" == "windows" ] ; then
	conan install ../.. -s build_type=Debug -s os=Windows -s arch=x86 -s compiler.toolset=v140 -s compiler.cppstd=14	
	cmake ../.. -G "${generator}" -A ${A} -T ${T}  -DCMAKE_TOOLCHAIN_FILE=${toolchain} -DCMAKE_BUILD_TYPE=${config}  -DTARGET_TYPE=${targettype}
else
	conan install ../.. -s build_type=${config} -s os=Qnx -s arch=aarch64le -s compiler=qcc  -s compiler.version=5.4 -s compiler.libcxx=cxx -s compiler.cppstd=14
	multicore="-- -j${MULTICORE_COMPILE_COUNT:-10}"
	cmake ../.. -G "${generator}" -DCMAKE_TOOLCHAIN_FILE=${toolchain} -DCMAKE_BUILD_TYPE=${config} -DTARGET_TYPE=${targettype}
fi

cmake --build . --config ${config} --target install  ${multicore}

if [ $? -ne 0 ]; then
	echo "ERROR: Cmake Install Failed!!"
	exit 1
fi


cd ../../..
