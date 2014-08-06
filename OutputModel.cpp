//
//  OutputModel.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/26/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"

void Model::RunModel()
{
    //Create for001 file
    CreateSteelInput();
}

void Model::PrepareModel()
{
    //Renumber Nodes
    RenumberNodes();
    //ReorderNodes();

    //Setup Basic Response Time Histories
    SetResponseTH();
    
    //Prepare Config Info
    PrepareConfig();
    
    //Set NBEL
    setNBEL();

    //Set Steel Nodes Fixity
    SetSteelNodeFixity();
    
    //Set Damping
    SetDamping();
    
    //Calculate nodal Forces & Masses
    CalculateNodalForces();
    CalculateNodalMass();
    
    //Determine Vertical Connection Elements
    CalculateNVCONEL();
    
    //Determine Diaphragm
    CalculateDiaphragm();
    
    //Set Slabs
    CalculateSlabs();
    
    //Add vertical rollers to the left most node in all secondary floors
    AddRollerSecondary();
    
    //Add Special Columns
    AddSpecialColumns();
    
    //Set Foundation Node Numbers
    AddFoundationNodes();
    

    
    //Calculate max frame width then shift nodes
    //CalculateNumNodesPerFloor();
    findMaxFrameWidth();
    CalculateNodalShift();
    SteelCoordSwitch();
    
    //Output Grid Shift
    OutputGridShiftConversion();
    
    //Create Steel Input Files
    CreateOtherSteelInput();
}

void Model::CreateSteelInput()
{
    //Get Save Path for steel files
    std::string saveLoc = *config.GetRawConfigStart("SAVELOC");
    
    //Get Title
    std::string title = *config.GetRawConfigStart("TITLE");
    
    std::ostringstream savePath;

    //for001
    std::ofstream out001;
    savePath<<saveLoc << "/" << title;
    out001.open(savePath.str());
    out001 << *this;
    out001.close();
}

void Model::CreateOtherSteelInput()
{
    //Get Title
    std::string title = *config.GetRawConfigStart("TITLE");
 

    //Get Save Path for steel files
    std::string saveLoc = *config.GetRawConfigStart("SAVELOC");
    
    std::ostringstream savePath;
    std::ostringstream copyCommand;
    
    
    
    
    //for020
    std::ofstream out020;
    
    savePath.str(std::string());
    savePath << saveLoc << "/" << title << " - Sections";

    out020.open(savePath.str());
    out020 << steelSection;
    out020.close();
    
    //for021
    std::ofstream out021;
    
    savePath.str(std::string());
    savePath << saveLoc << "/" << title << " - Slabs";
    out021.open(savePath.str());
    

    
    std::map<std::string, unsigned int>::iterator deckItr;
    int counter = 1;
    

    for (deckItr = deckConversion.begin(); deckItr != deckConversion.end(); deckItr++)
    {
        DeckProp curDeck = etabsDeckProps[(deckItr->first)];
        float dslab = curDeck.DeckSlabDepth()/2+curDeck.DeckRibDepth();
        float ddeck = curDeck.DeckRibDepth()/2;
        float adeck = 0;
        float aslab = 16*curDeck.DeckSlabDepth()*curDeck.DeckSlabDepth();
        
        unsigned int title;
        if (curDeck.Name() == "111")
            title = 111;
        else
            title = deckConversion[curDeck.Name()];
        
        out021 << title << " " << adeck << " " << ddeck << " " <<aslab << " " << dslab << std::endl;
        counter++;
    }
    out021.close();
    
    //Section Conversion Table
    if (*config.GetRawConfigStart("SECTIONCONVERSION") == "yes")
    {
        std::ofstream sectionConversion;
    
        savePath.str(std::string());
        savePath << saveLoc << "/" << "Section Conversion";
    
        sectionConversion.open(savePath.str());
        // Loop through section conversion data
        std::map<std::string, unsigned long>::iterator convItr;
        
        for (convItr = steelSection.GetConversionStart(); convItr != steelSection.GetConversionEnd(); convItr ++)
            sectionConversion << convItr->second << " " << convItr->first << std::endl;
        sectionConversion.close();
    }
    
    //Material Conversion Table
    if (*config.GetRawConfigStart("MATCONVERSION") == "yes")
    {
        std::ofstream matConversion;
        
        savePath.str(std::string());
        savePath << saveLoc << "/" << "Material Conversion";
        
        matConversion.open(savePath.str());
        //Loop through material conversion data
        std::map<std::string, unsigned int>::iterator convItr;
        
        for (convItr = steelSection.GetMaterialConversionStart(); convItr != steelSection.GetMaterialConversionEnd(); convItr++)
            matConversion << std::to_string(convItr->second) << " " << convItr->first << std::endl;
        matConversion.close();
    }
}


void Model::findMaxFrameWidth()
{
    d_out dout;
    //Iterate through grid list
    std::map<unsigned long, Grid>::iterator iter;
    

    for (iter = etabsGrids.begin(); iter != etabsGrids.end(); iter++)
    {
        if (abs((iter->second).Coordinate()) > maxFrameWidth)
            maxFrameWidth = abs((iter->second).Coordinate());
    }
    
    if (debugLevel >= 1)
        dout<<"Max Frame Width = "<<maxFrameWidth<<"\n";
}

void Model::CalculateNodalShift()
{
    d_out dout;
    //Look through node lists
    std::map<unsigned int, SteelNode>::iterator nodeIter;
    
    for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
    {
    
        //Default to zero nodal shift
        (nodeIter->second).NodalShift(0.f);
        
        //Find matching coordinate in the secondary direction, be careful because SteelNode lists and grid directions arent necessarily the same sense
        float nodeCoord = (nodeIter->second).Coords(Coord::Direction::Y);
        
        //Look through grid list
        std::map<unsigned long, Grid>::iterator gridIter;

        int gridCounter = 0;
        for (gridIter = etabsGrids.begin(); gridIter != etabsGrids.end(); gridIter++)
        {
            Coord::Direction gridDirection = (gridIter->second).EtabsGridDirection();
            float gridCoord = (gridIter->second).Coordinate();
            
            //Only a match if the directions and coord match finding closest match rounding down, separate out the if statements so i only increment the grid counter when looking at the proper grids, otherwise
            //  end up shifting too much
            
            if (GetEtabsSecondaryDirection(gridDirection) == primarySteelDirection)
            {
                if (isEqual(gridCoord,nodeCoord))
                {
                    //If a match, then shift the node by the gridCounter*(maxFrameWidth + Buffer)
                    (nodeIter->second).NodalShift(gridCounter*(maxFrameWidth + frameWidthBuffer));
            
                    if (debugLevel >= 3)
                        dout<<"Shifting Primary Node: "<<(nodeIter->second).Num()<<" by "<<(nodeIter->second).NodalShift()<<"\n";
                    //get out of the grid loop
                    break;
                }
                   //Increment counter
            }
            gridCounter += 1;
        }
    }
    
    
    for (nodeIter = secondarySteelNodes.begin(); nodeIter != secondarySteelNodes.end(); nodeIter++)
    {
        //Find matching coordinate in the secondary direction, be careful because SteelNode lists and grid directions arent necessarily the same
        //Also remember that steel nodes are always X - Y
        float nodeCoord = (nodeIter->second).Coords(Coord::Direction::X);
        
        
        //Look through grid list
        std::map<unsigned long, Grid>::iterator gridIter;

        int gridCounter = 0;
        for (gridIter = etabsGrids.begin(); gridIter != etabsGrids.end(); gridIter++)
        {
            Coord::Direction steelGridDirection = GetEquivSteelDirection((gridIter->second).EtabsGridDirection());
            float gridCoord = (gridIter->second).Coordinate();
            
            //Only a match if the directions and coords match, separate out the if statements so i only increment the grid counter when looking at the proper grids, otherwise
            //  end up shifting too much
            if (steelGridDirection == primarySteelDirection)
            {
                if (isEqual(gridCoord,nodeCoord))
                {
                    //If a match, then shift the node in the X direction by the gridCounter*(maxFrameWidth + Buffer)
                    (nodeIter->second).NodalShift(gridCounter*(maxFrameWidth + frameWidthBuffer));

                    if (debugLevel >= 3)
                        dout<<"Shifting Secondary Node: "<<(nodeIter->second).Num()<<" by "<<(nodeIter->second).NodalShift()<<"\n";
                    
                    //get out of the grid loop
                    break;
                }
                
            }
            gridCounter += 1;   //Increment counter
        }
    }

}

