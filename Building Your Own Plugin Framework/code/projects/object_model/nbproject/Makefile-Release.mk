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
	${OBJECTDIR}/Turn.o

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
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/GNU-MacOSX/libobject_model.a

dist/Release/GNU-MacOSX/libobject_model.a: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${RM} dist/Release/GNU-MacOSX/libobject_model.a
	${AR} rv dist/Release/GNU-MacOSX/libobject_model.a ${OBJECTFILES} 
	$(RANLIB) dist/Release/GNU-MacOSX/libobject_model.a

${OBJECTDIR}/Turn.o: Turn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/Turn.o Turn.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/libobject_model.a

# Subprojects
.clean-subprojects:
