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
OBJECTDIR=build/Debug/GNU-${OperatingSystem}

# Object Files
OBJECTFILES= \
${ObjectFiles}

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
.build-conf: $${BUILD_SUBPROJECTS} dist/Debug/GNU-${OperatingSystem}/lib${Name}.a

dist/Debug/GNU-${OperatingSystem}/lib${Name}.a: $${OBJECTFILES}
	$${MKDIR} -p dist/Debug/GNU-${OperatingSystem}
	$${RM} dist/Debug/GNU-${OperatingSystem}/lib${Name}.a
	$${AR} rv dist/Debug/GNU-${OperatingSystem}/lib${Name}.a $${OBJECTFILES}
	$$(RANLIB) dist/Debug/GNU-${OperatingSystem}/lib${Name}.a

${CompileFiles}

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	$${RM} -r build/Debug
	$${RM} dist/Debug/GNU-${OperatingSystem}/lib${Name}.a

# Subprojects
.clean-subprojects:
