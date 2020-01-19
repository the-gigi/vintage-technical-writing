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
OBJECTDIR=build/Release/GNU-${OperatingSystem}

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
.build-conf: $${BUILD_SUBPROJECTS} dist/Release/GNU-${OperatingSystem}/lib${Name}.a

dist/Release/GNU-${OperatingSystem}/lib${Name}.a: $${OBJECTFILES}
	$${MKDIR} -p dist/Release/GNU-${OperatingSystem}
	$${RM} dist/Release/GNU-${OperatingSystem}/lib${Name}.a
	$${AR} rv dist/Release/GNU-${OperatingSystem}/lib${Name}.a $${OBJECTFILES}
	$$(RANLIB) dist/Release/GNU-${OperatingSystem}/lib${Name}.a

${CompileFiles}

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	$${RM} -r build/Release
	$${RM} dist/Release/GNU-${OperatingSystem}/lib${Name}.a

# Subprojects
.clean-subprojects:
