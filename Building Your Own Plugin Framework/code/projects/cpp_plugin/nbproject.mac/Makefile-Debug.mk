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
OBJECTDIR=build/Debug/GNU-MacOSX

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cpp_plugin.o \
	${OBJECTDIR}/KillerBunny.o \
	${OBJECTDIR}/StationarySatan.o

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
.build-conf: ${BUILD_SUBPROJECTS} ../great_game/dist/Debug/GNU-MacOSX/libcpp_plugin.dylib

../great_game/dist/Debug/GNU-MacOSX/libcpp_plugin.dylib: ${OBJECTFILES}
	${MKDIR} -p ../great_game/dist/Debug/GNU-MacOSX
	${LINK.cc} -dynamiclib -install_name libcpp_plugin.dylib -o ../great_game/dist/Debug/GNU-MacOSX/libcpp_plugin.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/cpp_plugin.o: cpp_plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/cpp_plugin.o cpp_plugin.cpp

${OBJECTDIR}/KillerBunny.o: KillerBunny.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/KillerBunny.o KillerBunny.cpp

${OBJECTDIR}/StationarySatan.o: StationarySatan.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/StationarySatan.o StationarySatan.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} ../great_game/dist/Debug/GNU-MacOSX/libcpp_plugin.dylib

# Subprojects
.clean-subprojects:
