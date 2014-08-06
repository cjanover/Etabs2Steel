//
//  Model.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Model__
#define __Etabs2Steel__Model__

#include <iostream>
#include <sstream>
#include <map>
#include <regex>
#include <cmath>

#include <stdlib.h>

//Config File
#include "Config.h"

//Etabs Info
#include "EtabsNode.h"
#include "EtabsLine.h"
#include "Story.h"
#include "Grid.h"

//Steel Info
#include "SteelNode.h"
#include "SteelLine.h"

//Walls/Slabs
#include "EtabsAreaProps.h"
#include "DeckProp.h"
#include "EtabsWall.h"
#include "SteelArea.h"

//Restraints
#include "NodalRestraint.h"

//Constraints
#include "VerticalConstraint.h"

//Special Columns
#include "SpecialColumn.h"

//Point Springs
#include "PointSpring.h"
#include "FoundationNode.h"

//Loads
#include "LoadPattern.h"
#include "StaticLoad.h"
#include "LoadCombination.h"

//Sections
#include "SteelSection.h"
#include "Slab.h"


#include "FloatCompare.h"
#include "Debug.h"
#include "Parse.h"


extern std::map <std::string, std::string> PARSESTRINGS;
extern std::map<unsigned int, unsigned int> nodeNumberLookup;
extern std::string saveDirectory;
extern int debugLevel;

class Model
{
    public:
        Model():numNodes(0), numLineElements(0), numPrimaryNodes(0), numSecondaryNodes(0), numPrimCol(0), numPrimBeam(0), numPrimBrace(0), numPrimLineElements(0),
        numSecCol(0), numSecBeam(0), numSecBrace(0), numSecLineElements(0),
        secondaryEtabsDirection(GetEtabsSecondaryDirection(primaryEtabsDirection)){};
    
    
        //Set Info
        void ParseConfig();
        void ReadInput();
        void ParseUnits(std::string line);
    
        void newEtabsNode(EtabsNode nNode){etabsNodes[nNode.Num()] = nNode;};
        void newEtabsStory(Story nStory);
        void newEtabsGrid(Grid nGrid);
        void newEtabsLine(EtabsLine nLine);
    
        void newEtabsAreaProp(EtabsAreaProps nProp);
        void newDeckProp(DeckProp nProp);
        void newEtabsWall(EtabsWall nWall);
    
        void newNodalRestraint(NodalRestraint restraint);
    
        void newWideFlange(const std::string label, const float D, const float B, const float TF, const float TW);
    
        void newSteelNode(SteelNode nNode);
        void newSteelLine(SteelLine nLine);
        void newSteelArea(SteelArea nArea);
    
        void newFndNodeDef(PointSpring nSpring);
        void NewFndNode(FoundationNode nFndNode);
    
        void newLoadPattern(LoadPattern pattern);
        void newStaticLoad(StaticLoad load);
        void newLoadCombo(LoadCombination combo);
    
        void newCustomSection(const std::string label, const float D, const float B, const float TF, const float TW, const std::string material);
        void newRegSection(const std::string label, const std::string material);
    
        void findMaxFrameWidth();
        float getMaxFrameWidth(){return maxFrameWidth;};
    
        void setNBEL();
    
        void SetSteelNodeFixity();
        std::string FindStoryName(float elev);
    
        //Output Functions
        void PrepareModel();
        void RenumberNodes();
        void ReorderNodes();
        void CreateSteelInput();
        void CreateOtherSteelInput();
        void RunModel();
        void CleanGaruda();
        void RenameFiles();
    
        void SteelCoordSwitch();
    
        std::ostringstream OutputNodes();
        std::ostringstream OutputLineElements();
        std::ostringstream OutputSpecialColumns();
        std::ostringstream OutputFoundationNodes();
        std::ostringstream OutputBasementWallElements();
        std::ostringstream OutputFiberSectionMod();
        std::ostringstream OutputFiberStrainSectionMod();
        std::ostringstream OutputConfig();
        std::ostringstream OutputSegmentLengths();
        std::ostringstream OutputGAMULT();
        std::ostringstream OutputNNPBF();
        std::ostringstream OutputNVCONEL();
        std::ostringstream OutputTH();
        std::ostringstream OutputMaterial();
        void OutputGridShiftConversion();
        void OutputDeckInfo();
    
        void CalculateNodalShift();
        void CalculateNodalForces();
        void CalculateNodalMass();
        void CalculateNVCONEL();
    
        void CalculateNumNodesPerFloor();
        void CalculateDiaphragm();

        void CalculateSlabs();
    
        void AddRollerSecondary();
    
        void AddFoundationNodes();
    
        bool SecondaryNodeInPrimary(SteelNode node);
        void PrepareConfig();
    
        //Damping
        void SetDamping();
        void AddSpecialColumns();
        void CalcMinMaxEle();
        float CalcStoryShear(float curEle);
        void CalcValsPerFloor();
    