void Model::SteelCoordSwitch()
{
    d_out dout;
    //Loop through all nodes
    std::map<unsigned int, SteelNode>::iterator nodeIter;
    
    //Primary Nodes
    for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
    {    
        float steelPrim = (nodeIter->second).Coords(primaryEtabsDirection);
        float steelSec = (nodeIter->second).Coords(secondaryEtabsDirection);
        float steelVer = (nodeIter->second).Coords(Coord::Direction::Z);
        
        //Reorient them
        (nodeIter->second).Coords(steelPrim + (nodeIter->second).NodalShift(), steelVer, steelSec);
        
        //Reorient Forces

        float primForce = (nodeIter->second).TotForce(primaryEtabsDirection);
        (nodeIter->second).TotForce(primForce, Coord::Direction::X);

        float verForce = (nodeIter->second).TotForce(Coord::Direction::Z);
        (nodeIter->second).TotForce(verForce, Coord::Direction::Y);
        
        //Reorient Mass
        float primMass = (nodeIter->second).TotMass(primaryEtabsDirection);
        (nodeIter->second).TotMass(primMass, Coord::Direction::X);

        float verMass = (nodeIter->second).TotMass(Coord::Direction::Z);
        (nodeIter->second).TotMass(verMass, Coord::Direction::Y);
        
        if (debugLevel >= 3)
            dout<<"Primary Node: "<<(nodeIter->second).Num()<<"'s Coordinates went from ("<<steelPrim<<", "<<steelSec<<", "<<steelVer<<") to ("<<steelPrim + (nodeIter->second).NodalShift()<<", "<<steelVer<<", "<<steelSec<<")\n";
        
    }
    
    for (nodeIter = secondarySteelNodes.begin(); nodeIter != secondarySteelNodes.end(); nodeIter++)
    {
        float steelPrim = (nodeIter->second).Coords(secondaryEtabsDirection);
        float steelSec = (nodeIter->second).Coords(primaryEtabsDirection);
        float steelVer = (nodeIter->second).Coords(Coord::Direction::Z);
    
        //Reorient Coords
        (nodeIter->second).Coords(steelPrim + (nodeIter->second).NodalShift(), steelVer, steelSec);
        
        //Reorient Forces
        float primForce = (nodeIter->second).TotForce(secondaryEtabsDirection);
        (nodeIter->second).TotForce(primForce, Coord::Direction::X);

        float verForce = (nodeIter->second).TotForce(Coord::Direction::Z);
        (nodeIter->second).TotForce(verForce, Coord::Direction::Y);
        
        //Reorient Mass
        float primMass = (nodeIter->second).TotMass(secondaryEtabsDirection);
        (nodeIter->second).TotMass(primMass, Coord::Direction::X);

        float verMass = (nodeIter->second).TotMass(Coord::Direction::Z);
        (nodeIter->second).TotMass(verMass, Coord::Direction::Y);
        
        if (debugLevel >= 3)
            dout<<"Secondary Node: "<<(nodeIter->second).Num()<<"'s Coordinates went from ("<<steelPrim<<", "<<steelSec<<", "<<steelVer<<") to ("<<steelPrim + (nodeIter->second).NodalShift()<<", "<<steelVer<<", "<<steelSec<<")\n";
    }
    
    
    
    

}

std::ostringstream Model::OutputNodes()
{
    std::ostringstream outString;
    //Iterate through all Steel Nodes
    std::map<unsigned int, SteelNode>::iterator nodeIter;
    for (nodeIter = primarySteelNodes.begin(); nodeIter != primarySteelNodes.end(); nodeIter++)
        outString << (nodeIter->second);
    for (nodeIter = secondarySteelNodes.begin(); nodeIter != secondarySteelNodes.end(); nodeIter++)
        outString << (nodeIter->second);
    
    return outString;
}

std::ostringstream Model::OutputLineElements()
{
    std::ostringstream outString;
    
    //Iterate through all line elements
    std::map<unsigned int, SteelLine>::iterator lineIter;
    for (lineIter = primarySteelLines.begin(); lineIter != primarySteelLines.end(); lineIter++)
        outString << (lineIter->second);
    for (lineIter = secondarySteelLines.begin(); lineIter != secondarySteelLines.end(); lineIter++)
        outString << (lineIter->second);

    return outString;
}

std::ostringstream Model::OutputSpecialColumns()
{
    std::ostringstream outString;

    //Iterate through all special columns
    std::vector<SpecialColumn>::iterator colItr;
    
    for (colItr = specialColumns.begin(); colItr != specialColumns.end(); colItr++)
    {
        //Output the column
        outString << *colItr;
    }

    return outString;
}

std::ostringstream Model::OutputBasementWallElements()
{
    std::ostringstream outString;
    
    //Iterate through all wall elements
    std::map<unsigned long, SteelArea>::iterator areaItr;
    
    int counter = 1;
    for (areaItr = steelWallElements.begin(); areaItr != steelWallElements.end(); areaItr++)
    {
        //Set Num
        (areaItr->second).Num(counter);
        //Output the wall
        outString << (areaItr->second);
        counter++;
    }
    
    return outString;
}


std::ostringstream Model::OutputFiberSectionMod()
{
    std::ostringstream outString;
    
    //Columns
        //Strong Axis
            outString << "1 8 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl; //Fix - Fix
            outString << "1 1 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;

            outString << "2 8 1 2 7 8 0 0 0 0 0 0 0.0"<<std::endl; //Pin - Fix
            outString << "2 8 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
            outString << "2 1 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
    
            outString << "3 8 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl; //Fix - Pin
            outString << "3 1 1 2 7 8 0 0 0 0 0 0 0.0"<<std::endl;
            outString << "3 1 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
    
            outString << "4 8 1 2 7 8 0 0 0 0 0 0 0.0"<<std::endl; //Pin - Pin
            outString << "4 8 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
            outString << "4 1 1 2 7 8 0 0 0 0 0 0 0.0"<<std::endl;
            outString << "4 1 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
        //Weak Axis
            outString << "5 8 5 6 0 0 0 0 0 0 0 0 0.3"<<std::endl; //Fix - Fix
            outString << "5 1 5 6 0 0 0 0 0 0 0 0 0.3"<<std::endl;
    
            outString << "6 8 1 2 3 4 7 8 9 10 0 0 0.0"<<std::endl; //Pin - Fix
            outString << "6 8 5 6 0 0 0 0 0 0  0 0 0.3"<<std::endl;
            outString << "6 1 5 6 0 0 0 0 0 0  0 0 0.3"<<std::endl;
    
            outString << "7 8 5 6 0 0 0 0 0 0  0 0 0.3"<<std::endl; //Fix - Pin
            outString << "7 1 1 2 3 4 7 8 9 10 0 0 0.0"<<std::endl;
            outString << "7 1 5 6 0 0 0 0 0 0  0 0 0.3"<<std::endl;
    
            outString << "8 8 1 2 3 4 7 8 9 10 0 0 0.0"<<std::endl; //Pin - Pin
            outString << "8 8 5 6 0 0 0 0 0 0  0 0 0.3"<<std::endl;
            outString << "8 1 1 2 3 4 7 8 9 10 0 0 0.0"<<std::endl;
            outString << "8 1 5 6 0 0 0 0 0 0  0 0 0.3"<<std::endl;
    //Beams

        //Strong Axis
            outString << "9 8 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl; //Fix - Fix
            outString << "9 7 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
            outString << "9 1 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;
            outString << "9 2 3 4 5 6 0 0 0 0 0 0 0.3"<<std::endl;

            outString <<"10 8 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl; //Pin - Fix
            outString <<"10 7 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl;
            outString <<"10 8 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
            outString <<"10 7 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
    
            outString <<"11 1 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl; //Fix - Pin
            outString <<"11 2 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl;
            outString <<"11 1 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
            outString <<"11 2 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
    
            outString <<"12 8 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl; //Pin - Pin
            outString <<"12 7 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl;
            outString <<"12 8 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
            outString <<"12 7 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
            outString <<"12 1 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl;
            outString <<"12 2 1 2 7 8 3 6 0 0 0 0 0.0"<<std::endl;
            outString <<"12 1 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;
            outString <<"12 2 4 5 0 0 0 0 0 0 0 0 6"<<std::endl;

    
    //Zeros
            outString <<"0  0 0 0 0 0 0 0 0 0 0 0 0"<<std::endl;
    
    
    return outString;
}

