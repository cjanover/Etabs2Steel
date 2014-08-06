//
//  DeckProp.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/3/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "DeckProp.h"

//operator overloading
std::istream& operator>>(std::istream& is, DeckProp& prop)         //Input Operator
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;

    
    //[1] = Name, [2] = Type, [3] = DeckType, [4] = ConcMaterial, [5] = DeckMaterial, [6] = DeckSlabDepth, [7] = DeckRibDepth, [8] = DeckRibWidthTop
    //[9] = DeckRibWidthBottom, [10] = DeckRibSpacing, [11] = DeckShearThickness, [12] = DeckUnitWeight, [13] = ShearSTudDiam, [14] = ShearStudHeight
    //[15] = ShearStudFU
    if (std::regex_search(line, matches, std::regex{prop.ParserStringWall()})) //[1] = Name, [2] = Type, [3] = Material, [4] = Thickness
    {
        //Save Data
        prop.Name(matches[1].str());
        prop.Type(matches[2].str());
    
        prop.DeckType(matches[3].str());
        prop.ConcMaterial(matches[4].str());
        prop.DeckMaterial(matches[5].str());
    
        prop.DeckSlabDepth(std::stof(matches[6].str()));
        prop.DeckRibDepth(std::stof(matches[7].str()));
        prop.DeckRibWidthTop(std::stof(matches[8].str()));
        prop.DeckRibWidthBottom(std::stof(matches[9].str()));
        prop.DeckRibSpacing(std::stof(matches[10].str()));
        prop.DeckShearThickness(std::stof(matches[11].str()));
        prop.DeckUnitWeight(std::stof(matches[12].str()));
        prop.ShearStudDiam(std::stof(matches[13].str()));
        prop.ShearStudHeight(std::stof(matches[14].str()));
        prop.ShearStudFU(std::stof(matches[15].str()));
        
    }
    else
        dout<<"Etabs Deck Property Parse Error\n";

    return is;
}