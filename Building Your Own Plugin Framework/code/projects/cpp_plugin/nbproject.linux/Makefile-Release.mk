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
OBJECTDIR=build/Release/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/StationarySatan.o \
	${OBJECTDIR}/KillerBunny.o \
	${OBJECTDIR}/cpp_plugin.o

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
.build-conf: ${BUILD_SUBPROJECTS} ../great_game/dist/Release/GNU-Linux-x86/libcpp_plugin.so

../great_game/dist/Release/GNU-Linux-x86/libcpp_plugin.so: ${OBJECTFILES}
	${MKDIR} -p ../great_game/dist/Release/GNU-Linux-x86
	${LINK.cc} -dynamiclib -install_name libcpp_plugin.so -o ../great_game/dist/Release/GNU-Linux-x86/libcpp_plugin.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/StationarySatan.o: StationarySatan.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/StationarySatan.o StationarySatan.cpp

${OBJECTDIR}/KillerBunny.o: KillerBunny.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/KillerBunny.o KillerBunny.cpp

${OBJECTDIR}/cpp_plugin.o: cpp_plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/cpp_plugin.o cpp_plugin.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} ../great_game/dist/Release/GNU-Linux-x86/libcpp_plugin.so

# Subprojects
.clean-subprojects:
