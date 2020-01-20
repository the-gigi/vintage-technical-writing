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
	${OBJECTDIR}/wrapper_plugin.o \
	${OBJECTDIR}/PsychicPiranea.o \
	${OBJECTDIR}/GnarlyGolem.o

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
.build-conf: ${BUILD_SUBPROJECTS} ../great_game/dist/Release/GNU-Linux-x86/libwrapper_plugin.so

../great_game/dist/Release/GNU-Linux-x86/libwrapper_plugin.so: ${OBJECTFILES}
	${MKDIR} -p ../great_game/dist/Release/GNU-Linux-x86
	${LINK.cc} -shared -o ../great_game/dist/Release/GNU-Linux-x86/libwrapper_plugin.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/wrapper_plugin.o: wrapper_plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I.. -I../../include -I../../include/darwin86 -fPIC  -o ${OBJECTDIR}/wrapper_plugin.o wrapper_plugin.cpp

${OBJECTDIR}/PsychicPiranea.o: PsychicPiranea.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I.. -I../../include -I../../include/darwin86 -fPIC  -o ${OBJECTDIR}/PsychicPiranea.o PsychicPiranea.cpp

${OBJECTDIR}/GnarlyGolem.o: GnarlyGolem.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I.. -I../../include -I../../include/darwin86 -fPIC  -o ${OBJECTDIR}/GnarlyGolem.o GnarlyGolem.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} ../great_game/dist/Release/GNU-Linux-x86/libwrapper_plugin.so

# Subprojects
.clean-subprojects:
