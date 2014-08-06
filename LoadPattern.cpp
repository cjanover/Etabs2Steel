//
//  LoadPattern.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "LoadPattern.h"


//Operator Overloads

std::istream& operator>>(std::istream& is, LoadPattern& pattern)
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{pattern.ParserString()}))
    {
        //[1] = Pattern Name
        //[2] = Pattern Type
        
        //Save data
        pattern.PatternName(matches[1].str());
        pattern.PatternType(matches[2].str());
        
        if (debugLevel >= 3)
            dout<<"Making Load pattern: "<<matches[1].str()<<"\n";
    }
    else
        dout<<"Load Pattern Parse Error"<<line<<"\n";

    return is;


}
