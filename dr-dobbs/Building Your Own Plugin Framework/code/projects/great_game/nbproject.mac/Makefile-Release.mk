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
	${OBJECTDIR}/Hero.o \
	${OBJECTDIR}/great_game.o \
	${OBJECTDIR}/BattleManager.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../lib/darwin86 -ldl -lapr-1 -laprutil-1 ../plugin_framework/dist/Debug/GNU-MacOSX/libplugin_framework.a ../utils/dist/Debug/GNU-MacOSX/libutils.a ../object_model/dist/Debug/GNU-MacOSX/libobject_model.a ../static_plugin/dist/Debug/GNU-MacOSX/libstatic_plugin.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/GNU-MacOSX/great_game

dist/Release/GNU-MacOSX/great_game: ${BUILD_SUBPROJECTS}

dist/Release/GNU-MacOSX/great_game: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${LINK.cc} -o dist/Release/GNU-MacOSX/great_game ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Hero.o: Hero.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/Hero.o Hero.cpp

${OBJECTDIR}/great_game.o: great_game.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/great_game.o great_game.cpp

${OBJECTDIR}/BattleManager.o: BattleManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -DPF_PLATFORM_MAC -DAPR_DECLARE_STATIC -DAPU_DECLARE_STATIC -I../../include/ -I../../include/darwin86 -I.. -o ${OBJECTDIR}/BattleManager.o BattleManager.cpp

# Subprojects
.build-subprojects:
	cd ../plugin_framework && ${MAKE}  -f Makefile CONF=Debug
	cd ../utils && ${MAKE}  -f Makefile CONF=Debug
	cd ../object_model && ${MAKE}  -f Makefile CONF=Debug
	cd ../static_plugin && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/great_game

# Subprojects
.clean-subprojects:
	cd ../plugin_framework && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../utils && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../object_model && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../static_plugin && ${MAKE}  -f Makefile CONF=Debug clean
