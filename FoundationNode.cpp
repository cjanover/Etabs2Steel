//
//  FoundationNode.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/2/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "FoundationNode.h"

//Operator Overloads
std::istream& operator>>(std::istream& is, FoundationNode& fndNode)
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches      \\[1]->Etabs Ref Name, [2]->Floor Name, 3->SpringProperty Name
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{fndNode.ParserString()}))
    {
        fndNode.EtabsRefName(std::stoi(matches[1].str()));
        fndNode.FloorName(matches[2].str());
        fndNode.Name(matches[3].str());
    }
    else
        dout<<"Foundation Node Parse Error"<<line<<"\n";

    return is;
}

std::ostream& operator<<(std::ostream& os, FoundationNode& fndNode)   //Output Operator
{
    //Form is LMF STFH STFV ALP STRH STRVU STRVD
    
    //Node Number, Horizontal Stiff, Ver Stiffness, Post-Yield Stiffness Ratio, Hor Yield Str, Upward Ver Yield Str, Downward Ver Yield Str

    unsigned int num = nodeNumberLookup[fndNode.Num()];

    os << num << " " << fndNode.Stiffness(Coord::Direction::X) << " " << fndNode.Stiffness(Coord::Direction::Y) << " " << fndNode.ALP() << " " << fndNode.STRH() << " " << fndNode.STRVU() << " " << fndNode.STRVD() << std::endl;

    return os;
}