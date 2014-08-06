//
//  FoundationNode.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/2/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__FoundationNode__
#define __Etabs2Steel__FoundationNode__

#include <iostream>

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
extern std::map<unsigned int, unsigned int> nodeNumberLookup;

class FoundationNode
{
    public:
        FoundationNode(){};
    
        //Get Functions
        unsigned int Num(){return nodeNum;};
        unsigned int EtabsRefName(){return etabsRefName;};
        std::string FloorName(){return floorName;};
    
        std::string Name(){return name;};
        float Stiffness(Coord::Direction dir){return stiffness[dir];};
        float ALP(){return alp;};
        float STRH(){return strh;};
        float STRVU(){return strvu;};
        float STRVD(){return strvd;};
    
        const std::string ParserString(){return parseString;};
    
        //Set Functions
        void Num(unsigned int nNum){nodeNum = nNum;};
        void EtabsRefName(unsigned int nEtabsRefName){etabsRefName = nEtabsRefName;};
        void FloorName(std::string nFloor){floorName = nFloor;};
    
        void Name(const std::string nName){name = nName;};
        void Stiffness(const Coord::Direction dir, float nStiff){stiffness[dir] = nStiff;};
        void ALP(const float nALP){alp = nALP;};
        void STRH(const float nSTRH){strh = nSTRH;};
        void STRVU(const float nSTRVU){strvu = nSTRVU;};
        void STRVD(const float nSTRVD){strvd = nSTRVD;};
    
    
    private:
        unsigned int nodeNum;
        unsigned int etabsRefName;
        std::string floorName;
    
        std::string name;
        float stiffness[3];
    
        float alp;
        float strh;
        float strvu;
        float strvd;
    
        std::string parseString = PARSESTRINGS["PointAssign"];
    
};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, FoundationNode& fndNode);
std::istream& operator>>(std::istream& is, FoundationNode& fndNode);


#endif /* defined(__Etabs2Steel__FoundationNode__) */
