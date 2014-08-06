//
//  Releases.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/20/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Releases__
#define __Etabs2Steel__Releases__

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <map>

#include "Parse.h"

#include "FloatCompare.h"
#include "Debug.h"
extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class Releases
{
    public:
        enum ReleaseType {N1_FIX_N2_FIX, N1_FREE_N2_FREE, N1_FREE_N2_FIX, N1_FIX_N2_FREE};
        Releases(){};
        Releases(ReleaseType nReleases): releases(nReleases){};
    
    
    

        //Get functions
        ReleaseType Release(){return releases;};
        void WriteRelease(){};
        const std::string ParserString(){return parseString;};
        const std::string ReleaseString();

    
        //Set Functions
        void Release(ReleaseType nReleases){releases = nReleases;};
    
    private:
        ReleaseType releases;
        std::string parseString = PARSESTRINGS["ElementReleases"];

};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, const Releases& release);
std::istream& operator>>(std::istream& is, Releases& release);

#endif /* defined(__Etabs2Steel__Releases__) */


//
//  Releases.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/20/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//


