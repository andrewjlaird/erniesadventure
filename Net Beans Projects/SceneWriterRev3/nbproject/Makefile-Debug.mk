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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/4d9236b7/Action.o \
	${OBJECTDIR}/_ext/4d9236b7/AddObjectToInventoryAction.o \
	${OBJECTDIR}/_ext/4d9236b7/AddToScoreAction.o \
	${OBJECTDIR}/_ext/4d9236b7/AudioManager.o \
	${OBJECTDIR}/_ext/4d9236b7/AvoidBrain.o \
	${OBJECTDIR}/_ext/4d9236b7/BasicCharacter.o \
	${OBJECTDIR}/_ext/4d9236b7/Brain.o \
	${OBJECTDIR}/_ext/4d9236b7/ChangeSceneAction.o \
	${OBJECTDIR}/_ext/4d9236b7/Character.o \
	${OBJECTDIR}/_ext/4d9236b7/CharacterSpawner.o \
	${OBJECTDIR}/_ext/4d9236b7/ChaseBrain.o \
	${OBJECTDIR}/_ext/4d9236b7/Criteria.o \
	${OBJECTDIR}/_ext/4d9236b7/DisplayDriver.o \
	${OBJECTDIR}/_ext/4d9236b7/DisplayManager.o \
	${OBJECTDIR}/_ext/4d9236b7/DisplayTextAction.o \
	${OBJECTDIR}/_ext/4d9236b7/EndGameAction.o \
	${OBJECTDIR}/_ext/4d9236b7/Event.o \
	${OBJECTDIR}/_ext/4d9236b7/FollowBrain.o \
	${OBJECTDIR}/_ext/4d9236b7/InSceneCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/Inventory.o \
	${OBJECTDIR}/_ext/4d9236b7/KeyboardBrain.o \
	${OBJECTDIR}/_ext/4d9236b7/LayeredObject.o \
	${OBJECTDIR}/_ext/4d9236b7/MoveObjectAction.o \
	${OBJECTDIR}/_ext/4d9236b7/MovingCharacter.o \
	${OBJECTDIR}/_ext/4d9236b7/ObjectInAreaCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/ObjectInCurrentSceneCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/ObjectInInventoryCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/ObjectsAreNearCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/PathBrain.o \
	${OBJECTDIR}/_ext/4d9236b7/PathCharacter.o \
	${OBJECTDIR}/_ext/4d9236b7/PlaceObjectInGameAction.o \
	${OBJECTDIR}/_ext/4d9236b7/RemoveObjectFromGameAction.o \
	${OBJECTDIR}/_ext/4d9236b7/RemoveObjectFromInventoryAction.o \
	${OBJECTDIR}/_ext/4d9236b7/Scene.o \
	${OBJECTDIR}/_ext/4d9236b7/SceneManager.o \
	${OBJECTDIR}/_ext/4d9236b7/SearchBrain.o \
	${OBJECTDIR}/_ext/4d9236b7/SetVariableValueAction.o \
	${OBJECTDIR}/_ext/4d9236b7/SpawnCharacterAction.o \
	${OBJECTDIR}/_ext/4d9236b7/Sprite.o \
	${OBJECTDIR}/_ext/4d9236b7/SpriteManager.o \
	${OBJECTDIR}/_ext/4d9236b7/StartNextStepAction.o \
	${OBJECTDIR}/_ext/4d9236b7/StopSpriteRunningAction.o \
	${OBJECTDIR}/_ext/4d9236b7/SubtractFromScoreAction.o \
	${OBJECTDIR}/_ext/4d9236b7/TextInputCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/TimedCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/ToggleFollowingAction.o \
	${OBJECTDIR}/_ext/4d9236b7/VariableHasValueCriteria.o \
	${OBJECTDIR}/_ext/4d9236b7/VariableManager.o \
	${OBJECTDIR}/_ext/4d9236b7/WanderBrain.o \
	${OBJECTDIR}/_ext/c88eb9ba/BaseWindow.o \
	${OBJECTDIR}/_ext/c88eb9ba/ColorWindow.o \
	${OBJECTDIR}/_ext/c88eb9ba/SceneWindow.o \
	${OBJECTDIR}/_ext/c88eb9ba/SceneWriter2Main.o \
	${OBJECTDIR}/_ext/c88eb9ba/SpriteWindow.o


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
LDLIBSOPTIONS=-L../../allegro-4.4.2-mingw-3.4.5/include ../../allegro-4.4.2-mingw-3.4.5/bin/allegro-4.4.2-monolith-mt.dll

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/SceneWriter.exe
	${CP} ../../allegro-4.4.2-mingw-3.4.5/bin/allegro-4.4.2-monolith-mt.dll ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/SceneWriter.exe: ../../allegro-4.4.2-mingw-3.4.5/bin/allegro-4.4.2-monolith-mt.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/SceneWriter.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/SceneWriter ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/4d9236b7/Action.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Action.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Action.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Action.cpp

${OBJECTDIR}/_ext/4d9236b7/AddObjectToInventoryAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/AddObjectToInventoryAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddObjectToInventoryAction.cpp

${OBJECTDIR}/_ext/4d9236b7/AddToScoreAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddToScoreAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/AddToScoreAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AddToScoreAction.cpp

${OBJECTDIR}/_ext/4d9236b7/AudioManager.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AudioManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/AudioManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AudioManager.cpp

${OBJECTDIR}/_ext/4d9236b7/AvoidBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AvoidBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/AvoidBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/AvoidBrain.cpp

${OBJECTDIR}/_ext/4d9236b7/BasicCharacter.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/BasicCharacter.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/BasicCharacter.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/BasicCharacter.cpp

${OBJECTDIR}/_ext/4d9236b7/Brain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Brain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Brain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Brain.cpp

${OBJECTDIR}/_ext/4d9236b7/ChangeSceneAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ChangeSceneAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ChangeSceneAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ChangeSceneAction.cpp

${OBJECTDIR}/_ext/4d9236b7/Character.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Character.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Character.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Character.cpp

${OBJECTDIR}/_ext/4d9236b7/CharacterSpawner.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/CharacterSpawner.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/CharacterSpawner.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/CharacterSpawner.cpp

${OBJECTDIR}/_ext/4d9236b7/ChaseBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ChaseBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ChaseBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ChaseBrain.cpp

${OBJECTDIR}/_ext/4d9236b7/Criteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Criteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Criteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Criteria.cpp

${OBJECTDIR}/_ext/4d9236b7/DisplayDriver.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayDriver.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/DisplayDriver.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayDriver.cpp

${OBJECTDIR}/_ext/4d9236b7/DisplayManager.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/DisplayManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayManager.cpp

${OBJECTDIR}/_ext/4d9236b7/DisplayTextAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayTextAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/DisplayTextAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/DisplayTextAction.cpp

${OBJECTDIR}/_ext/4d9236b7/EndGameAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/EndGameAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/EndGameAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/EndGameAction.cpp

${OBJECTDIR}/_ext/4d9236b7/Event.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Event.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Event.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Event.cpp

${OBJECTDIR}/_ext/4d9236b7/FollowBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/FollowBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/FollowBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/FollowBrain.cpp

${OBJECTDIR}/_ext/4d9236b7/InSceneCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/InSceneCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/InSceneCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/InSceneCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/Inventory.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Inventory.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Inventory.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Inventory.cpp

${OBJECTDIR}/_ext/4d9236b7/KeyboardBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/KeyboardBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/KeyboardBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/KeyboardBrain.cpp

${OBJECTDIR}/_ext/4d9236b7/LayeredObject.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/LayeredObject.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/LayeredObject.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/LayeredObject.cpp

${OBJECTDIR}/_ext/4d9236b7/MoveObjectAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MoveObjectAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/MoveObjectAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MoveObjectAction.cpp

${OBJECTDIR}/_ext/4d9236b7/MovingCharacter.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MovingCharacter.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/MovingCharacter.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/MovingCharacter.cpp

${OBJECTDIR}/_ext/4d9236b7/ObjectInAreaCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ObjectInAreaCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInAreaCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/ObjectInCurrentSceneCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ObjectInCurrentSceneCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInCurrentSceneCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/ObjectInInventoryCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ObjectInInventoryCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectInInventoryCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/ObjectsAreNearCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ObjectsAreNearCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ObjectsAreNearCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/PathBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/PathBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathBrain.cpp

${OBJECTDIR}/_ext/4d9236b7/PathCharacter.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathCharacter.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/PathCharacter.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PathCharacter.cpp

${OBJECTDIR}/_ext/4d9236b7/PlaceObjectInGameAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/PlaceObjectInGameAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/PlaceObjectInGameAction.cpp

${OBJECTDIR}/_ext/4d9236b7/RemoveObjectFromGameAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/RemoveObjectFromGameAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromGameAction.cpp

