//
//  Node.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Node__
#define __Etabs2Steel__Node__

#include <iostream>
#include <regex>
#include <string>
#include <map>

#include "Coord.h"
#include "Parse.h"

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;
extern std::map <std::string, std::string> PARSESTRINGS;

class Node
{
    public:
        Node();
    //    Node(const unsigned int nNum, const float nX, const float nY, const float nZ):
    //        num(nNum){coords.Coords(nX, nY, nZ);};
    
        //Set Functions
        void Num(int nNum){num = nNum;};
        void ListNum(int nNum){listNum = nNum;};
        void Coords(const float nX, const float nY, const float nZ){coords.Coords(nX, nY, nZ);};
    
        void IncTotForce(const float nForce[]);
        void IncTotMass(const float nMass[]);
        void IncTotForce(const float nForce, const Coord::Direction dir);
        void IncTotMass(const float nMass, const Coord::Direction dir);
        void TotForce(const float nForce, const Coord::Direction dir){totForce[dir] = nForce;};
        void TotMass(const float nMass, const Coord::Direction dir){totMass[dir] = nMass;};
    
        void ZeroForce(const std::string patternName);
        void Force(const float nForce[], const std::string patternName);
        void Force(const float nForce, const std::string patternName, const Coord::Direction dir);
            
        //Return Functions
        float* Coords(){return coords.Coords();};
        float Coords(Coord::Direction dir){return coords.Coords(dir);};
        Coord& getCoord(){return coords;};
        unsigned int Num(){return num;};
        unsigned int ListNum(){return listNum;};
    
        float* TotForce(){return  totForce;};
        float* TotMass(){return totMass;};
        float TotForce(const Coord::Direction dir){return totForce[dir];};
        float TotMass(const Coord::Direction dir){return totMass[dir];};
    
        float* Force(const std::string patternName);
        float Force(const std::string patternName, const Coord::Direction dir);
        bool PatternExists(const std::string patternName);
    
        std::map<std::string, float>::iterator GetStartIterator(const Coord::Direction dir);
        std::map<std::string, float>::iterator GetEndIterator(const Coord::Direction dir);

    
    private:
        unsigned int num; //Node Number
        unsigned int listNum; //Node number in list
    
        Coord coords;     //Node x,z coordinates
    
        float totForce[3];
        
        std::map<std::string, float> forceX;
        std::map<std::string, float> forceY;
        std::map<std::string, float> forceZ;
        
        float totMass[3];
    

    
    

};


#endif /* defined(__Etabs2Steel__Node__) */
