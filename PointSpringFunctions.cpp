//
//  PointSpringFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/2/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"

void Model::newFndNodeDef(PointSpring nSpring)
{
    d_out dout;
    //Parse Config to get Default Foundation Node Properties
    float defALP;
    float defSTRH;
    float defSTRVU;
    float defSTRVD;
    
    //Check to make sure it was define
    if (config.GetRawConfigSize("DefFndNode") == 0)
    {
        dout<<"No Default Foundation Node Properties Define. Terminating Analysis\n";
        std::exit(0);
    }
    //Save Properties
    std::string defLine = *config.GetRawConfigStart("DefFndNode");
    std::stringstream sLine(defLine);
    sLine >> defALP >> defSTRH >> defSTRVU >> defSTRVD;
    
    //Look through the config list to see if this has specific overide Properties
    std::vector<std::string>::iterator springItr;
    for (springItr = config.GetRawConfigStart("FndNode"); springItr != config.GetRawConfigEnd("FndNode"); springItr++)
    {
        //Convert to SStream
        std::stringstream tmpLine(*springItr);
        
        std::string name;
        float ALP;
        float STRH;
        float STRVU;
        float STRVD;
        
        tmpLine >> name >> ALP >> STRH >> STRVU >> STRVD;
        
        if (name == nSpring.Name())
        {
            nSpring.ALP(ALP);
            nSpring.STRH(STRH);
            nSpring.STRVU(STRVU);
            nSpring.STRVD(STRVD);
            
            if (debugLevel >= 1)
        dout<<"Creating foundation spring definition. ALP = "<<ALP<<" STRH = "<<STRH<<" STRVU = "<<STRVU<<" STRVD = "<<STRVD<<"\n";
            
            //Push Onto List
            foundationNodeDefs[nSpring.Name()] = nSpring;
            
            return;
        }
    }
    
    //If no matches were found, assign default
    nSpring.ALP(defALP);
    nSpring.STRH(defSTRH);
    nSpring.STRVU(defSTRVU);
    nSpring.STRVD(defSTRVD);
    
    //Push Onto List
    foundationNodeDefs[nSpring.Name()] = nSpring;
    
    if (debugLevel >= 1)
        dout<<"Creating foundation spring definition. ALP = "<<defALP<<" STRH = "<<defSTRH<<" STRVU = "<<defSTRVU<<" STRVD = "<<defSTRVD<<"\n";
}

void Model::NewFndNode(FoundationNode fndNode)
{
    //Search through foundationNodeDefs to get proper definition
    std::map<std::string, PointSpring>::iterator springItr;
    
    for (springItr = foundationNodeDefs.begin(); springItr != foundationNodeDefs.end(); springItr++)
    {
        if ((springItr->second).Name() == fndNode.Name()) //Match
        {
            //Copy Data
            fndNode.Stiffness(Coord::Direction::X, (springItr->second).Stiffness(Coord::Direction::X));
            fndNode.Stiffness(Coord::Direction::Y, (springItr->second).Stiffness(Coord::Direction::Y));
            fndNode.Stiffness(Coord::Direction::Z, (springItr->second).Stiffness(Coord::Direction::Z));
            
            fndNode.ALP((springItr->second).ALP());
            fndNode.STRH((springItr->second).STRH());
            fndNode.STRVU((springItr->second).STRVU());
            fndNode.STRVD((springItr->second).STRVD());
            
            //Push Onto List
            foundationNodes.push_back(fndNode);
            
            continue;
        }
    
    }
}

void Model::AddFoundationNodes()
{
    //Go through Foundation Node List
    std::vector<FoundationNode>::iterator fndNodeItr;
    for (fndNodeItr = foundationNodes.begin(); fndNodeItr != foundationNodes.end(); fndNodeItr++)
    {
        unsigned int curEtabsRef = (*fndNodeItr).EtabsRefName();
        std::string curFloorName = (*fndNodeItr).FloorName();
        float curFloorEle = etabsStories[curFloorName].Elevation();
        
        //Loop through Primary Steel Nodes
        std::map<unsigned int, SteelNode>::iterator nodeItr;
        for (nodeItr = primarySteelNodes.begin(); nodeItr != primarySteelNodes.end(); nodeItr++)
        {
            unsigned int checkEtabsRef = (nodeItr->second).EtabsNodeNum();
            float checkFloorEle = (nodeItr->second).Coords(Coord::Direction::Z);
        
            //Check for match
            if (curEtabsRef == checkEtabsRef && isEqual(curFloorEle,checkFloorEle)) //Match
            {
                (*fndNodeItr).Num((nodeItr->second).Num());
                
                //Copy Z to Y
                (*fndNodeItr).Stiffness(Coord::Direction::Y, (*fndNodeItr).Stiffness(Coord::Direction::Z));
                break;
            }
        }
        
        //Loop through Secondary Steel Nodes
        for (nodeItr = secondarySteelNodes.begin(); nodeItr != secondarySteelNodes.end(); nodeItr++)
        {
            unsigned int checkEtabsRef = (nodeItr->second).EtabsNodeNum();
            float checkFloorEle = (nodeItr->second).Coords(Coord::Direction::Z);
        
            //Check for match
            if (curEtabsRef == checkEtabsRef && isEqual(curFloorEle,checkFloorEle)) //Match
            {
                //Create a new Foundation Node
                FoundationNode tmpNode = *fndNodeItr;
                
            
                tmpNode.Num((nodeItr->second).Num());
                
                //Put the secondary direciton into X and Copy Z to Y
                tmpNode.Stiffness(Coord::Direction::X, tmpNode.Stiffness(secondaryEtabsDirection));
                (*fndNodeItr).Stiffness(Coord::Direction::Y, (*fndNodeItr).Stiffness(Coord::Direction::Z));
                //Push onto back of list
                foundationNodes.push_back(tmpNode);
                break;
            }
        }
    }
    //Set NNPFN
    config.AddRawConfigVal("NNPFN", std::to_string(foundationNodes.size()));
    config.AddSaveConfigVal("NNPFN", std::to_string(foundationNodes.size()));
    
}

std::ostringstream Model::OutputFoundationNodes()
{
    std::ostringstream outString;
    //Iterate through all Foundation Nodes
    
    std::vector<FoundationNode>::iterator fndNodeItr;
    
    for (fndNodeItr = foundationNodes.begin(); fndNodeItr != foundationNodes.end(); fndNodeItr++)
        outString << (*fndNodeItr);
    
    return outString;
}