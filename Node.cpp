//
//  Node.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Node.h"

Node::Node():coords()
{
    for (int i = 0; i<=2; i++)
    {
        totForce[i] = 0.f;
        totMass[i] = 0.f;
    }
}

void Node::IncTotForce(const float nForce[])
{
    for (int i = 0; i<=2; i++)
    {
        totForce[i] += nForce[i];
    }
}

void Node::IncTotForce(const float nForce, const Coord::Direction dir)
{
    totForce[dir] += nForce;
}

void Node::IncTotMass(const float nMass[])
{
    totMass[0] += nMass[0] + nMass[2]; //Shift Vertical Mass to horizontal (be sure to negate) since neg down
    totMass[1] += nMass[1] + nMass[2]; //Shift Vertical Mass to Horizontal
    totMass[2] += nMass[2];
}

void Node::IncTotMass(const float nMass, const Coord::Direction dir)
{
    if (dir == Coord::Direction::Z) //If Vertical be careful of direction
    {
        totMass[0] += nMass;
        totMass[1] += nMass;
        totMass[2] += nMass;
    }
    else
    {
        totMass[dir] += nMass;
    }

}


void Node::ZeroForce(const std::string patternName)
{
    forceX[patternName] = 0.f;
    forceZ[patternName] = 0.f;
    forceY[patternName] = 0.f;
}

void Node::Force(const float nForce[], const std::string patternName)
{
    forceX[patternName] = nForce[0];
    forceY[patternName] = nForce[1];
    forceZ[patternName] = nForce[2];
}

void Node::Force(const float nForce, const std::string patternName, const Coord::Direction dir)
{
    if (dir == Coord::Direction::X)
        forceX[patternName] = nForce;
    else if (dir == Coord::Direction::Y)
        forceY[patternName] = nForce;
    else if (dir == Coord::Direction::Z)
        forceZ[patternName] = nForce;
}

float* Node::Force(const std::string patternName)
{
    float* tmp[3];
    tmp[0] = &forceX[patternName];
    tmp[1] = &forceY[patternName];
    tmp[2] = &forceZ[patternName];
    
    return *tmp;
}

float Node::Force(const std::string patternName, const Coord::Direction dir)
{
    if (dir == Coord::Direction::X)
        return forceX[patternName];
    else if (dir == Coord::Direction::Y)
        return forceY[patternName];
    else if (dir == Coord::Direction::Z)
        return forceZ[patternName];
    return 0.f;
}

bool Node::PatternExists(const std::string patternName)
{
    std::map<std::string, float>::iterator patternIter;
    
    for (patternIter = forceX.begin(); patternIter != forceX.end(); patternIter++)
    {
        if (patternIter->first == patternName)
            return true;
    }
    return false;
}

std::map<std::string, float>::iterator Node::GetStartIterator(const Coord::Direction dir)
{
    if (dir == Coord::Direction::X)
        return forceX.begin();
    else if (dir == Coord::Direction::Y)
        return forceY.begin();
    else
        return forceZ.begin();
}

std::map<std::string, float>::iterator Node::GetEndIterator(const Coord::Direction dir)
{
    if (dir == Coord::Direction::X)
        return forceX.end();
    else if (dir == Coord::Direction::Y)
        return forceY.end();
    else
        return forceZ.end();
}