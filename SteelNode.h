//
//  SteelNode.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__SteelNode__
#define __Etabs2Steel__SteelNode__

#include <iostream>

#include "Node.h"
#include "NodalRestraint.h"

extern std::map<unsigned int, unsigned int> nodeNumberLookup;

class SteelNode : public Node
{
    public:
        SteelNode(){};
        SteelNode(const unsigned int& nNum, const float& nX, const float& nY, const float& nZ, const unsigned int& nEtabsNodeNum):
            etabsNodeNum(nEtabsNodeNum),
            nodalShift(0.0f){Num(nNum); Coords(nX, nY, nZ);};
        const std::string ParserString(){return parseString;};
    
        //Set functions
        void EtabsNodeNum(const unsigned int nNum){etabsNodeNum = nNum;};
        void NodalFixity(const NodalRestraint nRestraint){nodeRestraint.SetFixity(nRestraint);};
        void NodalShift(const float nShift){nodalShift = nShift;};
    
        //Get functions
        unsigned int EtabsNodeNum(){return etabsNodeNum;};
        const int* NodalFixity(){return nodeRestraint.NodalFixity();};
        const int NodalFixity(int dir){return nodeRestraint.NodalFixity(dir);};
        const float NodalShift(){return nodalShift;};
    
        static int numSteelNodes;
        void incNumSteelNodes(){numSteelNodes+=1;};
    
        
    private:
        NodalRestraint nodeRestraint;
        float nodalShift;
        std::string parseString = PARSESTRINGS["SteelNode"];
    
        //Etabs Info
        unsigned int etabsNodeNum;
};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, SteelNode& node);
std::istream& operator>>(std::istream& is, SteelNode& node);

#endif /* defined(__Etabs2Steel__SteelNode__) */
