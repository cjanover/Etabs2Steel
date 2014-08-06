//
//  RestraintFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/24/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"


void Model::SetSteelNodeFixity()
{
    d_out dout;
    //Loop through all steel nodes
    std::map<unsigned int, SteelNode>::iterator iter;
    for (iter = primarySteelNodes.begin(); iter != primarySteelNodes.end(); iter++)
    {
        float elev = ((iter->second).getCoord()).Coords(Coord::Direction::Z);
        std::string StoryName = FindStoryName(elev);
        //Look up nodal Restraints
        
        if ( restrainedNodes.count(std::make_pair( (iter->second).EtabsNodeNum(), StoryName )) != 0 )
        {
            const NodalRestraint* tmpRestraint = restrainedNodes[std::make_pair((iter->second).EtabsNodeNum(), StoryName)].GetFixity();
            (iter->second).NodalFixity(*tmpRestraint);
        }
        else
        {
            NodalRestraint tmpRestraint;
            tmpRestraint.NodeFixity(1,1,1,2);
            (iter->second).NodalFixity(tmpRestraint);
        }
        
        
    }
    
    for (iter = secondarySteelNodes.begin(); iter != secondarySteelNodes.end(); iter++)
    {
        float elev = ((iter->second).getCoord()).Coords(Coord::Direction::Z);
        std::string StoryName = FindStoryName(elev);
        //Look up nodal Restraints
        NodalRestraint* tmpRestraint = restrainedNodes[std::make_pair((iter->second).EtabsNodeNum(), StoryName)].GetFixity();
        int* restraint = (*tmpRestraint).NodalFixity();
        
        if (!restraint[0] && !restraint[1] && !restraint[2])
            restraint[3] = false;
        
        (*tmpRestraint).NodeFixity(restraint[0], restraint[1], restraint[2], restraint[3]);
        (iter->second).NodalFixity(*tmpRestraint);
        
        if (debugLevel >= 1)
            dout<<"Restraints for Secondary Node "<<(iter->second).Num()<<" = "<<restraint[0]<<" "<<restraint[1]<<" "<<restraint[2]<<" "<<restraint[3]<<"\n";
    }

}


void Model::AddRollerSecondary()
{
    d_out dout;
    //Loop through each floor
    std::map<std::string, Story>::iterator storyItr;
    
    for (storyItr = etabsStories.begin(); storyItr != etabsStories.end(); storyItr++)
    {
        //Save current elevation
        float curEle = (storyItr->second).Elevation();
    
        if (isEqual(curEle,minEle))   //Stop if minimum elevation
            continue;
    
        //go through each secondary grid
        std::map<unsigned long, Grid>::iterator gridItr;
        
        for (gridItr = etabsGrids.begin(); gridItr != etabsGrids.end(); gridItr++)
        {
            float minCoord = 0;
            int minNodeNum = 0;
        
            //Only check grid if the line runs in the secondary direction, meaning the x coordinate is fixed
            if ((gridItr->second).EtabsGridDirection() == primaryEtabsDirection)
            {
                //Save current Grid Coordinate
                float curCoord = (gridItr->second).Coordinate();

                //Now search through secondary node list for nodes on the correct elevation and grid location,
                std::map<unsigned int, SteelNode>::iterator nodeItr;
                
                for (nodeItr = secondarySteelNodes.begin(); nodeItr != secondarySteelNodes.end(); nodeItr++)
                {
                    float checkEle = (nodeItr->second).Coords(Coord::Direction::Z);
                    float checkCoord = (nodeItr->second).Coords(primaryEtabsDirection);
                    
                    if (isEqual(checkEle,curEle) && isEqual(checkCoord,curCoord))
                    {
                        float compareCoord = (nodeItr->second).Coords(secondaryEtabsDirection);
                    
                        //Check to see if checkCoord is less than the current mind Coord
                        if (compareCoord < minCoord || minNodeNum == 0)
                        {
                            minCoord = compareCoord;
                            minNodeNum = (nodeItr->second).Num();
                            
                        }
                    }
                }
            }
            //Only adjust fixity if a node has been found
            if (minNodeNum != 0)
            {
                //Get the Curernt nodal Releases
                const int* curFixity = secondarySteelNodes[minNodeNum].NodalFixity();
            
                int freeX = curFixity[0];
                int freeY = curFixity[1];
                int freeZ = curFixity[2];
                int freeRZ = curFixity[3];
            
                //Make the appropriate direction fixed
                if (primaryEtabsDirection == Coord::Direction::X)
                    freeX = false;
                else
                    freeY = false;
            
            
               // std::cout<<"Adding Secondary Restraint at: ("<<secondarySteelNodes[minNodeNum].Coords(Coord::Direction::X)<<", "<<secondarySteelNodes[minNodeNum].Coords(Coord::Direction::Y)<<", "<<secondarySteelNodes[minNodeNum].Coords(Coord::Direction::Z)<<")"<<std::endl;
            
                //Create restraint
                NodalRestraint tmpRes;
                tmpRes.NodeFixity(freeX, freeY, freeZ, freeRZ);
            
                //Since im done checking all nodes on this grid and elevation, set the minimum nodes restraint to be fixed horizontally
                secondarySteelNodes[minNodeNum].NodalFixity(tmpRes);
                
                if (debugLevel >= 1)
                    dout<<"Adding vertical roller to secondary node "<<minNodeNum<<"\n";
            }
        }
    }
}
