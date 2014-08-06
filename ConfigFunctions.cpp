//
//  ConfigFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/10/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"


void Model::ParseUnits(std::string line)
{
    
    //Find matches
    std::smatch matches;
    d_out dout;
    if (std::regex_search(line, matches, std::regex{config.ParserStringUnits()}))
    {
        //matches[1] = force unit
        //matches[2] = distance unit
        //matches[3] = temperature unit
        //m, ft, in, mm, cm, kg, slugs (lbs^2/ft),
        
        if (matches[2].str() == "IN")
        {
            config.AddSaveConfigVal("AGRAV", std::to_string(386.4));
            sectionConversionMult = 1;
        }
        else if (matches[2].str() == "FT")
        {
            config.AddSaveConfigVal("AGRAV", std::to_string(32.2));
            sectionConversionMult = 1/12;
        }
        else if (matches[2].str() == "M")
        {
            config.AddSaveConfigVal("AGRAV", std::to_string(9.81));
            sectionConversionMult = 0.0254;
        }
        else if (matches[2].str() == "MM")
        {
            config.AddSaveConfigVal("AGRAV", std::to_string(9810));
            sectionConversionMult = 25.4;
        }
        else if (matches[2].str() == "CM")
        {
            config.AddSaveConfigVal("AGRAV", std::to_string(981));
            sectionConversionMult = 2.54;
        }
        else
        {
            dout<<"Unknown Gravity Unit. Terminating Analysis\n";
            exit(0);
        }
    }
    else
        dout<<"Units Parse Error\n";
    
    if (debugLevel >= 2)
        dout << "ETABS Units found to be: "<<matches[2].str()<<" Given AGRAV = "<<*config.GetRawConfigStart("AGRAV")<< " and Section Multiplier = "<<sectionConversionMult<<"\n";

}

void Model::ParseConfig()
{
    //Parse Through Config File
    config.ParseConfig();
    
    //Set up Load and Mass Combination Names
    LoadCombination::LoadComboName = *config.GetRawConfigStart("LOADCOMBO");
    LoadCombination::MassComboName = *config.GetRawConfigStart("MASSCOMBO");
    
    //Set up Savelocation
    saveDirectory = *config.GetRawConfigStart("SAVELOC");
    
    //Set up directions
    primaryEtabsDirection = Coord::FindDirection(*config.GetRawConfigStart("PRIMARYETABSDIR"));
    secondaryEtabsDirection = GetEtabsSecondaryDirection(primaryEtabsDirection);
    primarySteelDirection = GetEquivSteelDirection(primaryEtabsDirection);
    secondarySteelDirection = GetSteelSecondaryDirection(primarySteelDirection);
}

void Model::PrepareConfig()
{
    //Iterate through the config key list
    std::vector<std::string>::iterator keyItr;
    for (keyItr = config.GetConfigListStart(); keyItr != config.GetConfigListEnd(); keyItr++)
    {
        std::string curKey = *keyItr;
        
        //Special Cases
        if (curKey == "ExTH")
            ParseExTH();
        else if (curKey == "ALPHAVC")
            ParseAlphaVC();
        //Values that need to be set from etabs model
        else if (curKey == "NNP")
            config.AddSaveConfigVal("NNP", std::to_string(numNodes));
        else if (curKey == "NEL")
            config.AddSaveConfigVal("NEL", std::to_string(numLineElements));
        
        else if (curKey == "NDS")
            config.AddSaveConfigVal("NDS", "ASNI4");
        
        
        //Information not in config
        else if (curKey == "NRTH")
        {
            unsigned long numTH = config.GetConfigSize("TimeHistory");
            config.AddSaveConfigVal("NRTH", std::to_string(numTH));
        }
        else if (curKey == "DEBUGLEVEL")
        {
            debugLevel = std::stoi(*config.GetRawConfigStart(curKey));
        }
        
        //Values from Config file that are easy to parse
        else
        {
            //iterate through save list
            std::vector<std::string>::iterator configItr;
            for (configItr = config.GetRawConfigStart(curKey); configItr != config.GetRawConfigEnd(curKey); configItr++)
            {
                //std::cout<<*configItr<<std::endl;
                config.AddSaveConfigVal(curKey, *configItr);
            }
        }
    
    }
}

