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
OBJECTDIR=build/Debug/GNU-MacOSX

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/utils.o

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
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-MacOSX/libutils.a

dist/Debug/GNU-MacOSX/libutils.a: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-MacOSX
	${RM} dist/Debug/GNU-MacOSX/libutils.a
	${AR} rv dist/Debug/GNU-MacOSX/libutils.a ${OBJECTFILES} 
	$(RANLIB) dist/Debug/GNU-MacOSX/libutils.a

${OBJECTDIR}/utils.o: utils.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g3 -gdwarf-2 -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/utils.o utils.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-MacOSX/libutils.a

# Subprojects
.clean-subprojects:
