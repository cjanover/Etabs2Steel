//
//  PointSpring.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/2/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "PointSpring.h"


//Operator Overloads
std::istream& operator>>(std::istream& is, PointSpring& spring)
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches      \\[1]->Spring  Name, [2]-> Stiffnesses
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{spring.ParserString()}))
    {
        //Save info and update num stories

        spring.Name(matches[1].str());
        
        //Parse the Stiffness Info
        std::stringstream stiffProp(matches[2].str());
        
        std::string strDir;
        Coord::Direction dir;
        float stiffness;

        while (stiffProp >> strDir >> stiffness)
        {
            //Get Direction
            if (strDir == "UX")
                dir = Coord::Direction::X;
            else if (strDir == "UY")
                dir = Coord::Direction::Y;
            else
                dir = Coord::Direction::Z;
            //Save values
            spring.Stiffness(dir, stiffness);
        }
    }
    else
        dout<<"Point Spring Parse Error: "<<line<<"\n";

    return is;
}