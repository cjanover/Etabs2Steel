//
//  Grid.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/27/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Grid__
#define __Etabs2Steel__Grid__

#include <iostream>
#include <string>
#include <regex>
#include <map>

#include "Coord.h"

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class Grid
{
    public:
        Grid(){};
    
        //Get Functions
        const std::string ParserString(){return parseString;};
        const float Coordinate(){return coordinate;};
        const Coord::Direction EtabsGridDirection(){return etabsGridDirection;};

        const std::string GridName(){return gridName;};
    
        //Set Functions
        void Coordinate(const float nCoordinate){coordinate = nCoordinate;};
        void GridDirection(const Coord::Direction nDirection){etabsGridDirection = nDirection;};
        void GridName(const std::string nName){gridName = nName;};
    
    private:
        std::string parseString = PARSESTRINGS["Grid"];
        float coordinate;
        Coord::Direction etabsGridDirection;

        std::string gridName;

};

//Operator Overloads
std::istream& operator>>(std::istream& is, Grid& grid);

#endif /* defined(__Etabs2Steel__Grid__) */
