//
//  Coord.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Coord.h"

float Coord::Coords(const Coord::Direction dir)
{
    switch (dir)
    {
        case X:
            return x;
            break;
        case Y:
            return y;
            break;
        case Z:
            return z;
            break;
        default:
            throw "Bad Coordinate Direction";
            break;
    
    }
}

float* Coord::Coords()
{
    float dir[3] = {x, y, z};
    float* ptr = dir;
    return ptr;
}

void Coord::Coords(float nx, float ny, float nz)
{
    x = nx;
    y = ny;
    z = nz;
}

Coord::Direction Coord::FindDirection(const std::string checkDirection)
{
    if (checkDirection == "X")
        return Direction::X;
    else if (checkDirection == "Y")
        return Direction::Y;
    else
        return Direction::Z;
}

char Coord::FindDirString(Coord::Direction dir)
{
    if (dir == Coord::Direction::X)
        return 'X';
    else if (dir == Coord::Direction::Y)
        return 'Y';
    else
        return 'Z';

}

//operator overloading
std::istream& operator>>(std::istream& is, Coord& coord)         //Input Operator
{
    d_out dout;
    //Expect inputs as x y z    where z may or may not be there.

    if (!is) return is; //check for bad read

    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{coord.ParserString()}))
    {
        //Convert read values to floats
        float nX = std::stod(matches[1].str());
        float nY = std::stod(matches[2].str());
        float nZ = 0.f;
        if (matches[3].matched)
            nZ = std::stod(matches[3].str());
        
        //update coordinates of coord
        coord.Coords(nX, nY, nZ);
    }
    else
        dout<<"Coordinate Parse Error"<<line<<"\n";
    
    return is;
}


std::ostream& operator<<(std::ostream& os, const Coord& coord)   //Output Operator
{
    return os;
}