void Model::SetResponseTH()
{
    d_out dout;
    //For every node add displacement time history if enabled
    if (*config.GetRawConfigStart("PlotAll") == "1")
    {
        //Iterate through primary nodes
        std::map<unsigned int, SteelNode>::iterator nodeItr;
        int numTH = 1;
    
        
        for (nodeItr = primarySteelNodes.begin(); nodeItr != primarySteelNodes.end(); nodeItr++)
        {
            std::string curNodeNum = std::to_string((nodeItr->second).Num());
    
            //Horizontal Displacement
            std::stringstream THInput;
            THInput << numTH << " " << curNodeNum << " " << "1 0 0 0 0"<<std::endl;
            //Add to response TH config
            config.AddSaveConfigVal("TimeHistory", THInput.str());
            
            if (debugLevel >= 2)
                dout<<"Adding ExTH: "<<THInput.str()<<"\n";
            numTH++;

            //Vertical Displacement
            THInput.str(std::string());
            THInput << numTH << " " << curNodeNum << " " << "2 0 0 0 0"<<std::endl;
            config.AddSaveConfigVal("TimeHistory", THInput.str());
            
            if (debugLevel >= 2)
                dout<<"Adding ExTH: "<<THInput.str()<<"\n";
            numTH += 1;
        }
    
        //Only plot secondary nodes if toggle is enabled
        if (*config.GetRawConfigStart("PlotSecondary") == "1")
        {
            for (nodeItr = secondarySteelNodes.begin(); nodeItr != secondarySteelNodes.end(); nodeItr++)
            {
                std::string curNodeNum = std::to_string((nodeItr->second).Num());
    
                //Horizontal Displacement
                std::stringstream THInput;
                THInput << numTH << " " << curNodeNum << " " << "1 0 0 0 0" << std::endl;
                config.AddSaveConfigVal("TimeHistory", THInput.str());
                
                if (debugLevel >= 2)
                    dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                numTH += 1;
        
                //Vertical Displacement
                THInput.str(std::string());
                THInput << numTH << " " << curNodeNum << " " << "2 0 0 0 0"<<std::endl;
                config.AddSaveConfigVal("TimeHistory", THInput.str());
                
                if (debugLevel >= 2)
                    dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                numTH += 1;
            }
        }
    }
}

