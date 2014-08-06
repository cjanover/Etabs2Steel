//
//  Element.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Element__
#define __Etabs2Steel__Element__

#include <iostream>
#include <string>
#include <vector>

#include "Node.h"
#include "Releases.h"

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

class Element
{
    public:
        Element(){};
    
        enum ElementType {COLUMN, BEAM, BRACE};
    
        //Get functions
        ElementType Type(){return elementType;};
        std::vector<unsigned int> NodeConnectivity(){return nodeConnectivity;};
        std::string SectionName(){return sectionName;};
    
        //Set functions
        void Type(ElementType nElementType){elementType = nElementType;};
        void Type(std::string nType);
        void NodeConnectivity(unsigned int n1, unsigned int n2){nodeConnectivity.push_back(n1); nodeConnectivity.push_back(n2);};
        void ModifyNodeConnectivity(unsigned int n, unsigned int mID){nodeConnectivity[mID] = n;};
        void SectionName(std::string nSectionName){sectionName = nSectionName;};
    
        
    
    private:
        std::vector <unsigned int> nodeConnectivity;
        ElementType elementType;
    
        std::string sectionName;

    


};

#endif /* defined(__Etabs2Steel__Element__) */
