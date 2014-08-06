//
//  EtabsWall.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "EtabsWall.h"

//operator overloading


std::istream& operator>>(std::istream& is, EtabsWall& wall)         //Input Operator
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{wall.ParserStringWall()})) //[1] Name, [2], [3], [4], [5] = Node Connectivity
    {
        //std::cout<<"Wall"<<std::endl;
        //std::cout<<matches[1]<<" " <<matches[2]<<" "<<matches[3]<<" "<<matches[4]<<" "<<matches[5]<<std::endl;
    
    
        //Save Info
        wall.Name(matches[1].str());
        
        wall.AddConnectivity(std::stoi(matches[2].str()));
        wall.AddConnectivity(std::stoi(matches[3].str()));
        wall.AddConnectivity(std::stoi(matches[4].str()));
        wall.AddConnectivity(std::stoi(matches[5].str()));
    }
    else if (std::regex_search(line, matches, std::regex{wall.ParserStringSlab()})) //[1] Name, [2], [3], [4], [5] = Node Connectivity
    {
        //std::cout<<"Slab"<<std::endl;
        //std::cout<<matches[1]<<" " <<matches[2]<<" "<<matches[3]<<" "<<matches[4]<<" "<<matches[5]<<std::endl;
    
        //Save Info
        wall.Name(matches[1].str());
        
        wall.AddConnectivity(std::stoi(matches[2].str()));
        wall.AddConnectivity(std::stoi(matches[3].str()));
        wall.AddConnectivity(std::stoi(matches[4].str()));
        wall.AddConnectivity(std::stoi(matches[5].str()));

    }
    else
        dout<<"Etabs Wall Parse Error\n";

    return is;
}
