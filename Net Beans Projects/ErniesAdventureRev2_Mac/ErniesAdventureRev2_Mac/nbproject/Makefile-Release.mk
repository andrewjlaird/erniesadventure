#
# Generated Makefile - do not edit!
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
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SceneManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PathCharacter.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SpriteManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/KeyboardBrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Character.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Criteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Brain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/StartNextStepAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/EndGameAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/TextInputCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/MovingCharacter.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Action.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ErniesAdventureMain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ChangeSceneAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayTextAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Sprite.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PathBrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AddToScoreAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/FollowBrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SetVariableValueAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SearchBrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/VariableManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/InSceneCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/TimedCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/BasicCharacter.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/LayeredObject.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Inventory.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayDriver.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/WanderBrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AvoidBrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Event.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Scene.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/MoveObjectAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayManager.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/erniesadventurerev2_mac

dist/Release/GNU-MacOSX/erniesadventurerev2_mac: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/erniesadventurerev2_mac ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SceneManager.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SceneManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SceneManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SceneManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PathCharacter.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathCharacter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PathCharacter.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathCharacter.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SpriteManager.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SpriteManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SpriteManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SpriteManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/KeyboardBrain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/KeyboardBrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/KeyboardBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/KeyboardBrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Character.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Character.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Character.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Character.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Criteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Criteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Criteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Criteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Brain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Brain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Brain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Brain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/StartNextStepAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StartNextStepAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/StartNextStepAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StartNextStepAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/EndGameAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/EndGameAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/EndGameAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/EndGameAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/TextInputCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TextInputCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/TextInputCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TextInputCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/MovingCharacter.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MovingCharacter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/MovingCharacter.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MovingCharacter.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Action.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Action.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Action.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Action.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ErniesAdventureMain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ErniesAdventureMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ErniesAdventureMain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ErniesAdventureMain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ChangeSceneAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ChangeSceneAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ChangeSceneAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ChangeSceneAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayTextAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayTextAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayTextAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayTextAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Sprite.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Sprite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Sprite.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Sprite.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PathBrain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathBrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PathBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathBrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AddToScoreAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddToScoreAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AddToScoreAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddToScoreAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/FollowBrain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/FollowBrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/FollowBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/FollowBrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SetVariableValueAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SetVariableValueAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SetVariableValueAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SetVariableValueAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SearchBrain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SearchBrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/SearchBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SearchBrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/VariableManager.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/VariableManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/InSceneCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/InSceneCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/InSceneCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/InSceneCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/TimedCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TimedCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/TimedCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TimedCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/BasicCharacter.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/BasicCharacter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/BasicCharacter.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/BasicCharacter.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/LayeredObject.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/LayeredObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/LayeredObject.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/LayeredObject.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Inventory.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Inventory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Inventory.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Inventory.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayDriver.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayDriver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayDriver.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayDriver.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/WanderBrain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/WanderBrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/WanderBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/WanderBrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AvoidBrain.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AvoidBrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/AvoidBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AvoidBrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Event.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Event.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Event.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Event.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Scene.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/Scene.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Scene.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/MoveObjectAction.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MoveObjectAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/MoveObjectAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MoveObjectAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayManager.o: nbproject/Makefile-${CND_CONF}.mk ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/Common/Ernies_Adventure/ErniesAdventureCode/src/DisplayManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayManager.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/erniesadventurerev2_mac

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
