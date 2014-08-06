//
//  SpecialColumn.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/28/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "SpecialColumn.h"

std::ostream& operator<<(std::ostream& os, SpecialColumn& SpecialColumn)
{
    os << SpecialColumn.Num() << " " << SpecialColumn.PDL1() << " " << SpecialColumn.PDL4() << " " <<SpecialColumn.STRMULT() << " " << SpecialColumn.STFMULT() << std::endl;

    return os;
}