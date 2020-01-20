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
.build-conf: ${BUILD_SUBPROJECTS} ../great_game/dist/Debug/GNU-MacOSX/libwrapper_plugin.dylib

../great_game/dist/Debug/GNU-MacOSX/libwrapper_plugin.dylib: ${OBJECTFILES}
	${MKDIR} -p ../great_game/dist/Debug/GNU-MacOSX
	${LINK.cc} -dynamiclib -install_name libwrapper_plugin.dylib -o ../great_game/dist/Debug/GNU-MacOSX/libwrapper_plugin.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/wrapper_plugin.o: wrapper_plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I.. -I../../include -I../../include/darwin86 -fPIC  -o ${OBJECTDIR}/wrapper_plugin.o wrapper_plugin.cpp

${OBJECTDIR}/PsychicPiranea.o: PsychicPiranea.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I.. -I../../include -I../../include/darwin86 -fPIC  -o ${OBJECTDIR}/PsychicPiranea.o PsychicPiranea.cpp

${OBJECTDIR}/GnarlyGolem.o: GnarlyGolem.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I.. -I../../include -I../../include/darwin86 -fPIC  -o ${OBJECTDIR}/GnarlyGolem.o GnarlyGolem.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} ../great_game/dist/Debug/GNU-MacOSX/libwrapper_plugin.dylib

# Subprojects
.clean-subprojects:
