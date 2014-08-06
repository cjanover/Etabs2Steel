//
//  SteelLine.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/12/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "SteelLine.h"


void SteelLine::SetICAT(unsigned int etabsNode1, unsigned int node1Ref)
{
    d_out dout;
    //Get which etabs node the steel nodes are referencing
    //node1Ref

    //This represents which etabs node came first in its definition to properly add releases
    //etabsNode1
    //etabsNode2


    if (this->Type() == COLUMN) //If its a column then orrientations need to be flipped
    {
        if (IOR == 1) // Strong Axis
        {
            /*if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
                ICAT = 1;
            else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
                ICAT = 3;
            else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
                ICAT = 2;
            else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
                ICAT = 4;
            */
            ICAT = 0; //N1_FIX_N2_FIX>
        }
        else    //Weak Axis
        {
            /*
            if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
                ICAT = 5;
            else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
                ICAT = 7;
            else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
                ICAT = 6;
            else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
                ICAT = 8;
            */
            ICAT = 5;   //N1_FIX_N2_FIX
        }
    }
    else if (this->Type() == BEAM)
    {
        if (IOR == 1) //Strong Axis
        {
            if (node1Ref == etabsNode1) //Then the nodes are the same and release orrientation is okay
            {
                if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
                    ICAT = 9;
                else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
                    ICAT = 10;
                else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
                    ICAT = 11;
                else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
                    ICAT = 12;
            }
            else //Then the steel node is flipped and release orrientation needs to be changed
            {
                if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
                    ICAT = 9;
                else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
                    ICAT = 11;
                else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
                    ICAT = 10;
                else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
                    ICAT = 12;
            }
        }
    }
    else    //Brace
        ICAT = 0;


/*

 if (this->Type() == COLUMN)
 {
    if (IOR == 1) //Strong Axis
    {
        if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
            ICAT = 1;
        else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
            ICAT = 2;
        else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
            ICAT = 3;
        else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
            ICAT = 4;
    }
    else    // Weak Axis
    {
        if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
            ICAT = 5;
        else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
            ICAT = 6;
        else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
            ICAT = 7;
        else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
            ICAT = 8;
    }
 }
 else if (this->Type() == BEAM)
 {
    if (IOR == 1) //Strong Axis
    {
        if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FIX)
            ICAT = 9;
        else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FIX)
            ICAT = 10;
        else if (releases.Release() == Releases::ReleaseType::N1_FIX_N2_FREE)
            ICAT = 11;
        else if (releases.Release() == Releases::ReleaseType::N1_FREE_N2_FREE)
            ICAT = 12;
    }
    
 }
 else   ICAT = 0;
 */
 
    if (debugLevel >= 1)
        dout<<"Element: "<<SteelID()<<" given release category of "<<ICAT<<"\n";
}

void SteelLine::SetIOR(Coord::Direction primaryEtabsDirection, Coord::Direction elementPrimaryDirection)
{
    d_out dout;
    if (Type() == Element::ElementType::COLUMN)
    {
        IOR = 1;
        if (angle == 90)
            IOR = IOR*-1;
        if (primaryEtabsDirection == Coord::Direction::Y)
            IOR = IOR*-1;
        if (elementPrimaryDirection != primaryEtabsDirection)
            IOR = IOR*-1;
    }
    else if (Type() == Element::ElementType::BEAM)
    {
        IOR = 1;
    }
    else    //Brace
    {
        //If element section name begins with HSS then strong axis, else weak
        if (SectionName().substr(0,3) == "HSS")
            IOR = 1;
        else
            IOR = -1;
    }
    if (debugLevel >=1 )
        dout<<"Element: "<<SteelID()<<" given a element orientation of "<<IOR<<"\n";
}


//Operator Overloads
std::istream& operator>>(std::istream& is, SteelLine& steelLine)
{
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    
    if (std::regex_search(line, matches, std::regex{steelLine.ParserString()}))
    {
        d_out dout;
        //[1] Etabs Element ID, [2] Etabs Story ID, [3] Section Name, [4] Angle String, [6] Releases
        steelLine.EtabsRefID(matches[1].str());
        steelLine.EtabsStoryID(matches[2].str());
        steelLine.SectionName(matches[3].str());
        
        //Need to take care of Releases
        
        //Parse Release
        std::string releaseParseString = "";    //Since the fix-fix default is empty, pass in an emptry string if it doesnt find the word RELEASE

        if (std::regex_search(line, std::regex{R"(RELEASE)"}))
            releaseParseString = matches[6].str();
        
        Releases nRelease;
        
        GetSStream(releaseParseString)>>nRelease;
//        std::cout<<nRelease.ReleaseString()<<std::endl;
        steelLine.Release(nRelease);
        //steelLine.SetICAT();
        
        //Parse Rotation Angle
        int angle = 0;
        if (std::regex_search(line, std::regex{R"(ANG)"}))
        {
            std::smatch angleMatches;
            if (std::regex_search(matches[4].str(), angleMatches, std::regex{R"((\d+))"}))
                angle = std::stoi(angleMatches[1]);
            else
                dout << "Error: Unknown Rotation Angle: "<<matches[4].str()<<". Ignoring. \n";
        
        }
        steelLine.SetAngle(angle);
        
        //std::cout<<"Reading: Etabs RefID: "<<matches[1].str()<<" Etabs StoryID: "<<matches[2]<<" Section Name: "<<matches[3]<<" Releases: "<<steelLine.ReleaseString()<<std::endl;
        
    }
    
    
    return is;
}

std::ostream& operator<<(std::ostream& os, SteelLine& steelLine)
{
    //N MT, MAT, IOR, ISS, ICS, ICT, LM(1), LM(2), WSCALE
    //Element Number, Material Set Number, strong axis / weak axis, Steel member designator, Slab Designator, Element Fiber Category, Connectivity(1), Connectivity(2), Scaling (1)
    
    //Get element connectivity
    std::vector<unsigned int> connectivity = steelLine.NodeConnectivity();
    
    //Get nodes
    unsigned int nd1 = nodeNumberLookup[connectivity[0]];
    unsigned int nd2 = nodeNumberLookup[connectivity[1]];
    
    
    os << steelLine.SteelID() << " "<< steelLine.GetMT()<<" " << steelLine.GetMAT() <<" "<<steelLine.GetIOR()<<" "<<steelLine.SteelSectionID()<<" "<<steelLine.ICS()<<" "<< steelLine.GetICAT() <<" "<<nd1<<" "<<nd2<<" "<<"1"<<std::endl;


    return os;
}