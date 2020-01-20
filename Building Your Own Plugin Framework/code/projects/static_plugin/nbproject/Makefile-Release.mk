#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add custumized code.
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
	${OBJECTDIR}/static_plugin.o \
	${OBJECTDIR}/FidgetyPhantom.o

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
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/GNU-MacOSX/libstatic_plugin.a

dist/Release/GNU-MacOSX/libstatic_plugin.a: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${RM} dist/Release/GNU-MacOSX/libstatic_plugin.a
	${AR} rv dist/Release/GNU-MacOSX/libstatic_plugin.a ${OBJECTFILES} 
	$(RANLIB) dist/Release/GNU-MacOSX/libstatic_plugin.a

${OBJECTDIR}/static_plugin.o: static_plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/static_plugin.o static_plugin.cpp

${OBJECTDIR}/FidgetyPhantom.o: FidgetyPhantom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/FidgetyPhantom.o FidgetyPhantom.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/libstatic_plugin.a

# Subprojects
.clean-subprojects:
