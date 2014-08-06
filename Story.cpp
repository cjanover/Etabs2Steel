//
//  Story.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Story.h"

unsigned int Story::numStories = 0; //Number of stories

//operator overloading
std::istream& operator>>(std::istream& is, Story& story)         //Input Operator
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches      \\[1]->Story Name, [2]->Height or Elev, [3]->Value
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{story.ParserString()}))
    {
        //Save info and update num stories
        story.Name(matches[1].str());
        story.ElvType(matches[2].str());
        story.Height(std::stod(matches[3].str()));
        story.incNumStories();
        
        if (debugLevel >= 3)
            dout<<"Story: "<<story.Name()<<" ("<<story.Height()<<")\n";
    }
    else
        dout<<"Story Parse Error"<<line<<"\n";

    return is;
}


std::ostream& operator<<(std::ostream& os, const Story& story)   //Output Operator
{
    return os;
}