std::ostringstream Model::OutputFiberStrainSectionMod()
{
    std::ostringstream outString;
    
    //Beams
        //Strong Axis
            //outString << "10 8 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl; //Pin - Fix
            //outString << "10 7 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl;
    
            //outString << "11 1 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl; //Fix - Pin
            //outString << "11 2 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl;
    
            //outString << "12 1 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl; //Pin - Pin
            //outString << "12 2 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl;
            //outString << "12 8 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl; //Pin - Fix
            //outString << "12 7 4 5 0 0 0 0 0 0 0 0 100 100 100 100 100 100 100 100 100 100"<<std::endl;
    //Zeros
            outString << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"<<std::endl;
    
    return outString;
}

std::ostringstream Model::OutputConfig()
{
    std::ostringstream outString;
   
    outString << config;
    
    return outString;
}

std::ostringstream Model::OutputSegmentLengths()
{
    std::ostringstream outString;
    
    //Get start of iterator
    std::vector<std::string>::iterator keyItr;
    
    //Beam&Col Seg Lengths
    for (keyItr = config.GetConfigStart("FRAC-BC"); keyItr != config.GetConfigEnd("FRAC-BC"); keyItr++)
        outString << *keyItr<<std::endl;

    
    outString <<std::endl<<std::endl;
    
    //Brace Seg Lengths
    for (keyItr = config.GetConfigStart("FRAC-BR"); keyItr != config.GetConfigEnd("FRAC-BR"); keyItr++)
        outString << *keyItr<<std::endl;


    return outString;
}

std::ostringstream Model::OutputGAMULT()
{
    std::ostringstream outString;
    //Check to see if GAMULT was specified in Config
    if (config.GetRawConfigSize("GAMULT") == 0)
        outString << std::to_string(0); //Use Etabs Value
    else
        outString << *config.GetRawConfigStart("GAMULT");
    

    return outString;
}


std::ostringstream Model::OutputNNPBF()
{
    std::ostringstream os;
    
    //Loop through NNPBF Data
    std::vector<std::vector<int>>::iterator mcc1Itr = mcc1.begin();
    std::vector<std::vector<int>>::iterator mcc2Itr = mcc2.begin();
    
    int counter = 1;
    while (mcc1Itr != mcc1.end())
    {

        //Loop through each MCC information
        std::vector<int>::iterator curMCC1Itr = (*mcc1Itr).begin();
        std::vector<int>::iterator curMCC2Itr = (*mcc2Itr).begin();

        os << counter << " ";
        while (curMCC1Itr != (*mcc1Itr).end())
        {
            int num = nodeNumberLookup[*curMCC1Itr];
            os << num << " ";
            curMCC1Itr++;

        }
        while (curMCC2Itr != (*mcc2Itr).end())
        {
            int num = nodeNumberLookup[*curMCC2Itr];
            os << num << " ";
            curMCC2Itr++;
        }
        os << std::endl;
        
        counter++;
        mcc1Itr++;
        mcc2Itr++;
    }
    
    return os;
}


