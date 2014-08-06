//
//  Diaphragm.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/21/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"

void Model::CalculateDiaphragm()
{
    //Calculate number of nodes per floor
    Model::CalculateNumNodesPerFloor();

    //Create PrimaryGridList and SecondaryGridList
    std::map<std::string, Grid> gridRunPrim;
    std::map<std::string, Grid> gridRunSec;
    
    //Loop through grids
    std::map<unsigned long, Grid>::iterator gridSort;
    for (gridSort = etabsGrids.begin(); gridSort != etabsGrids.end(); gridSort++)
    {
        if ((gridSort->second).EtabsGridDirection() == secondaryEtabsDirection)
            gridRunPrim[(gridSort->second).GridName()] = (gridSort->second);
        else
            gridRunSec[(gridSort->second).GridName()] = (gridSort->second);
    }


    std::vector<int> prevList;
    std::vector<int> curList;
    
    //Loop through each floor
    std::map<std::string, Story>::iterator storyItr;
    for (storyItr = etabsStories.begin(); storyItr != etabsStories.end(); storyItr++)
    {
        //Clear the Lists
        prevList.clear();
        curList.clear();
    
        float curElevation = (storyItr->second).Elevation();
        
        //Only check if CurElevation isnt the minimum elevation
        
        if (isEqual(curElevation,minEle))
            continue;
    
        //Loop through grids running in primary direction
        std::map<std::string, Grid>::iterator primGridItr;
        for (primGridItr = gridRunPrim.begin(); primGridItr != gridRunPrim.end(); primGridItr++)
        {
            float curCoord = (primGridItr->second).Coordinate();
            
            //Now search through primary node list for a matching node
            std::map<unsigned int, SteelNode>::iterator nodeItr;
            
            for (nodeItr = primarySteelNodes.begin(); nodeItr != primarySteelNodes.end(); nodeItr++)
            {
                float checkCoord = (nodeItr->second).Coords(secondaryEtabsDirection);
                float checkSecCoord = (nodeItr->second).Coords(primaryEtabsDirection);
                float checkEle = (nodeItr->second).Coords(Coord::Direction::Z);
            
                //Check to see if the current node is on the correct floor and the correct grid
                    
                if (isEqual(checkEle,curElevation) && isEqual(checkCoord,curCoord))
                {
                    //Make sure the node lands on grid in the other direction
                    std::map<std::string, Grid>::iterator secGridItr;
                    for (secGridItr = gridRunSec.begin(); secGridItr != gridRunSec.end(); secGridItr++)
                    {
                        float curSecCoord = (secGridItr->second).Coordinate();
                        if (isEqual(curSecCoord,checkSecCoord))
                        {
                            //Get Node Number
                            int checkNum = (nodeItr->second).Num();
                    
                            //Add the node to the list currentList
                            curList.push_back(checkNum);
                        
                        }
                    }
                }
            }
        
            //Now that im done looking through the grids, check to see if my prevList isnt empty
            if (prevList.size() != 0)
            {
                //Also check that curList has the proper number of nodes
                if (curList.size() == std::stoi(*config.GetConfigStart("NNPBF")))
                {
                    //Since it isnt empty push onto mcc1 and mcc2
                    mcc1.push_back(prevList);
                    mcc2.push_back(curList);
                
                    //Now set prevList to Cur List and start on the next floor
                    prevList.clear();
                    prevList = curList;
                }
                else
                {
                    //Set prevList to an empty list
                    prevList.clear();
                }
            }
            else
                prevList = curList;
            
            //Empty Current List
            curList.clear();
        }
    }
    
    //Update NCONEL
    config.AddRawConfigVal("NCONEL", std::to_string(mcc1.size()));
    config.AddSaveConfigVal("NCONEL", std::to_string(mcc1.size()));

}

void Model::CalculateNumNodesPerFloor()
{
    d_out dout;
    //Create PrimaryGridList and SecondaryGridList
    std::map<std::string, Grid> gridRunPrim;
    std::map<std::string, Grid> gridRunSec;
    
    int maxNodes = 0;
    
    //Loop through grids
    std::map<unsigned long, Grid>::iterator gridSort;
    for (gridSort = etabsGrids.begin(); gridSort != etabsGrids.end(); gridSort++)
    {
        if ((gridSort->second).EtabsGridDirection() == secondaryEtabsDirection)
            gridRunPrim[(gridSort->second).GridName()] = (gridSort->second);
        else
            gridRunSec[(gridSort->second).GridName()] = (gridSort->second);
    }

//Loop through each floor
    std::map<std::string, Story>::iterator storyItr;
    for (storyItr = etabsStories.begin(); storyItr != etabsStories.end(); storyItr++)
    {
        float curElevation = (storyItr->second).Elevation();
    
        //Loop through grids running in primary direction
        std::map<std::string, Grid>::iterator primGridItr;
        for (primGridItr = gridRunPrim.begin(); primGridItr != gridRunPrim.end(); primGridItr++)
        {
            int curNNPBF = 0;
        
            float curCoord = (primGridItr->second).Coordinate();
            
            //Now search through primary node list for a matching node
            std::map<unsigned int, SteelNode>::iterator nodeItr;
            
            for (nodeItr = primarySteelNodes.begin(); nodeItr != primarySteelNodes.end(); nodeItr++)
            {
                float checkCoord = (nodeItr->second).Coords(secondaryEtabsDirection);
                float checkSecCoord = (nodeItr->second).Coords(primaryEtabsDirection);
                float checkEle = (nodeItr->second).Coords(Coord::Direction::Z);
            
                //Check to see if the current node is on the correct floor and the correct grid
                    
                if (isEqual(checkEle,curElevation) && isEqual(checkCoord,curCoord))
                {
                    //Make sure the node lands on grid in the other direction
                    std::map<std::string, Grid>::iterator secGridItr;
                    for (secGridItr = gridRunSec.begin(); secGridItr != gridRunSec.end(); secGridItr++)
                    {
                        float curSecCoord = (secGridItr->second).Coordinate();
                        if (isEqual(curSecCoord,checkSecCoord))
                        {
                            //Increment counter
                            curNNPBF++;
                        
                        }
                    }
                }
            }
            //Since im done checking this grid, check to see if curNNPBF is higher than maxNodes
            if (curNNPBF > maxNodes)
                maxNodes = curNNPBF;
        }
    }
    if (debugLevel >=2)
        dout<<"Maximum number of nodes per floor (NNPBF) = "<<maxNodes<<"\n";
    //update NNPFN
    config.AddSaveConfigVal("NNPBF", std::to_string(maxNodes));
    config.AddRawConfigVal("NNPBF", std::to_string(maxNodes));

}

