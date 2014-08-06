//
//  Grid.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/27/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Grid.h"




std::istream& operator>>(std::istream& is, Grid& grid)
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
   
    if (std::regex_search(line, matches, std::regex{grid.ParserString()})) //[1] Etabs node #, [2] Story name
    {
        //Matches[1] = Grid Label
        //Matches[2] = Grid Direction
        //Matches[3] = Grid Coordinate

        //Parse and Save Info
        grid.GridName(matches[1].str());
        grid.GridDirection(Coord::FindDirection(matches[2].str()));
        grid.Coordinate(std::stof(matches[3].str()));
        
        if (debugLevel >=3)
            dout<<"Creating Grid: "<<matches[1].str()<<" Direction: "<<matches[2].str()<<" Coordinate: "<<matches[3].str()<<"\n";
    }
    else
        dout<<"Grid Parse Error"<<line<<"\n";

    return is;
}