//
//  EtabsNode.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "EtabsNode.h"

//operator overloading
std::istream& operator>>(std::istream& is, EtabsNode& node)         //Input Operator
{
    d_out dout;
    //Expect lines in format POINT "n"  x z y
    
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{node.ParserString()}))
    {
        //Convert node number
        node.Num(std::stod(matches[1].str()));
        
        //Parse Nodal coords
        GetSStream(matches[2].str())>>(node.getCoord());
        
        if (debugLevel >=3)
            dout<<"Etabs Node ("<<node.Num()<<") = ("<<node.Coords(Coord::Direction::X)<<", "<<node.Coords(Coord::Direction::Y)<<", "<<node.Coords(Coord::Direction::Z)<<")\n";
        
        
    }
    else
        dout<<"Etabs Node Parse Error"<<line<<"\n";
    
    return is;
}
