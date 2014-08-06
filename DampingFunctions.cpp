//
//  DampingFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/28/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"


void Model::SetDamping()
{
    //Set A1

        float T;

        //Check to see if you need to calculate period
        if (config.GetRawConfigSize("FIRSTMODEPERIOD") == 0)
        {
            //use T = 0.1N
            T = 0.1*etabsStories.size();
        }
        else
            T = std::stof(*config.GetRawConfigStart("FIRSTMODEPERIOD"));
    
        //Convert to rad/sec
        float Wn = 2*3.14159/T;
    
        //Get given stiffness dampign ratio
        float csi_k = std::stof(*config.GetRawConfigStart("DAMPINGRATIOSTIFF"));
    
        //Calculate A1
        float A1 = 2*csi_k/Wn;
    
        //Set in config properties
        config.AddRawConfigVal("A1", std::to_string(A1));
        config.AddSaveConfigVal("A1", std::to_string(A1));
    //Set A2
        //Get Percent Multiplier
        float mult = std::stof(*config.GetRawConfigStart("BASESHEARPERCENT"));
    
        //Get Ratio R
        float R = std::stof(*config.GetRawConfigStart("R"));
    
        //Get Drift
        float Drift = std::stof(*config.GetRawConfigStart("BASEDRIFT"));
    
        //Get Given column damping ratio
        float csi_c = std::stof(*config.GetRawConfigStart("DAMPINGRATIOCOL"));
    
        //Calculate damping
        float C = Wn/(2*csi_c);
    
        //Calculate A2
        float A2 = mult*R*Drift*C;
    
        //Set in config Properties
        config.AddRawConfigVal("A2", std::to_string(A2));
        config.AddSaveConfigVal("A2", std::to_string(A2));
}

void Model::AddSpecialColumns()
{
    d_out dout;
    //First calculate min and max elevations
    CalcMinMaxEle();
    
    
    //Calculate Columns and Shear per floor
    CalcValsPerFloor();

    if (numColsPerFloor.size() == 0)
        numColsPerFloor.push_back(0);


    //Add Special Columns
    std::vector<int>::iterator numColsItr = numColsPerFloor.begin();
    std::vector<float>::iterator shearItr = shearPerFloor.begin();
    std::vector<int>::iterator eleItr = specialColNum.begin();
    

    
    while (true)
    {
        for (int i = 1; i <= *numColsItr; i++)
        {
            SpecialColumn tmpCol;
            
            float numCols = *numColsItr;

            tmpCol.Num(*eleItr);
            tmpCol.PDL1(0.f);
            tmpCol.PDL4(0.1*(*shearItr));
            tmpCol.STRMULT(1/numCols);
            tmpCol.STFMULT(0.f);
            
            //store tmpCol
            specialColumns.push_back(tmpCol);
            if (debugLevel >=1)
                dout<<"Adding special column for element number: "<<*eleItr<<" PDL1 = 0.0f PDL4 = "<<0.1*(*shearItr)<<" STRMULT = "<<1/numCols<<" STFMult = 0.0f\n";
            eleItr++;
        }

        numColsItr++;
        shearItr++;
        
        if (numColsItr == numColsPerFloor.end() || shearItr == shearPerFloor.end())
            break;
    }
    
    //Set NC
    config.AddRawConfigVal("NC", std::to_string(specialColumns.size()));
    config.AddSaveConfigVal("NC", std::to_string(specialColumns.size()));
}



void Model::CalcMinMaxEle()
{
    d_out dout;
    //Loop through flors
    std::map<std::string, Story>::iterator storyItr;

    float minElev = 999999999999999;
    float maxElev = -999999999999999;

    
    for (storyItr = etabsStories.begin(); storyItr != etabsStories.end(); storyItr++)
    {
        float curEle = (storyItr->second).Elevation();
        
        if (curEle > maxElev)
            maxElev = curEle;
        if (curEle < minElev)
            minElev = curEle;
    }
    maxEle = maxElev;
    minEle = minElev;
    
    if (debugLevel >=2)
        dout<<"minElevation = "<<minEle<<" MaxElevation = "<<maxElev<<"\n";
}


float Model::CalcStoryShear(float curEle)
{
    d_out dout;
    //Find Story height of that floor
    float curStoryHeight = 0;
    std::vector<std::string>::iterator searchItr;
    for (searchItr = etabsStoriesOrder.begin(); searchItr != etabsStoriesOrder.end(); searchItr++)
    {
        if (isEqual(etabsStories[*searchItr].Elevation(),curEle))
            break;
        curStoryHeight = etabsStories[*searchItr].Height();
    }

    //Get the Pushover Base Shear
    float BaseShear = std::stof(*config.GetRawConfigStart("BASESHEAR"));
    
    //Calculate shear at top
    float ShearTop = 0.07*BaseShear;

    //Get height of lowest floor
    float lowestFloorHeight = etabsStories[etabsStoriesOrder[etabsStoriesOrder.size()-2]].Height(); //Minus 2 because it needs the floor aboves height

    //Linearly Interpolate to get shear at elevation
    float storyShear = ((BaseShear - ShearTop)*(curEle + curStoryHeight - maxEle))/(minEle + lowestFloorHeight - maxEle) + ShearTop;
    
    if (debugLevel >= 2)
        dout<<"Story Shear for Elevation: "<<curEle<<" = "<<storyShear<<"\n";
    return storyShear;
}



void Model::CalcValsPerFloor()
{
    d_out dout;
    //Loop through each Floor and calculate the number of columns on each floor
    std::map<std::string, Story>::iterator storyItr;
    
    for (storyItr = etabsStories.begin(); storyItr != etabsStories.end(); storyItr++)
    {
        int numCols = 0;
        float curEle = (storyItr->second).Elevation();
        
        //Iterate through primary elements and find columns at this elevation
        std::map<unsigned int, SteelLine>::iterator eleItr;
        
        for (eleItr = primarySteelLines.begin(); eleItr != primarySteelLines.end(); eleItr++)
        {
            //Check if its a column
            if ((eleItr->second).Type() == Element::ElementType::COLUMN)
            {
                //Get element connectivity (second node)
                 std::vector<unsigned int> checkNodes = (eleItr->second).NodeConnectivity();
                
                //Get the elevation of the second node
                float checkEle = primarySteelNodes[checkNodes[1]].Coords(Coord::Direction::Z);
            
                //If the elevations match, count it and store element number
                if (isEqual(curEle,checkEle))
                {
                    specialColNum.push_back((eleItr->second).SteelID());
                    numCols++;
                }
            }
        }
        if (numCols != 0)
        {
            //Store the number of columns on the floor
            numColsPerFloor.push_back(numCols);
        
            //Calculate the shear on the floor
            shearPerFloor.push_back(CalcStoryShear(curEle));
            
            if (debugLevel >=2)
                dout<<"Elevation: "<<curEle<<" has "<<numCols<<" columns.\n";
        }
    }
}
