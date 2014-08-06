//
//  SteelNode.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "SteelNode.h"

int SteelNode::numSteelNodes=1;

//operator overloading
std::istream& operator>>(std::istream& is, SteelNode& node)         //Input Operator
{
    d_out dout;
    //Expect lines in format POINT "n"  x z y
    
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{node.ParserString()})) //[1] Etabs node #, [2] Story name
    {
        node.EtabsNodeNum(std::stod(matches[1].str()));
    }
    else
        dout<<"SteelNode Parse Error"<<line<<"\n";

    return is;
}


std::ostream& operator<<(std::ostream& os, SteelNode& node)   //Output Operator
{
    //Form is N, C(1), C(2), ID(1), ID(2), IDJ, IDOUB, F(1), F(2), F(3), F(4)
    // Node Number, X Coordinate, Y Coordinate, X Fixity, Y Fixity, RZ Fixity, Panel Zone Thickness Rule (1), Static Force in X, Static Force in Y, Mass in X, Mass in Y

    std::map<unsigned int, unsigned int> tmp = nodeNumberLookup;
    //Get NodeNum
    unsigned int num = nodeNumberLookup[node.Num()];

    os << num << " "<< node.Coords(Coord::Direction::X)<<" "<< node.Coords(Coord::Direction::Y)<<" "<< node.NodalFixity(0)<<" "<<node.NodalFixity(1)<<" "<<node.NodalFixity(3)<<" "<<"1"<<" "<<node.TotForce(Coord::Direction::X)<<" "<<node.TotForce(Coord::Direction::Y)<<" "<<node.TotMass(Coord::Direction::X)<<" "<<node.TotMass(Coord::Direction::Y)<<std::endl;
    return os;
}