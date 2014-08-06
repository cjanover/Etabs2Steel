//
//  SectionFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/8/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"


void Model::newCustomSection(const std::string label, const float D, const float B, const float TF, const float TW, const std::string material)
{
    steelSection.AddSectionDefinition(label, D, TW, B, TF);
    
    //Get Material Conv
    newRegSection(label, material);
}

void Model::newRegSection(const std::string label, const std::string material)
{
    d_out dout;
    //Loop through conversion database for material
    std::vector<std::string>::iterator matItr;
    int i = 0;
  //  for (matItr = config.GetRawConfigStart("MATERIALCONV"); matItr != config.GetRawConfigEnd("MATERIALCONV"); matItr++)
    matItr = config.GetRawConfigStart("MATERIALCONV");
    while (i <  config.GetRawConfigSize("MATERIALCONV"))
    {
        std::string matName;
        unsigned int matConv;

        std::istringstream line(*matItr);
        
        while (line >> matName >> matConv)
        {
            if (material == matName)
            {
                steelSection.AddMaterialConversion(label, matConv);
                return;
            }
            
        }
        i++;
        matItr++;
        
    }
    
    dout<<"No Etabs to Steel section conversion found for material: "<<material<<". Terminating Analysis.\n";
    exit(0);


}


void Model::newWideFlange(const std::string label, const float D, const float B, const float TF, float TW)
{
    steelSection.AddSectionDefinition(label, D, TW, B, TF);
}

void Model::CreateSlabDatabase()
{
    //Create basic slab for now
    

}

