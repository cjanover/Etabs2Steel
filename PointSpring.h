//
//  PointSpring.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/2/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__PointSpring__
#define __Etabs2Steel__PointSpring__

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <map>

#include "Coord.h"
#include "Parse.h"

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;
extern std::map <std::string, std::string> PARSESTRINGS;

class PointSpring
{
    public:
        PointSpring(){};
    
        //Get Functions
        std::string Name(){return name;};
        float Stiffness(Coord::Direction dir){return stiffness[dir];};
        float ALP(){return alp;};
        float STRH(){return strh;};
        float STRVU(){return strvu;};
        float STRVD(){return strvd;};
    
        const std::string ParserString(){return parseString;};
    
        //Set Functions
        void Name(const std::string nName){name = nName;};
        void Stiffness(const Coord::Direction dir, float nStiff){stiffness[dir] = nStiff;};
        void ALP(const float nALP){alp = nALP;};
        void STRH(const float nSTRH){strh = nSTRH;};
        void STRVU(const float nSTRVU){strvu = nSTRVU;};
        void STRVD(const float nSTRVD){strvd = nSTRVD;};
    
    
    private:
        std::string name;
        float stiffness[3];
    
        float alp;
        float strh;
        float strvu;
        float strvd;
    
        std::string parseString = PARSESTRINGS["PointSpring"];
    
};

//Operator Overloads
std::istream& operator>>(std::istream& is, PointSpring& spring);


#endif /* defined(__Etabs2Steel__PointSpring__) */
