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
OBJECTDIR=build/Release/GNU-MacOSX

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/MellowMonster.o \
	${OBJECTDIR}/c_plugin.o

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
.build-conf: ${BUILD_SUBPROJECTS} ../great_game/dist/Release/GNU-MacOSX/libc_plugin.dylib

../great_game/dist/Release/GNU-MacOSX/libc_plugin.dylib: ${OBJECTFILES}
	${MKDIR} -p ../great_game/dist/Release/GNU-MacOSX
	${LINK.c} -dynamiclib -install_name libc_plugin.dylib -o ../great_game/dist/Release/GNU-MacOSX/libc_plugin.dylib -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/MellowMonster.o: MellowMonster.c 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/MellowMonster.o MellowMonster.c

${OBJECTDIR}/c_plugin.o: c_plugin.c 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -fPIC  -o ${OBJECTDIR}/c_plugin.o c_plugin.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} ../great_game/dist/Release/GNU-MacOSX/libc_plugin.dylib

# Subprojects
.clean-subprojects:
