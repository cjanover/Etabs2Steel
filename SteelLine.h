//
//  SteelLine.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/12/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__SteelLine__
#define __Etabs2Steel__SteelLine__

#include <iostream>

#include "Element.h"

extern std::map<unsigned int, unsigned int> nodeNumberLookup;

class SteelLine : public Element
{
    public:
        SteelLine(){};
    
        //set functions
        void SteelID(const unsigned int nID){steelID = nID;};
        void EtabsRefID(const std::string nID){etabsRefID = nID;};
        void EtabsStoryID(const std::string nID){etabsStoryID = nID;};
        void Release(Releases nReleases){releases = nReleases;};
        void SetICAT(unsigned int etabsNode1, unsigned int node1Ref);
        void SetIOR(Coord::Direction primaryEtabsDirection, Coord::Direction elementPrimaryDirection);
        void SetAngle(int nAngle){angle = nAngle;};
        void SetMT(unsigned int nMT){MT = nMT;};
        void SetMAT(unsigned int nMAT){MAT = nMAT;};
        void SteelSectionID(unsigned long nID){steelSectionID = nID;};
        void ICS(const unsigned int nICS){ics = nICS;};
    
        //get functions
        const std::string ParserString(){return parseString;};
        const unsigned int SteelID(){return steelID;};
        const std::string EtabsStoryID(){return etabsStoryID;};
        const std::string EtabsRefID(){return etabsRefID;};
        Releases Release(){return releases;};
        const std::string ReleaseString(){return releases.ReleaseString();};
        const unsigned int GetICAT(){return ICAT;};
        const int GetIOR(){return IOR;};
        const unsigned int GetMT(){return MT;};
        const unsigned int GetMAT(){return MAT;};
        const unsigned long SteelSectionID(){return steelSectionID;};
        const unsigned int ICS(){return ics;};
    
    
    private:
        std::string parseString = PARSESTRINGS["SteelLine"];
        unsigned long steelSectionID;
        unsigned int steelID;
        Releases releases;
        unsigned int ICAT;
        unsigned int MAT;
        int IOR = 1;
        int angle;
        unsigned int MT;
        unsigned int ics;
    
        std::string etabsRefID; //Used to refence back what etabs element it refers to
        std::string etabsStoryID; //Used to reference back what etabs floor it rests on
    
};

//Operator Overloads
std::istream& operator>>(std::istream& is, SteelLine& line);
std::ostream& operator<<(std::ostream& os, SteelLine& line);
#endif /* defined(__Etabs2Steel__SteelLine__) */
