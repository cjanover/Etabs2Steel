//
//  NodalRestraint.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/26/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__NodalRestraint__
#define __Etabs2Steel__NodalRestraint__

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include "Coord.h"
#include "FoundationNode.h"

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;
extern std::map <std::string, std::string> PARSESTRINGS;

class NodalRestraint //Done in the etabs sense
{
    public:
        NodalRestraint();
        const std::string ParserString(){return parseString;};
    
        //Set Functions
        void EtabsNodeNum(const unsigned int nEtabsNodeNum){etabsNodeNum = nEtabsNodeNum;};
        void EtabsFloor(const std::string nEtabsFloor){etabsFloor = nEtabsFloor;};
        void NodeFixity(int nUX, int nUY, int nUZ, int nRZ);
        void NodeSpring(bool nUS[], std::string nUSP[]);
        void SetFixity(NodalRestraint nRestraint);
        void FndNode(const FoundationNode nFndNode){fndNode = nFndNode;};
        void FndNodeExists(const bool nExist){fndNodeExists = nExist;};

    
        //Get Functions
        unsigned int EtabsNodeNum(){return etabsNodeNum;};
        std::string EtabsFloor(){return etabsFloor;};
        int* NodalFixity(){return U_Free;};
        int NodalFixity(int dir){return U_Free[dir];};
        bool* NodalSpring(){return U_NoSpring;};
        std::string* NodalSpringProp(){return U_SpringProp;};
        NodalRestraint* GetFixity(){return this;};
        FoundationNode FndNode(){return fndNode;};
        bool FndNodeExists(){return fndNodeExists;};
    
    private:
        unsigned int etabsNodeNum;
        std::string etabsFloor;
        FoundationNode fndNode;
        bool fndNodeExists = false;
    
        std::string parseString = PARSESTRINGS["NodalFixity"];
        int U_Free[4];
        bool U_NoSpring[4];
        std::string U_SpringProp[4];

};


//Operator Overloads
std::istream& operator>>(std::istream& is, NodalRestraint& line);
std::ostream& operator<<(std::ostream& os, const NodalRestraint& line);

#endif /* defined(__Etabs2Steel__NodalRestraint__) */
