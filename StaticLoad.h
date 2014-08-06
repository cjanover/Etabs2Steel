//
//  StaticLoad.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__StaticLoad__
#define __Etabs2Steel__StaticLoad__

#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <sstream>

#include "Coord.h"
#include "Parse.h"

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class StaticLoad
{
    public:
        StaticLoad(){force[0] = 0.f; force[1] = 0.f; force[2] = 0.f;};
    
        //Get functions
        unsigned int EtabsNodeNum(){return etabsNodeNum;};
        std::string EtabsFloorName(){return etabsFloorName;};
        std::string PatternName(){return patternName;};
        float Force(Coord::Direction dir){return force[dir];};
        std::string PatternType(){return patternType;};
        const std::string ParserString(){return parseString;};
        
        //Set functions
        void EtabsNodeNum(const unsigned int nNum){etabsNodeNum = nNum;};
        void EtabsFloorName(const std::string nFloor){etabsFloorName = nFloor;};
        void PatternName(std::string nPatternName){patternName = nPatternName;};
        void Force(const float nFx, const float nFy, const float nFz){force[0] = nFx; force[1] = nFy; force[2] = nFz;};
        void Force(const float nF, const Coord::Direction dir){force[dir] = nF;};
        void PatternType(const std::string nType){patternType = nType;};
    
    private:
        unsigned int etabsNodeNum;
        std::string etabsFloorName;
    
        std::string patternName;
        float force[3];
        std::string patternType;
    
        std::string parseString = PARSESTRINGS["PointObjectLoads"];
    
    

};

//Operator Overloads
std::istream& operator>>(std::istream& is, StaticLoad& load);


#endif /* defined(__Etabs2Steel__StaticLoad__) */