        //Sections
        void ParsePrebuildSteel(){steelSection.ParsePrebuildDatabse(config.GetRawConfigStart("STEELSECTION"), config.GetRawConfigEnd("STEELSECTION"), sectionConversionMult);};
        void CreateSlabDatabase();
        SteelSection GetSteelSection(){return steelSection;};
    
        //Config Functions
        int FindNodeNumber(float x, float y, float z, Coord::Direction searchDir);
        int FindElementNumber(int node1, int node2, Coord::Direction searchDir);
    
        void SetResponseTH();
        void ParseExTH();
        void ParseAlphaVC();
        float SetAlphaVC(Coord searchCoords);
    
        //Garuda Functions
        void RunAnalysis();
    


        
    private:
        //Nodes
        std::map<unsigned int, EtabsNode> etabsNodes;
        std::map<unsigned int, SteelNode> primarySteelNodes;
        std::map<unsigned int, SteelNode> secondarySteelNodes;
    
    
        std::map<std::string, EtabsLine> etabsLines;
    
        //Line Elements
        std::map<unsigned int, SteelLine> primarySteelLines;
        std::map<unsigned int, SteelLine> secondarySteelLines;
    
        //Restraints
        std::map<std::pair<unsigned int, std::string>, NodalRestraint> restrainedNodes;

        //Walls and Slabs
        std::map<std::string, EtabsAreaProps> etabsAreaProps;
        std::map<std::string, DeckProp> etabsDeckProps;
        std::map<std::string, EtabsWall> etabsAreaElements;
        std::map<unsigned long, SteelArea> steelWallElements;
        std::map<unsigned long, SteelArea> steelSlabElements;
        std::map<unsigned long, SteelArea> steelDeckElements;
        std::map<std::string, unsigned int> deckConversion;

        //Foundation Nodes
        std::map<std::string, PointSpring> foundationNodeDefs;
        std::vector<FoundationNode> foundationNodes;
    
        //Geometry
        std::map<std::string, Story> etabsStories;
        std::vector<std::string> etabsStoriesOrder;
        std::map<unsigned long, Grid> etabsGrids;
    
        //Forces
        std::map<std::string, LoadPattern> LoadPatterns;
        std::map<std::string, LoadCombination> LoadCombos;
    
        //Diaphragm
        std::vector<std::vector<int>> mcc1;
        std::vector<std::vector<int>> mcc2;
    
        //Vectical Connection Elements
        std::vector<std::string> nvconelList;
        std::vector<VerticalConstraint> alphaVCList;
    
        //Damping
        std::vector<SpecialColumn> specialColumns;
        std::vector<int> specialColNum;
        std::vector<int> numColsPerFloor;
        std::vector<float> shearPerFloor;
        float minEle;
        float maxEle;
    
        //Sections
        SteelSection steelSection;
        float sectionConversionMult;
        Slab slabs;
    
        //Output
        float maxFrameWidth = 0.f;
        float frameWidthBuffer = 200.f;

    
        //Config file
        Config config;
    
        //Model Properties
        unsigned int numNodes;
        unsigned int numPrimaryNodes;
        unsigned int numSecondaryNodes;
    
        unsigned int numLineElements;
        unsigned int numPrimCol;
        unsigned int numPrimBeam;
        unsigned int numPrimBrace;

        unsigned int numSecCol;
        unsigned int numSecBeam;
        unsigned int numSecBrace;
    
        unsigned int numPrimLineElements;
        unsigned int numSecLineElements;
    
        Coord::Direction primarySteelDirection;   //Used to model main frames
        Coord::Direction secondarySteelDirection;  //Used to model frames in other direction, dont double dip on steel
    
        Coord::Direction primaryEtabsDirection;
        Coord::Direction secondaryEtabsDirection;
    
        //functions
        Coord::Direction GetEquivSteelDirection(Coord::Direction etabsDirection);   //Retruns equiv Steel direction from Etabs Direction
        Coord::Direction GetEtabsSecondaryDirection(Coord::Direction etabsPrimary);
        Coord::Direction GetSteelSecondaryDirection(Coord::Direction steelPrimary);
    
        void SetICAT();
    
    
        Coord::Direction GetElementEtabsDirection(SteelLine nLine);   //Determines which direction an element is running
        void CheckAddNewNodes(const SteelLine nLine, const Coord::Direction curPrimaryEtabsDirection, const Coord::Direction curSecondaryEtabsDirection, SteelNode newNodes[]); //Check to see if the nodes exist, if they dont add them to the proper node list
    
        void GetElemOrientation(SteelLine &nLine, SteelNode nodes[], const Coord::Direction curPrimaryEtabsDirection); //Determines and sets the proper connectivity for nLine
    
        bool SecondaryElementExists(SteelLine nLine);
    
    

};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, Model& model);
std::istream& operator>>(std::istream& is, Model& model);


#endif /* defined(__Etabs2Steel__Model__) */
