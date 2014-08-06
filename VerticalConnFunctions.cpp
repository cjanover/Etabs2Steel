//
//  VerticalConnFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/21/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"

void Model::CalculateNVCONEL()
{
    d_out dout;
    //Loop through primary nodes and seach for matching coordinates in secondary nodes
    
    std::map<unsigned int, SteelNode>::iterator primItr;
    
    int nvCount = 1;
    for (primItr = primarySteelNodes.begin(); primItr != primarySteelNodes.end(); primItr++)
    {
        //Get coordinates of current node
        float curX, curY, curZ;
        
        curX = (primItr->second).Coords(Coord::Direction::X);
        curY = (primItr->second).Coords(Coord::Direction::Y);
        curZ = (primItr->second).Coords(Coord::Direction::Z);
        
        //Loop throguh secondary nodes
        std::map<unsigned int, SteelNode>::iterator secItr;
        for (secItr = secondarySteelNodes.begin(); secItr != secondarySteelNodes.end(); secItr++)
        {
            float checkX, checkY, checkZ;
            
            checkX = (secItr->second).Coords(Coord::Direction::X);
            checkY = (secItr->second).Coords(Coord::Direction::Y);
            checkZ = (secItr->second).Coords(Coord::Direction::Z);
            
            //Check for match
            if (isEqual(curX,checkX) && isEqual(curY,checkY) && isEqual(curZ,checkZ))
            {
            
                //Check to make sure the vertical degree of freedom of either node isnt fixed
                const int* primRestraint = (primItr->second).NodalFixity();
                const int* secRestraint = (primItr->second).NodalFixity();
                
                
                if (primRestraint[2] == 1 && secRestraint[2] == 1)
                {
                
                    //Get Node Numbers
                    unsigned int curNodeNum = (primItr->second).Num();
                    unsigned int checkNodeNum = (secItr->second).Num();
                
                    //Get actual node Numbers
                    unsigned int curNodeNumAct = nodeNumberLookup[curNodeNum];
                    unsigned int checkNodeNumAct = nodeNumberLookup[checkNodeNum];
                
                    //Create line
                    std::ostringstream nvconel;
                    nvconel << nvCount << " " << SetAlphaVC((primItr->second).getCoord()) << " " << curNodeNumAct << " " << checkNodeNumAct << std::endl;
                    
                    if (debugLevel >=1)
                        dout<<"Creating vertical connection between nodes "<<curNodeNumAct<<" and "<<checkNodeNumAct<<"\n";
                    //push onto list
                    nvconelList.push_back(nvconel.str());
                
                    //Increment Counter
                    nvCount++;
                
                    
                }
                break;
            }
            
        }
    }
    //Set NVCONEL in configs
    config.AddSaveConfigVal("NVCONEL", std::to_string(nvconelList.size()));
    config.AddRawConfigVal("NVCONEL", std::to_string(nvconelList.size()));
}


void Model::ParseAlphaVC()
{
    //Loop through the raw config
    std::vector<std::string>::iterator configItr;
    
    for (configItr = config.GetRawConfigStart("ALPHAVC"); configItr != config.GetRawConfigEnd("ALPHAVC"); configItr++)
    {
        //Parse the line in the format of (x, y, z) alphavc
        
        std::string line = *configItr;
        std::regex alphaVCR(R"(\s*\(\s*([\w.]+)\s*,\s*([\w.]+)\s*,\s*([\w.]+)\s*\)\s+([\w.]+))");

        std::smatch matches;

        if (std::regex_search(line, matches, alphaVCR)) //Found a Config Option
        {
            //Create the vertical Constraint
            VerticalConstraint tmpConstr(std::stof(matches[1]), std::stof(matches[2]), std::stof(matches[3]), std::stof(matches[4]));
            
            //Push onto list
            alphaVCList.push_back(tmpConstr);
        }

    }

}

float Model::SetAlphaVC(Coord searchCoords)
{
    d_out dout;
    //Get Search values
    float searchX = searchCoords.Coords(Coord::Direction::X);
    float searchY = searchCoords.Coords(Coord::Direction::Y);
    float searchZ = searchCoords.Coords(Coord::Direction::Z);

    //Loop through AlphaVC List to try to find a coordinate match
    std::vector<VerticalConstraint>::iterator constrItr;
    
    for (constrItr = alphaVCList.begin(); constrItr != alphaVCList.end(); constrItr++)
    {
        //Get current Coords
        Coord curCoords = (*constrItr).Coord();
        float curX = curCoords.Coords(Coord::Direction::X);
        float curY = curCoords.Coords(Coord::Direction::Y);
        float curZ = curCoords.Coords(Coord::Direction::Z);

        //Check to see if coordinates match
        if (isEqual(searchX,curX) && isEqual(searchY,curY) && isEqual(searchZ,curZ))
        {
            //return alphaVC Value
            if (debugLevel >= 2)
                dout<<"AlphaVC for coordinates ("<<searchX<<", "<<searchY<<", "<<searchZ<<") = "<<(*constrItr).AlphaVC()<<"\n";
            return (*constrItr).AlphaVC();
        }
    }
    
    //If no match found returnd default value
    return std::stof(*config.GetRawConfigStart("ALPHAVCDEF"));
}