std::ostringstream Model::OutputNVCONEL()
{
    std::ostringstream os;
    
    //Loop through NVCONEL Data
    std::vector<std::string>::iterator nvItr;
    
    for (nvItr = nvconelList.begin(); nvItr != nvconelList.end(); nvItr++)
    {
        //write line
        os << *nvItr;
    }
    return os;
}


std::ostringstream Model::OutputTH()
{
    std::ostringstream os;
    
    //Loop through TimeHistory Data
    std::vector<std::string>::iterator THItr;
    
    for (THItr = config.GetConfigStart("TimeHistory"); THItr != config.GetConfigEnd("TimeHistory"); THItr++)
        os << *THItr;

    return os;
}

std::ostringstream Model::OutputMaterial()
{
    std::ostringstream os;
    
    //Output Steel Material Data
    os << *config.GetConfigStart("SteelMat")<<std::endl;
    
    //Loop through material data
    std::vector<std::string>::iterator MatItr;
    for (MatItr = config.GetConfigStart("MAT"); MatItr != config.GetConfigEnd("MAT"); MatItr++)
        os << *MatItr << std::endl;
    
    //Output Concrete Material Data
    os << *config.GetConfigStart("ConcreteMat")<<std::endl;

    return os;
}

void Model::OutputGridShiftConversion()
{
    d_out dout;
    std::ofstream gridConversion;
    
    
    std::string saveLoc = *config.GetRawConfigStart("SAVELOC");
    
    std::ostringstream savePath;
    savePath.str(std::string());
    savePath << saveLoc << "/" << "GridShiftConversion.txt";

    gridConversion.open(savePath.str());


    //Loop through grids
    std::map<unsigned long, Grid>::iterator gridItr;
    
    int counter = 0;
    for (gridItr = etabsGrids.begin(); gridItr != etabsGrids.end(); gridItr++)
    {
        //for each grid calculate the shift
        float gridShift = counter*(maxFrameWidth + frameWidthBuffer);
        float shiftGridCoord = gridShift;// + (gridItr->second).Coordinate();
        //Write to file
        if (debugLevel >= 2)
        {
            dout<< "Grid Name: " << (gridItr->second).GridName() << ". Constant Direction: " << Coord::FindDirString((gridItr->second).EtabsGridDirection()) << ". Etabs Coordinate: " << (gridItr->second).Coordinate() << ". Steel Coordinate: " << shiftGridCoord << "\n";
            dout<< "Grid Name: " << (gridItr->second).GridName() << ". Constant Direction: " << Coord::FindDirString((gridItr->second).EtabsGridDirection()) << ". Etabs Coordinate: " << (gridItr->second).Coordinate() << ". Steel Coordinate: " << shiftGridCoord << "\n";
        }
        counter++;
    }
    gridConversion.close();
}


