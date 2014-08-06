//
//  LoadCombination.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "LoadCombination.h"

std::string LoadCombination::MassComboName = "";
std::string LoadCombination::LoadComboName = "";

bool LoadCombination::CaseExists(std::string caseName)
{
    //iterate through scale factors
    std::map<std::string, float>::iterator loadIter;
    
    for (loadIter = loadSF.begin(); loadIter != loadSF.end(); loadIter++)
    {
        if ((loadIter->first) == caseName)
            return true;
    }
    return false;
}

//Operator Overloads
std::istream& operator>>(std::istream& is, LoadCombination& combo)
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    //std::cout<<line<<std::endl;
    if (std::regex_search(line, matches, std::regex{combo.ParserString()}))
    {
        //[1] = Combo Name
        //[2] = if Type then
            //[3] = Add Type (ignore)
        //[2] = if LOADCASE
            //[3] = Name of Load Pattern
            //[4] = SF (just text)
            //[5] = ScaleFactor Value
        //[2] = if LOADCOMBO
            //[3] = Name of Load combination
            //[4] = SF (just text)
            //[5] = ScaleFactor Value
        
        if (matches[2].str() != "TYPE" && matches[4].str() == "SF")
        {
            //Save Data
            combo.Name(matches[1].str());
            //std::cout<<line;

            combo.LoadSF(matches[3].str(), std::stod(matches[5].str()));
            
            if (debugLevel >= 3)
                dout<<"Creating load combo: "<<matches[1]<<" SF = "<<matches[3]<<"\n";
        }
        else
            combo.Name("");
        

    }
    else
        dout<<"Load Combo Parse Error"<<line<<"\n";

    return is;
}