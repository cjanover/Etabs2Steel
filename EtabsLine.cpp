//
//  EtabsLine.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/12/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "EtabsLine.h"


//Operator Overloads
std::istream& operator>>(std::istream& is, EtabsLine& etabsLine)
{
    if (!is) return is; //check for bad read
    
    //read in line
    std::string line;
    std:;getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{etabsLine.ParserString()}))
    {
        //[1] Element num, [2] element type, [3] 1st etabs node, [4] 2nd etabs node, [5] 0 or 1, something
        etabsLine.EtabsID(matches[1].str());
        etabsLine.Type(matches[2].str());
        etabsLine.NodeConnectivity(std::stoi(matches[3].str()), std::stoi(matches[4].str()));
        
        //std::cout<<"Etabs Line: "<<matches[1]<< " Type: "<<matches[2]<<" Etabs Node ID: ("<<std::stoi(matches[3])<<" , "<<std::stoi(matches[4])<<")"<<std::endl;
    }

    return is;
}