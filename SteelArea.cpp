//
//  SteelArea.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "SteelArea.h"

std::istream& operator>>(std::istream& is, SteelArea& area)         //Input Operator
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{area.ParserString()})) //[1] EtabsName, [2] = Floor, [3] = Section Name
    {
  //      std::cout<<"Steel Area"<<std::endl;
  //      std::cout<<matches[1]<<" " <<matches[2]<<" "<<matches[3]<<std::endl;
    
    
        //Save Info
        area.EtabsName(matches[1].str());
        area.FloorName(matches[2].str());
        area.SectionName(matches[3].str());
    }
    else
        dout<<"Steel Area Parse Error\n";

    return is;
}


std::ostream& operator<<(std::ostream& os, SteelArea& area)
{
    //N MT, MAT, IOR, ISS, ICS, ICT, LM(1), LM(2), WSCALE
    //Element Number, Material Set Number, strong axis / weak axis, Steel member designator, Slab Designator, Element Fiber Category, Connectivity(1), Connectivity(2), Scaling (1)
    
   //N Height, Width, Thickness, Shear Modulus, Connectivity
   std::vector<unsigned int>::iterator connectivityIter;

    os << area.Num() << " " << area.Height() << " " << area.Width() << " " << area.Thickness() << " " << area.ShearMod();
    
    for (connectivityIter = area.WallConnectivityStart(); connectivityIter != area.WallConnectivityEnd(); connectivityIter++)
    {
        unsigned int nodeNum = nodeNumberLookup[*connectivityIter];
        os << " " << nodeNum;
    
    }
    os << std::endl;
  
    return os;
}