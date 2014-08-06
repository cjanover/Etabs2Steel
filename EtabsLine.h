//
//  EtabsLine.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/12/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__EtabsLine__
#define __Etabs2Steel__EtabsLine__

#include <iostream>

#include "Element.h"

class EtabsLine : public Element
{
    public:
        EtabsLine(){};
    
        //set functions
        void EtabsID(const std::string nID){etabsID = nID;};
    
        //get functions
        const std::string ParserString(){return parseString;};
        const std::string EtabsID(){return etabsID;};
    
        
    
    private:
        std::string parseString = PARSESTRINGS["EtabsLine"];
        std::string etabsID;
        Releases::ReleaseType releases;
    
};

//Operator Overloads
std::istream& operator>>(std::istream& is, EtabsLine& coord);

#endif /* defined(__Etabs2Steel__EtabsLine__) */
