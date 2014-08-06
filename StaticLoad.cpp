//
//  StaticLoad.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "StaticLoad.h"


//Operator Overloads
std::istream& operator>>(std::istream& is, StaticLoad& load)
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{load.ParserString()}))
    {
        //[1] = Etabs node Number
        //[2] = Etabs Story Name
        //[3] = Load Type
        //[4] = Load Pattern Name
        //[5] = Loads in format    Dir val
        
        //Save data
        load.EtabsNodeNum(std::stoi(matches[1].str()));
        load.EtabsFloorName(matches[2].str());
        load.PatternType(matches[3].str());
        load.PatternName(matches[4].str());
        
        //Parse through loads, first convert to stringstream
        std::stringstream forces = GetSStream(matches[5]);

        std::string forceDir;
        std::string forceVal;
        while (forces >> forceDir >> forceVal)
        {
            if (forceDir == "FX")
                load.Force(std::stod(forceVal), Coord::Direction::X);
            
            else if (forceDir == "FY")
                load.Force(std::stod(forceVal), Coord::Direction::Y);
            
            else if (forceDir == "FZ")
                load.Force(std::stod(forceVal), Coord::Direction::Z);
            else
                dout<<"Unusable force: "<<forceDir<<" "<<forceVal<<". On EtabsNode: "<<matches[1]<<" Story: "<<matches[2]<<"\n";
            
            if (debugLevel >= 3)
                dout<<"Adding Force: "<<forceVal<<" in Direction: "<<forceDir<<" for Pattern: "<<matches[4].str()<<"\n";
        }
    }
    else
        dout<<"Static Load Parse Error"<<"\n";

    return is;
}