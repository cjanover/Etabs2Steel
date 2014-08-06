//
//  VerticalConstraint.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "VerticalConstraint.h"

VerticalConstraint::VerticalConstraint(const float x, const float y, const float z, const float nAlphaVC):
    alphavc(nAlphaVC)
{
    coord.Coords(x, y, z);
}

void VerticalConstraint::Coord(const float x, const float y, const float z)
{
    coord.Coords(x, y, z);
}