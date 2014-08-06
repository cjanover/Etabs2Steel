//
//  Debug.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 7/16/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Debug__
#define __Etabs2Steel__Debug__

#include <iostream>
#include <sstream>
#include <fstream>

extern std::ofstream debugFile;
extern bool developerMode;

struct d_out
{
    template<typename T>
    d_out&
    operator << (T&& x)
    {
        if (developerMode)
            std::cout<<x;
        debugFile << x;
        
        return *this;
    };
};
#endif /* defined(__Etabs2Steel__Debug__) */