void Model::ParseExTH()
{
    d_out dout;
    std::vector<std::string>::iterator curConfig;
    for (curConfig = config.GetRawConfigStart("ExTH"); curConfig != config.GetRawConfigEnd("ExTH"); curConfig++)
    {
        std::string line = *curConfig;
                
        //Of the form (x1, y1, z1) (x2, y2, z2)? OutputType OutputValue
        
        std::regex exTHR(R"(\(\s*([\w.]+)\s*,\s*([\w.]+)\s*,\s*([\w.]+)\s*\)\s*(\(\s*[\w.]+\s*,\s*[\w.]+\s*,\s*[\w.]+\s*\))?\s*([\w.]+)\s+([\w.]+))");
        std::smatch matches;
                
        if (std::regex_search(line, matches, exTHR)) //Found a Config Option
        {
            //matches[1] = x1 coord
            //matches[2] = y1 coord
            //matches[3] = z1 coord
            //matches[4] = node 2 coordinates or empty
            //matches[5] = OutputType
            //matches[6] = OutputValue
        
            //Store values
            float x1 = std::stof(matches[1].str());
            float y1 = std::stof(matches[2].str());
            float z1 = std::stof(matches[3].str());
            
            int primNum1 = -1;
            int secNum1 = -1;
            int primNum2 = -1;
            int secNum2 = -1;
            
            int boolCheckSec = std::stoi(*config.GetRawConfigStart("PlotSecondary"));
            
            //Find Node Number in Primary
            primNum1 = FindNodeNumber(x1, y1, z1, primaryEtabsDirection);
            
    
            //Find Node Number in Secondary
            secNum1 = FindNodeNumber(x1, y1, z1, secondaryEtabsDirection);
        
            if (primNum1 == -1 && secNum1 == -1)
            {
                dout<<"Error finding node at: ("<<std::to_string(x1)<<", "<<std::to_string(y1)<<", "<<std::to_string(z1)<<")\n";
                continue;
            }
            
            float x2, y2, z2;
            bool twoCoords = false;
            
            unsigned int outputType = std::stoi(matches[5].str());
            unsigned int outputValue = std::stoi(matches[6].str());
    
            
            if (!(matches[4].str()).empty()) //if not empty then parse second set of coordinates
            {
                twoCoords = true;
                std::regex coordR(R"(([\w.]+)\s*,\s*([\w.]+)\s*,\s*([\w.]+))");
                std::smatch coordMatch;
                if (std::regex_search(matches[4].str(), coordMatch, coordR))
                {
                    x2 = std::stof(coordMatch[1].str());
                    y2 = std::stof(coordMatch[2].str());
                    z2 = std::stof(coordMatch[3].str());
                    
                    //Find Node Number in Primary
                    primNum2 = FindNodeNumber(x2, y2, z2, primaryEtabsDirection);
                    
                    //Find Node Number in Secondary
                    secNum2 = FindNodeNumber(x2, y2, z2, secondaryEtabsDirection);
                    
                    if (primNum2 == -1 && secNum2 == -1)
                    {
                        dout<<"Error finding node at: ("<<std::to_string(x2)<<", "<<std::to_string(y2)<<", "<<std::to_string(z2)<<")\n";
                        continue;
                    }
                }
            }
            
            
            
            //Check validity of input - if Output Type = 1 or 2 there should only be 1 set of coordinates
            if (outputType == 1)
            {
                if (twoCoords)
                {
                    dout<<"ExTH Input Error: Should only have 1 set of coordinates: "<<line<<"\n";
                    continue;
                }
                unsigned long curTH = config.GetConfigSize("TimeHistory")+1;

                if (primNum1 != -1)
                {
                    std::stringstream THInput;
                    THInput << std::to_string(curTH) << " " << std::to_string(nodeNumberLookup[primNum1]) << " " << outputValue << " 0 0 0 0" << std::endl;
                    config.AddSaveConfigVal("TimeHistory", THInput.str());
                    
                    if (debugLevel >= 2)
                        dout<<"Adding ExTH: "<<THInput.str()<<"\n";

                    curTH += 1;
                }
                if (secNum1 != -1 && boolCheckSec)
                {
                    std::stringstream THInput;
                    THInput << std::to_string(curTH) << " " << std::to_string(nodeNumberLookup[secNum1]) << " " << outputValue << " 0 0 0 0" << std::endl;
                    config.AddSaveConfigVal("TimeHistory", THInput.str());
                    
                    if (debugLevel >= 2)
                        dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                }
            }
            if (outputType == 2)
            {
                if (twoCoords)
                {
                    dout<<"ExTH Input Error: Should only have 1 set of coordinates: "<<line<<"\n";
                    continue;
                }
                unsigned long curTH = config.GetConfigSize("TimeHistory")+1;

                if (primNum1 != -1)
                {
                    std::stringstream THInput;
                    THInput << std::to_string(curTH) << " 0 0 " << std::to_string(nodeNumberLookup[primNum1]) << " " << outputValue << " 0 0" << std::endl;
                    config.AddSaveConfigVal("TimeHistory", THInput.str());
                    
                    if (debugLevel >= 2)
                        dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                    curTH += 1;
                }
                if (secNum1 != -1 && boolCheckSec)
                {
                    std::stringstream THInput;
                    THInput << std::to_string(curTH) << " 0 0 " << std::to_string(nodeNumberLookup[secNum1]) << " " << outputValue << " 0 0" << std::endl;
                    config.AddSaveConfigVal("TimeHistory", THInput.str());
                    if (debugLevel >= 2)
                        dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                }
            }
            if (outputType == 3)
            {
                if (!twoCoords)
                {
                    dout<<"ExTH Input Error: Should have 2 sets of coordinates: "<<line<<"\n";
                    continue;
                }
                unsigned long curTH = config.GetConfigSize("TimeHistory")+1;
                
                //Find Element Number
                int primEleNum = FindElementNumber(primNum1, primNum2, primaryEtabsDirection);
                int secEleNum = FindElementNumber(secNum1, secNum2, secondaryEtabsDirection);
                
                if (primEleNum == -1 && secEleNum == -1)
                {
                    dout<<"Error finding Element at: ("<<std::to_string(x1)<<", "<<std::to_string(y1)<<", "<<std::to_string(z1)<<") "<<"("<<std::to_string(x2)<<", "<<std::to_string(y2)<<", "<<std::to_string(z2)<<")\n";
                    continue;
                }
                
                if (primEleNum != -1)
                {
                    std::stringstream THInput;
                    THInput << std::to_string(curTH) << " 0 0 0 0 " << primEleNum << " " << std::to_string(outputValue) << std::endl;
                    config.AddSaveConfigVal("TimeHistory", THInput.str());
                    if (debugLevel >= 2)
                        dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                    curTH += 1;
                }
                if (secEleNum != -1 && boolCheckSec)
                {
                    std::stringstream THInput;
                    THInput << std::to_string(curTH) << " 0 0 0 0 " << secEleNum << " " << std::to_string(outputValue) << std::endl;
                    config.AddSaveConfigVal("TimeHistory", THInput.str());
                    if (debugLevel >= 2)
                        dout<<"Adding ExTH: "<<THInput.str()<<"\n";
                }
            }
        }
    }
}

