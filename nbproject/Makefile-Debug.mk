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
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Config.o \
	${OBJECTDIR}/ConfigFunctions.o \
	${OBJECTDIR}/Coord.o \
	${OBJECTDIR}/DampingFunctions.o \
	${OBJECTDIR}/Debug.o \
	${OBJECTDIR}/DeckProp.o \
	${OBJECTDIR}/Diaphragm.o \
	${OBJECTDIR}/Element.o \
	${OBJECTDIR}/EtabsAreaProps.o \
	${OBJECTDIR}/EtabsLine.o \
	${OBJECTDIR}/EtabsNode.o \
	${OBJECTDIR}/EtabsWall.o \
	${OBJECTDIR}/FloatCompare.o \
	${OBJECTDIR}/FoundationNode.o \
	${OBJECTDIR}/Grid.o \
	${OBJECTDIR}/LoadCombination.o \
	${OBJECTDIR}/LoadPattern.o \
	${OBJECTDIR}/Model.o \
	${OBJECTDIR}/ModelForces.o \
	${OBJECTDIR}/NewElement.o \
	${OBJECTDIR}/NodalRestraint.o \
	${OBJECTDIR}/Node.o \
	${OBJECTDIR}/OutputModel.o \
	${OBJECTDIR}/Parse.o \
	${OBJECTDIR}/PointSpring.o \
	${OBJECTDIR}/PointSpringFunctions.o \
	${OBJECTDIR}/Releases.o \
	${OBJECTDIR}/RestraintFunctions.o \
	${OBJECTDIR}/SectionFunctions.o \
	${OBJECTDIR}/Slab.o \
	${OBJECTDIR}/SpecialColumn.o \
	${OBJECTDIR}/StaticLoad.o \
	${OBJECTDIR}/SteelArea.o \
	${OBJECTDIR}/SteelLine.o \
	${OBJECTDIR}/SteelNode.o \
	${OBJECTDIR}/SteelSection.o \
	${OBJECTDIR}/Story.o \
	${OBJECTDIR}/TimeHistory.o \
	${OBJECTDIR}/VerticalConnFunctions.o \
	${OBJECTDIR}/VerticalConstraint.o \
	${OBJECTDIR}/WallFunctions.o \
	${OBJECTDIR}/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/etabs2steel

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/etabs2steel: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/etabs2steel ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Config.o: Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config.o Config.cpp

${OBJECTDIR}/ConfigFunctions.o: ConfigFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ConfigFunctions.o ConfigFunctions.cpp

${OBJECTDIR}/Coord.o: Coord.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Coord.o Coord.cpp

${OBJECTDIR}/DampingFunctions.o: DampingFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DampingFunctions.o DampingFunctions.cpp

${OBJECTDIR}/Debug.o: Debug.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Debug.o Debug.cpp

${OBJECTDIR}/DeckProp.o: DeckProp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DeckProp.o DeckProp.cpp

${OBJECTDIR}/Diaphragm.o: Diaphragm.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Diaphragm.o Diaphragm.cpp

${OBJECTDIR}/Element.o: Element.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Element.o Element.cpp

${OBJECTDIR}/EtabsAreaProps.o: EtabsAreaProps.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EtabsAreaProps.o EtabsAreaProps.cpp

${OBJECTDIR}/EtabsLine.o: EtabsLine.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EtabsLine.o EtabsLine.cpp

${OBJECTDIR}/EtabsNode.o: EtabsNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EtabsNode.o EtabsNode.cpp

${OBJECTDIR}/EtabsWall.o: EtabsWall.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EtabsWall.o EtabsWall.cpp

${OBJECTDIR}/FloatCompare.o: FloatCompare.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FloatCompare.o FloatCompare.cpp

${OBJECTDIR}/FoundationNode.o: FoundationNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FoundationNode.o FoundationNode.cpp

${OBJECTDIR}/Grid.o: Grid.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Grid.o Grid.cpp

${OBJECTDIR}/LoadCombination.o: LoadCombination.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LoadCombination.o LoadCombination.cpp

${OBJECTDIR}/LoadPattern.o: LoadPattern.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LoadPattern.o LoadPattern.cpp

${OBJECTDIR}/Model.o: Model.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Model.o Model.cpp

${OBJECTDIR}/ModelForces.o: ModelForces.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ModelForces.o ModelForces.cpp

${OBJECTDIR}/NewElement.o: NewElement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NewElement.o NewElement.cpp

${OBJECTDIR}/NodalRestraint.o: NodalRestraint.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NodalRestraint.o NodalRestraint.cpp

${OBJECTDIR}/Node.o: Node.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Node.o Node.cpp

${OBJECTDIR}/OutputModel.o: OutputModel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OutputModel.o OutputModel.cpp

${OBJECTDIR}/Parse.o: Parse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parse.o Parse.cpp

${OBJECTDIR}/PointSpring.o: PointSpring.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PointSpring.o PointSpring.cpp

${OBJECTDIR}/PointSpringFunctions.o: PointSpringFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PointSpringFunctions.o PointSpringFunctions.cpp

${OBJECTDIR}/Releases.o: Releases.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Releases.o Releases.cpp

${OBJECTDIR}/RestraintFunctions.o: RestraintFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/RestraintFunctions.o RestraintFunctions.cpp

${OBJECTDIR}/SectionFunctions.o: SectionFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SectionFunctions.o SectionFunctions.cpp

${OBJECTDIR}/Slab.o: Slab.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Slab.o Slab.cpp

${OBJECTDIR}/SpecialColumn.o: SpecialColumn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SpecialColumn.o SpecialColumn.cpp

${OBJECTDIR}/StaticLoad.o: StaticLoad.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StaticLoad.o StaticLoad.cpp

${OBJECTDIR}/SteelArea.o: SteelArea.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SteelArea.o SteelArea.cpp

${OBJECTDIR}/SteelLine.o: SteelLine.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SteelLine.o SteelLine.cpp

${OBJECTDIR}/SteelNode.o: SteelNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SteelNode.o SteelNode.cpp

${OBJECTDIR}/SteelSection.o: SteelSection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SteelSection.o SteelSection.cpp

${OBJECTDIR}/Story.o: Story.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Story.o Story.cpp

${OBJECTDIR}/TimeHistory.o: TimeHistory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TimeHistory.o TimeHistory.cpp

${OBJECTDIR}/VerticalConnFunctions.o: VerticalConnFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/VerticalConnFunctions.o VerticalConnFunctions.cpp

${OBJECTDIR}/VerticalConstraint.o: VerticalConstraint.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/VerticalConstraint.o VerticalConstraint.cpp

${OBJECTDIR}/WallFunctions.o: WallFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WallFunctions.o WallFunctions.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/etabs2steel

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
