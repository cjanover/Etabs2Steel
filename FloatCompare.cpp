//
//  FloatCompare.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 7/9/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "FloatCompare.h"
#include <cmath>
extern float tol;

bool isEqual(float a, float b)
{
    float ratio;
    if (b != 0)
        ratio = a/b;
    else if (a != 0)
        ratio = b/a;
    else
        return true;
    
    if (std::abs(ratio - 1) < tol)
        return true;
    return false;
}