//
//  Slab.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/9/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Slab.h"

void Slab::AddSlabDefinition(const std::string label, const float adeck, const float ddeck, const float aslab, const float dslab)
{
    std::ostringstream def;
    
    ////Create Conversion
    //AddConversion(label);
    
    def << label << " " << std::to_string(adeck) << " " << std::to_string(ddeck) << " " << std::to_string(aslab) << " " << std::to_string(dslab) << std::endl;
    //std::cout<<"Custom Slab: "<<def.str();
    slabDefinition.push_back(def.str());
    slabList.push_back(label);
}