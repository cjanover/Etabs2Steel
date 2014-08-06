//
//  Coord.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Coord__
#define __Etabs2Steel__Coord__

#include <iostream>
#include <regex>
#include <string>
#include <map>

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class Coord
{
    public:
        //Enumeration
        enum Direction {X, Y, Z};
    
        //Constructors
        Coord():x(0.f), y(0.f), z(0.f){};
        Coord(float nx, float ny, float nz): x(nx), y(ny), z(nz){};
        Coord(float nCoord[]):x(nCoord[0]), y(nCoord[1]), z(nCoord[2]){};
    
        //Return Functions
        float Coords(const Direction dir); //Return a specific coordinate
        float* Coords();                   //Return all coordinates
        static Direction FindDirection(const std::string);
        static char FindDirString(Coord::Direction dir);

        //Set Functions
        void Coords(float nx, float ny, float nz);

        const std::string ParserString(){return parseString;};
    
    
    private:
        float x; //x in Etabs -> x in Steel
        float y; //y in Etabs -> z in Steel (multiple frame direction)
        float z; //z in Etabs -> y in Steel
    
        //std::string parseString = R"((\w+)\s(\w+)\s(\w+)?)";
        std::string parseString = PARSESTRINGS["Coord"];

};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, const Coord& coord);
std::istream& operator>>(std::istream& is, Coord& coord);


#endif /* defined(__Etabs2Steel__Coord__) */