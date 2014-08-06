//
//  VerticalConstraint.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__VerticalConstraint__
#define __Etabs2Steel__VerticalConstraint__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <map>

#include "Coord.h"

#include "FloatCompare.h"

#include "Debug.h"

extern int debugLevel;

class VerticalConstraint
{
    public:
        VerticalConstraint(){};
        VerticalConstraint(const float x, const float y, const float z, const float nALphaVC);

        //Get Functions
        Coord Coord(){return coord;};

        float AlphaVC(){return alphavc;};
    
        //Set Functions
        void Coord(const float x, const float y, const float z);
        void AlphaVC(const float nAlphaVC){alphavc = nAlphaVC;};
    
    
    private:
        class Coord coord;
        float alphavc;

};


#endif /* defined(__Etabs2Steel__VerticalConstraint__) */
