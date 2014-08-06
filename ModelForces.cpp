//
//  ModelForces.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"


void Model::newLoadPattern(LoadPattern pattern)
{
    //Add to list
    LoadPatterns[pattern.PatternName()] = pattern;
}


void Model::newStaticLoad(StaticLoad load)
{
    //Search for matching nodes in the primary and secondary direction, if a primary direction node add force in primary direction
    //else add in secondary direction

    bool matchFound = false;
    
    std::map<unsigned int, SteelNode>::iterator nodeIter;
    
    for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
    {
        unsigned int curEtabsNodeNum = (nodeIter->second).EtabsNodeNum();
        std::string curEtabsFloorName = FindStoryName((nodeIter->second).Coords(Coord::Direction::Z));
        
        //Check for match in etabs node number and etabs floor
        if (curEtabsNodeNum == load.EtabsNodeNum() && curEtabsFloorName == load.EtabsFloorName())
        {
            //Add to list
            float force[3] = {0.f, 0.f, 0.f};
            force[primaryEtabsDirection] = load.Force(primaryEtabsDirection);
            force[2] = load.Force(Coord::Direction::Z);
            
            (nodeIter->second).Force(force, load.PatternName());
            
            matchFound = true;
        }
    
    }
    
    for (nodeIter = secondarySteelNodes.begin(); nodeIter != secondarySteelNodes.end(); nodeIter++)
    {
        unsigned int curEtabsNodeNum = (nodeIter->second).EtabsNodeNum();
        std::string curEtabsFloorName = FindStoryName((nodeIter->second).Coords(Coord::Direction::Z));
        
        //Check for match in etabs node number and etabs floor
        if (curEtabsNodeNum == load.EtabsNodeNum() && curEtabsFloorName == load.EtabsFloorName())
        {
            float force[3] = {0.f, 0.f, 0.f};

            force[primaryEtabsDirection] = load.Force(primaryEtabsDirection);
            
            if (!matchFound) //Also copy vertical force
                force[2] = load.Force(Coord::Direction::Z);
            
            (nodeIter->second).Force(force, load.PatternName());
        }
    
    }

}

void Model::newLoadCombo(LoadCombination combo)
{
    //Check to see if combo already exists in LoadCombos, if it does, merge the lists, if not make new one
    std::map<std::string, LoadCombination>::iterator comboIter;
    for (comboIter = LoadCombos.begin(); comboIter != LoadCombos.end(); comboIter++)
    {
        if ((comboIter->second).Name() == combo.Name()) //Merge
        {
            std::string newCombo = combo.GetFirstName();
            (comboIter->second).LoadSF(newCombo, combo.LoadSF(newCombo));
            return;
        }
    }
    
    //Only name new item if Name isnt empty
    if (combo.Name() != "")
        LoadCombos[combo.Name()] = combo;
}


void Model::CalculateNodalForces()
{
    //For Load Combo name loop through all patterns
    LoadCombination loadCombo = LoadCombos[LoadCombination::LoadComboName];
    
    std::map<std::string, float>::iterator sfIter;
    
    for (sfIter = loadCombo.GetStartIterator(); sfIter != loadCombo.GetEndIterator(); sfIter++)
    {
        std::string curPatternName = (sfIter->first);
        float curPatternSF = (sfIter->second);
        
        //Now loop through all primary nodes
        std::map<unsigned int, SteelNode>::iterator nodeIter;
        for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
        {
            //Check to see if the load pattern exists
            if ((nodeIter->second).PatternExists(curPatternName))
            {
                float force[3] = {0, 0, 0};
                force[0] = curPatternSF*(nodeIter->second).Force(curPatternName, Coord::Direction::X);
                force[1] = curPatternSF*(nodeIter->second).Force(curPatternName, Coord::Direction::Y);
                force[2] = curPatternSF*(nodeIter->second).Force(curPatternName, Coord::Direction::Z);
            
                //Add to current nodes total force
                (nodeIter->second).IncTotForce(force);
            }
        }
        
        //Now Loop through all secondary nodes
        for (nodeIter = secondarySteelNodes.begin(); nodeIter != secondarySteelNodes.end(); nodeIter++)
        {
            bool foundInPrimary = SecondaryNodeInPrimary(nodeIter->second);
        
            //Check to see if the load pattern exists
            if ((nodeIter->second).PatternExists(curPatternName))
            {
                float force[3];
                force[0] = (nodeIter->second).Force(curPatternName, Coord::Direction::X);
                force[1] = (nodeIter->second).Force(curPatternName, Coord::Direction::Y);
                force[2] = 0.f;
                
                if (!foundInPrimary)    //Only do vertical force if the secondary node isnt a duplicate
                    force[2] = (nodeIter->second).Force(curPatternName, Coord::Direction::Z);
            
                //Add to current nodes total force
                (nodeIter->second).IncTotForce(force);
            }
        
        }
    }
    

}


void Model::CalculateNodalMass()
{
    //For Mass Combo name, loop through all patterns
    LoadCombination massCombo = LoadCombos[LoadCombination::MassComboName];
    
    std::map<std::string, float>::iterator sfIter;
    
    for (sfIter = massCombo.GetStartIterator(); sfIter != massCombo.GetEndIterator(); sfIter++)
    {
        std::string curPatternName = (sfIter->first);
        float curPatternSF = (sfIter->second);
        
        

        
        //Now loop through all primary nodes
        std::map<unsigned int, SteelNode>::iterator nodeIter;
        for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
        {
            //Check to see if the load pattern exists
            if ((nodeIter->second).PatternExists(curPatternName))
            {
                float mass[3];
                mass[0] = std::abs(curPatternSF*(nodeIter->second).Force(curPatternName, Coord::Direction::X));
                mass[1] = std::abs(curPatternSF*(nodeIter->second).Force(curPatternName, Coord::Direction::Y));
                mass[2] = std::abs(curPatternSF*(nodeIter->second).Force(curPatternName, Coord::Direction::Z));
            
                //Add to current nodes total force
                (nodeIter->second).IncTotMass(mass);
            }
        }
        
        //Now Loop through all secondary nodes
        for (nodeIter = secondarySteelNodes.begin(); nodeIter != secondarySteelNodes.end(); nodeIter++)
        {
            //bool foundInPrimary = SecondaryNodeInPrimary(nodeIter->second);
            
            //Check to see if the load pattern exists
            if ((nodeIter->second).PatternExists(curPatternName))
            {
                float mass[3]; //Always make seondary frames have zero mass
                mass[0] = 0.f;
                mass[1] = 0.f;
                mass[2] = 0.f;
                //mass[0] = (nodeIter->second).Force(curPatternName, Coord::Direction::X);
                //mass[1] = (nodeIter->second).Force(curPatternName, Coord::Direction::Y);
                //mass[2] = 0.f;
                
                //if (!foundInPrimary)    //Only do vertical mass if the secondary node isnt a duplicate
                //    mass[2] = (nodeIter->second).Force(curPatternName, Coord::Direction::Z);
            
                //Add to current nodes total force
                (nodeIter->second).IncTotMass(mass);
            }
        
        }
    }
}

bool Model::SecondaryNodeInPrimary(SteelNode node)
{
    float secCoords[3];
    secCoords[0] = node.Coords(Coord::Direction::X);
    secCoords[1] = node.Coords(Coord::Direction::Y);
    secCoords[2] = node.Coords(Coord::Direction::Z);
    
    //Iterate through primary Node List
    std::map<unsigned int, SteelNode>::iterator nodeIter;
    
    for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
    {
        float curCoords[3];
        curCoords[0] = (nodeIter->second).Coords(Coord::Direction::X);
        curCoords[1] = (nodeIter->second).Coords(Coord::Direction::Y);
        curCoords[2] = (nodeIter->second).Coords(Coord::Direction::Z);
    
        //check if coordinates match
        if (curCoords[0] == secCoords[0] && curCoords[1] == secCoords[1] && curCoords[2] == secCoords[2])
            return true;
    }
    
    return false;

}