void Model::RenumberNodes()
{
    d_out dout;
    std::map<unsigned int, EtabsNode> revisedEtabsNodes;
    //If the primary direction is X, reorder the etabsnodes
    if (primaryEtabsDirection == Coord::Direction::X)
    {
        std::map<unsigned int, EtabsNode>::iterator nodeItr;
        std::vector<float> yList;
        //Make a list of each Y
        for (nodeItr = etabsNodes.begin(); nodeItr != etabsNodes.end(); nodeItr++)
        {
            float curYCoord = (nodeItr->second).Coords(Coord::Direction::Y);
            if (std::find(yList.begin(), yList.end(), curYCoord)== yList.end())
                yList.push_back(curYCoord);
        }
        //Sort the list
        std::sort (yList.begin(), yList.end());
        
        std::vector<float> xList;
        //Make a list of each X
        for (nodeItr = etabsNodes.begin(); nodeItr != etabsNodes.end(); nodeItr++)
        {
            float curXCoord = (nodeItr->second).Coords(Coord::Direction::X);
            if (std::find(xList.begin(), xList.end(), curXCoord)== xList.end())
                xList.push_back(curXCoord);
        }
        //Sort the list
        std::sort (xList.begin(), xList.end());
  
  
        
        //Now put etabsNodes in new order
        //loop through Y direction
        std::vector<float>::iterator yItr;
        int counter = 0;
        for (yItr = yList.begin(); yItr != yList.end(); yItr++)
        {
            float curY = *yItr;
            
            //Loop through X direction
            std::vector<float>::iterator xItr;
            for (xItr = xList.begin(); xItr != xList.end(); xItr++)
            {
                float curX = *xItr;
                
                //Now loop through all etabs Nodes
                for (nodeItr = etabsNodes.begin(); nodeItr != etabsNodes.end(); nodeItr++)
                {
                    float checkY = (nodeItr->second).Coords(Coord::Direction::Y);
                    float checkX = (nodeItr->second).Coords(Coord::Direction::X);
                    
                    if (isEqual(checkY,curY) && isEqual(checkX,curX)) //Next in list
                    {
                        revisedEtabsNodes[counter] = (nodeItr->second);
                        counter++;
                    }
                }
            }
        }
    }
    else
    {
        revisedEtabsNodes = etabsNodes;
    }
    
    
    //Loop through each floor
    std::vector<std::string>::reverse_iterator storyItr;
    int counter = 1;
    for (storyItr = etabsStoriesOrder.rbegin(); storyItr != etabsStoriesOrder.rend(); storyItr++)
    {
        float curEle = etabsStories[*storyItr].Elevation();
        
        //go through each etabs node for primary direction
        std::map<unsigned int, EtabsNode>::iterator etabsNodeItr;
        for (etabsNodeItr = revisedEtabsNodes.begin(); etabsNodeItr != revisedEtabsNodes.end(); etabsNodeItr++)
        {
            unsigned int curEtabsNode = (etabsNodeItr->second).Num();
        
            //go through primary nodes
            std::map<unsigned int, SteelNode>::iterator nodeItr;
            for (nodeItr = primarySteelNodes.begin(); nodeItr != primarySteelNodes.end(); nodeItr++)
            {
                float checkEle = (nodeItr->second).Coords(Coord::Direction::Z);
                unsigned int checkEtabsNode = (nodeItr->second).EtabsNodeNum();
                
                if (isEqual(curEle,checkEle) && curEtabsNode == checkEtabsNode) //Then Save it
                {
                    nodeNumberLookup[(nodeItr->second).Num()] = counter;
                    
                    if (debugLevel >= 3)
                        dout<<"Primary Node: "<<(nodeItr->second).Num()<<" Renumbered to "<<counter<<"\n";
                    counter++;
                    break;
                }
            }
        }
        //go through each etabs node for secondary direction
        for (etabsNodeItr = revisedEtabsNodes.begin(); etabsNodeItr != revisedEtabsNodes.end(); etabsNodeItr++)
        {
            unsigned int curEtabsNode = (etabsNodeItr->second).Num();
        
            //go through secondary nodes
            std::map<unsigned int, SteelNode>::iterator nodeItr;
            for (nodeItr = secondarySteelNodes.begin(); nodeItr != secondarySteelNodes.end(); nodeItr++)
            {
                float checkEle = (nodeItr->second).Coords(Coord::Direction::Z);
                unsigned int checkEtabsNode = (nodeItr->second).EtabsNodeNum();
                
                if (isEqual(curEle,checkEle) && curEtabsNode == checkEtabsNode) //Then Save it
                {
                    nodeNumberLookup[(nodeItr->second).Num()] = counter;
                    
                    if (debugLevel >= 3)
                        dout<<"Secondary Node: "<<(nodeItr->second).Num()<<" Renumbered to "<<counter<<"\n";
                    counter++;
                    break;
                }
            }
        }
    }
}

