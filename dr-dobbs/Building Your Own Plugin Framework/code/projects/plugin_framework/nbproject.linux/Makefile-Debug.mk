#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Path.o \
	${OBJECTDIR}/DynamicLibrary.o \
	${OBJECTDIR}/Directory.o \
	${OBJECTDIR}/PluginManager.o \
	${OBJECTDIR}/base.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-Linux-x86/libplugin_framework.a

dist/Debug/GNU-Linux-x86/libplugin_framework.a: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${RM} dist/Debug/GNU-Linux-x86/libplugin_framework.a
	${AR} rv dist/Debug/GNU-Linux-x86/libplugin_framework.a ${OBJECTFILES} 
	$(RANLIB) dist/Debug/GNU-Linux-x86/libplugin_framework.a

${OBJECTDIR}/Path.o: Path.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DPF_PLATFORM_LINUX -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/Path.o Path.cpp

${OBJECTDIR}/DynamicLibrary.o: DynamicLibrary.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DPF_PLATFORM_LINUX -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/DynamicLibrary.o DynamicLibrary.cpp

${OBJECTDIR}/Directory.o: Directory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DPF_PLATFORM_LINUX -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/Directory.o Directory.cpp

${OBJECTDIR}/PluginManager.o: PluginManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DPF_PLATFORM_LINUX -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/PluginManager.o PluginManager.cpp

${OBJECTDIR}/base.o: base.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DPF_PLATFORM_LINUX -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/base.o base.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libplugin_framework.a

# Subprojects
.clean-subprojects:
