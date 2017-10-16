#!/bin/bash
export CVM_TARGET_TOOLS_PREFIX=/home/android/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-
export GNU_TOOLS_DIR=/home/android/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi
export WORK_DIR=$PWD
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
export JAVA_PATH=$JAVA_HOME
export JDK_HOME=$JAVA_HOME
export CVM_JAVABIN=$JAVA_HOME/bin
export CVM_PRELOAD_LIB=true
export JAVAME_LEGAL_DIR=$PWD/legal
export PATH=$JAVA_HOME:$PATH
export CVM_DEBUG=false
export CVM_JIT=false
export USE_AAPCS=true
export J2ME_CLASSLIB=foundation
export TOOLS_DIR=$WORK_DIR/tools

export CVM_DEFINES="--sysroot=/home/android/android-ndk-r14b/platforms/android-24/arch-arm/ -fPIE"
export LINK_ARCH_FLAGS="--sysroot=/home/android/android-ndk-r14b/platforms/android-24/arch-arm/ -fPIE -pie"
export PCSL_CFLAGS="$CVM_DEFINES"
export PCSL_LINK_FLAGS="$LINK_ARCH_FLAGS"
export USE_MIDP=true
export USE_JPEG=true

cd cdc/build/linux-arm-generic
make -f GNUmakefile bin VERBOSE_BUILD=true USE_VERBOSE_MAKE=true CVM_COMPILER_INCOMPATIBLE=false USE_EXTRA_GCC_WARNINGS=false $@
