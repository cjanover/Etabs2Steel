//
//  Parse.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Parse.h"


std::stringstream GetSStream(std::string inString)
{
    std::stringstream ss;
    ss << inString;
    
    return ss;
}