${OBJECTDIR}/_ext/4d9236b7/RemoveObjectFromInventoryAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/RemoveObjectFromInventoryAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/RemoveObjectFromInventoryAction.cpp

${OBJECTDIR}/_ext/4d9236b7/Scene.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Scene.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Scene.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Scene.cpp

${OBJECTDIR}/_ext/4d9236b7/SceneManager.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SceneManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/SceneManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SceneManager.cpp

${OBJECTDIR}/_ext/4d9236b7/SearchBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SearchBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/SearchBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SearchBrain.cpp

${OBJECTDIR}/_ext/4d9236b7/SetVariableValueAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SetVariableValueAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/SetVariableValueAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SetVariableValueAction.cpp

${OBJECTDIR}/_ext/4d9236b7/SpawnCharacterAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SpawnCharacterAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/SpawnCharacterAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SpawnCharacterAction.cpp

${OBJECTDIR}/_ext/4d9236b7/Sprite.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Sprite.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/Sprite.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/Sprite.cpp

${OBJECTDIR}/_ext/4d9236b7/SpriteManager.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SpriteManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/SpriteManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SpriteManager.cpp

${OBJECTDIR}/_ext/4d9236b7/StartNextStepAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StartNextStepAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/StartNextStepAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StartNextStepAction.cpp

${OBJECTDIR}/_ext/4d9236b7/StopSpriteRunningAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/StopSpriteRunningAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/StopSpriteRunningAction.cpp

${OBJECTDIR}/_ext/4d9236b7/SubtractFromScoreAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/SubtractFromScoreAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/SubtractFromScoreAction.cpp

${OBJECTDIR}/_ext/4d9236b7/TextInputCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TextInputCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/TextInputCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TextInputCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/TimedCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TimedCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/TimedCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/TimedCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/ToggleFollowingAction.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ToggleFollowingAction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/ToggleFollowingAction.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/ToggleFollowingAction.cpp

${OBJECTDIR}/_ext/4d9236b7/VariableHasValueCriteria.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/VariableHasValueCriteria.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableHasValueCriteria.cpp

${OBJECTDIR}/_ext/4d9236b7/VariableManager.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/VariableManager.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/VariableManager.cpp

${OBJECTDIR}/_ext/4d9236b7/WanderBrain.o: ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/WanderBrain.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/4d9236b7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4d9236b7/WanderBrain.o ../../Common/Ernies\ Adventure/ErniesAdventureCode/src/WanderBrain.cpp

${OBJECTDIR}/_ext/c88eb9ba/BaseWindow.o: ../../Common/Ernies\ Adventure/SceneWriter2Code/src/BaseWindow.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c88eb9ba
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c88eb9ba/BaseWindow.o ../../Common/Ernies\ Adventure/SceneWriter2Code/src/BaseWindow.cpp

${OBJECTDIR}/_ext/c88eb9ba/ColorWindow.o: ../../Common/Ernies\ Adventure/SceneWriter2Code/src/ColorWindow.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c88eb9ba
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c88eb9ba/ColorWindow.o ../../Common/Ernies\ Adventure/SceneWriter2Code/src/ColorWindow.cpp

${OBJECTDIR}/_ext/c88eb9ba/SceneWindow.o: ../../Common/Ernies\ Adventure/SceneWriter2Code/src/SceneWindow.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c88eb9ba
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c88eb9ba/SceneWindow.o ../../Common/Ernies\ Adventure/SceneWriter2Code/src/SceneWindow.cpp

${OBJECTDIR}/_ext/c88eb9ba/SceneWriter2Main.o: ../../Common/Ernies\ Adventure/SceneWriter2Code/src/SceneWriter2Main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c88eb9ba
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c88eb9ba/SceneWriter2Main.o ../../Common/Ernies\ Adventure/SceneWriter2Code/src/SceneWriter2Main.cpp

${OBJECTDIR}/_ext/c88eb9ba/SpriteWindow.o: ../../Common/Ernies\ Adventure/SceneWriter2Code/src/SpriteWindow.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c88eb9ba
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Common/Ernies\ Adventure/ErniesAdventureCode/inc -I../../Common/Ernies\ Adventure/SceneWriter2Code/inc -I../../allegro-mingw-4.2.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c88eb9ba/SpriteWindow.o ../../Common/Ernies\ Adventure/SceneWriter2Code/src/SpriteWindow.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/allegro-4.4.2-monolith-mt.dll
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/SceneWriter.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