int Model::FindNodeNumber(float x, float y, float z, Coord::Direction searchDir)
{
    //Set proper list
    std::map<unsigned int, SteelNode> searchList;
    if (searchDir == primaryEtabsDirection)
        searchList = primarySteelNodes;
    else
        searchList = secondarySteelNodes;
    
    //Loop through list
    std::map<unsigned int, SteelNode>::iterator nodeItr;
    for (nodeItr = searchList.begin(); nodeItr != searchList.end(); nodeItr++)
    {
        //float curX = (nodeItr->second).Coords(Coord::Direction::X);
        //float curY = (nodeItr->second).Coords(Coord::Direction::Y);
        //float curZ = (nodeItr->second).Coords(Coord::Direction::Z);
        
        
        float curX = etabsNodes[(nodeItr->second).EtabsNodeNum()].Coords(Coord::Direction::X);
        float curY = etabsNodes[(nodeItr->second).EtabsNodeNum()].Coords(Coord::Direction::Y);
        float curZ = (nodeItr->second).Coords(Coord::Direction::Z);
        
        if (isEqual(x,curX) && isEqual(y,curY) && isEqual(z,curZ))
        {
            unsigned int id = 0;
            id = (nodeItr->second).Num();
            return id;
        }
    }
    return -1;
}

int Model::FindElementNumber(int node1, int node2, Coord::Direction searchDir)
{
    //Set proper list
    std::map<unsigned int, SteelLine> searchList;
    if (searchDir == primaryEtabsDirection)
        searchList = primarySteelLines;
    else
        searchList = secondarySteelLines;
    
    //Loop through list
    std::map<unsigned int, SteelLine>::iterator eleItr;
    for (eleItr = searchList.begin(); eleItr != searchList.end(); eleItr++)
    {
        std::vector<unsigned int> curNodes = (eleItr->second).NodeConnectivity();
        if ( (curNodes[0] == node1 && curNodes[1] == node2) || (curNodes[1] == node1 && curNodes[0] == node2) )
            return (eleItr->second).SteelID();
    }
    return -1;
}
