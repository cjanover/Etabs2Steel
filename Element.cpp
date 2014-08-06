//
//  Element.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Element.h"

void Element::Type(std::string nType)
{
    d_out dout;
    if (nType == "COLUMN")
        elementType = ElementType::COLUMN;
    else if (nType == "BEAM")
        elementType = ElementType::BEAM;
    else if (nType == "BRACE")
        elementType = ElementType::BRACE;
    else
    {
        dout<<"Etabs Element ID Error.\n";
    }
}