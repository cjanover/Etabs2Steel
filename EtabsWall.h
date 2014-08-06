//
//  EtabsWall.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__EtabsWall__
#define __Etabs2Steel__EtabsWall__

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <map>

#include "Parse.h"

#include "FloatCompare.h"

#include "Debug.h"
extern int debugLevel;
extern std::map <std::string, std::string> PARSESTRINGS;
class EtabsWall
{
    public:
        EtabsWall(){};
    
        //Get Functions
        std::vector<unsigned int>::iterator WallConnectivityStart(){return wallConnectivity.begin();};
        std::vector<unsigned int>::iterator WallConnectivityEnd(){return wallConnectivity.end();};
        std::string Name(){return name;};
    
        //Set Functions
        void AddConnectivity(unsigned int nNode){wallConnectivity.push_back(nNode);};
        void Name(std::string nName){name = nName;};
    
    
        const std::string ParserStringWall(){return parseStringWall;};
        const std::string ParserStringSlab(){return parseStringSlab;};

    private:
        std::string name;
        std::vector<unsigned int> wallConnectivity;
    
    
        std::string parseStringWall = PARSESTRINGS["EtabsWallConnect"];
        std::string parseStringSlab = PARSESTRINGS["EtabsSlabConnect"];

};

//Operator Overloads
std::istream& operator>>(std::istream& is, EtabsWall& node);

#endif /* defined(__Etabs2Steel__EtabsWall__) */
