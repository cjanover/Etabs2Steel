//
//  Story.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Story__
#define __Etabs2Steel__Story__

#include <iostream>
#include <string>
#include <map>
#include <regex>

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class Story
{
    public:
        Story(){};
    
        //Set functions
        void Name(const std::string nName){name = nName;};
        void Height(const float nHeight){height = nHeight;};
        void Elevation(const float floorBelowElev){elevation = floorBelowElev + height;};
        void ElvType(const std::string nElvType){elvType = nElvType;};

        void incNumStories(){numStories+=1;};
    
        //Get functions
        const std::string ParserString(){return parseString;};
        const float Height(){return height;};
        const float Elevation(){return elevation;};
        const std::string Name(){return name;};
        const std::string ElvType(){return elvType;};

    
    
        static unsigned int numStories;
    private:
        std::string name;
        std::string elvType;    //"Height" for normal story, "ELEV" for base
        float height;  //Measured from floor down
        float elevation;

    
    
        std::string parseString = PARSESTRINGS["Story"];

};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, const Story& story);
std::istream& operator>>(std::istream& is, Story& story);

#endif /* defined(__Etabs2Steel__Story__) */
