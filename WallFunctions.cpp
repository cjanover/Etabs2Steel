
//
//  WallFunctions.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"



void Model::newEtabsAreaProp(EtabsAreaProps nProp)
{
    etabsAreaProps[nProp.Name()] = nProp;
}

void Model::newDeckProp(DeckProp nProp)
{
    etabsDeckProps[nProp.Name()] = nProp;
}

void Model::newEtabsWall(EtabsWall nWall)
{
    etabsAreaElements[nWall.Name()] = nWall;
}

void Model::newSteelArea(SteelArea nArea)
{
    //Check to see if the sectionName is a wall or a slab
    
    std::string sectionName = nArea.SectionName();
    
    //Get EtabsConnectivityIterators
    std::vector<unsigned int>::iterator etabsConnectItr;
    std::vector<unsigned int>::iterator etabsConnectFirst = etabsAreaElements[nArea.EtabsName()].WallConnectivityStart();
    std::vector<unsigned int>::iterator etabsConnectSecond = etabsConnectFirst++;
    
    //Set thickness
    nArea.Thickness(etabsAreaProps[nArea.SectionName()].Thickness());
    
    //Set Shear Modulus
    nArea.ShearMod(std::stof(*config.GetRawConfigStart("DefWallShearMod")));
    
    //Get Floor Name and Floor Elevation
    std::string FloorName = nArea.FloorName();
    float curEle = etabsStories[FloorName].Elevation();
    
    if (etabsAreaProps[sectionName].Type() == "Slab")   //Options are Slab or Wall, Deck
    {
        //Since its a slab, just push it onto the list
        nArea.Type("Slab");
        steelSlabElements[steelSlabElements.size()] = nArea;
    }
    else if (etabsDeckProps[sectionName].Type() == "Deck")
    {
        //Set all properties
        nArea.DeckType(etabsDeckProps[sectionName].DeckType());
        nArea.ConcMaterial(etabsDeckProps[sectionName].ConcMaterial());
        nArea.DeckMaterial(etabsDeckProps[sectionName].DeckMaterial());
        nArea.DeckSlabDepth(etabsDeckProps[sectionName].DeckSlabDepth());
        nArea.DeckRibDepth(etabsDeckProps[sectionName].DeckRibDepth());
        nArea.DeckRibWidthTop(etabsDeckProps[sectionName].DeckRibWidthTop());
        nArea.DeckRibWidthBottom(etabsDeckProps[sectionName].DeckRibWidthBottom());
        nArea.DeckRibSpacing(etabsDeckProps[sectionName].DeckRibSpacing());
        nArea.DeckShearThickness(etabsDeckProps[sectionName].DeckShearThickness());
        nArea.DeckUnitWeight(etabsDeckProps[sectionName].DeckUnitWeight());
        nArea.ShearStudDiam(etabsDeckProps[sectionName].ShearStudDiam());
        nArea.ShearStudHeight(etabsDeckProps[sectionName].ShearStudHeight());
        nArea.ShearStudFU(etabsDeckProps[sectionName].ShearStudFU());
        
        nArea.Type("Deck");
        
        //Push Onto steelDeckElements
        steelDeckElements[steelDeckElements.size()] = nArea;
        
    }
    else if (etabsAreaProps[sectionName].Type() == "Wall")
    {
        nArea.Type("Wall");
        //Need to Get the story Below, Loop through all stories until i find the story im on
        
        std::vector<std::string>::reverse_iterator etabsStoryItr = etabsStoriesOrder.rbegin();
        std::string prevStory = *etabsStoriesOrder.rbegin();
        float prevEle = 0.f;
        
        for (etabsStoryItr = etabsStoriesOrder.rbegin()++; etabsStoryItr != etabsStoriesOrder.rend(); etabsStoryItr++)
        {
            std::string curStory = *etabsStoryItr;
            
            if (FloorName == curStory) //Get prevStoryElevation
            {
                prevEle = etabsStories[prevStory].Elevation();
                break;
            }
            prevStory = curStory;
        }
        //Calculate Wall Height
        float height = curEle - prevEle;
        nArea.Height(height);
        
        //Now Get the steel Node Numbers by matching etabs Node Number and Floor.
            //Numbering scheme in Etabs starts at top left node and goes clockwise while looking from the origin
        //Get the X and Y coordinates to see if it runs in the ETabs Primary or Secondary Direction
        float Coord1Prim = etabsNodes[*etabsConnectFirst].Coords(primaryEtabsDirection);
        float Coord2Prim = etabsNodes[*etabsConnectSecond].Coords(primaryEtabsDirection);
        float Coord1Sec = etabsNodes[*etabsConnectFirst].Coords(secondaryEtabsDirection);
        float Coord2Sec = etabsNodes[*etabsConnectSecond].Coords(secondaryEtabsDirection);
    
        //If Coord1Prim != Coord2Prim then it runs in the primary Direction
        Coord::Direction wallDir = secondaryEtabsDirection;
        std::map<unsigned int, SteelNode>* searchList = &secondarySteelNodes;
        if (!isEqual(Coord1Prim,Coord2Prim))
        {
            wallDir = primaryEtabsDirection;
            searchList = &primarySteelNodes;
        }
        
        
        //Put nodes in proper order according to steel
        unsigned int steelOrder_etabsName[4];
        if (wallDir == primaryEtabsDirection)
        {
            //Steel order is starting top left and going clockwise
            if (Coord1Prim < Coord2Prim)
            {
                steelOrder_etabsName[0] = *etabsConnectFirst;
                steelOrder_etabsName[1] = *etabsConnectSecond;
                steelOrder_etabsName[2] = *etabsConnectFirst;
                steelOrder_etabsName[3] = *etabsConnectSecond;
            }
            else
            {
                steelOrder_etabsName[0] = *etabsConnectSecond;
                steelOrder_etabsName[1] = *etabsConnectFirst;
                steelOrder_etabsName[2] = *etabsConnectSecond;
                steelOrder_etabsName[3] = *etabsConnectFirst;
            }
            
            //Calculate Wall Width
            nArea.Width(std::abs(Coord1Prim - Coord2Prim));
        }
        else
        {
            //Steel order is top left, top right, bottom left, bottom right
            if (Coord1Sec < Coord2Sec)
            {
                steelOrder_etabsName[0] = *etabsConnectFirst;
                steelOrder_etabsName[1] = *etabsConnectSecond;
                steelOrder_etabsName[2] = *etabsConnectFirst;
                steelOrder_etabsName[3] = *etabsConnectSecond;
            }
            else
            {
                steelOrder_etabsName[0] = *etabsConnectSecond;
                steelOrder_etabsName[1] = *etabsConnectFirst;
                steelOrder_etabsName[2] = *etabsConnectSecond;
                steelOrder_etabsName[3] = *etabsConnectFirst;
            }
            
            //Calculate Wall Width
            nArea.Width(std::abs(Coord1Sec - Coord2Sec));
        }
        
        //Loop through the list and find the matching node numbers
        unsigned int steelOrder_steelName[4];
        std::map<unsigned int, SteelNode>::iterator nodeItr;
        for (nodeItr = searchList->begin(); nodeItr != searchList->end(); nodeItr++)
        {
            unsigned int curEtabsNum = (nodeItr->second).EtabsNodeNum();
            float curFloor = (nodeItr->second).Coords(Coord::Direction::Z);

            if (curEtabsNum == steelOrder_etabsName[0])
            {
                if (isEqual(curFloor,curEle)) //Then top left node
                {
                    steelOrder_steelName[0] = (nodeItr->second).Num();
                    continue;
                }
                else if (isEqual(curFloor,prevEle)) //Then bottom left node
                {
                    steelOrder_steelName[2] = (nodeItr->second).Num();
                    continue;
                }
            }
            else if (curEtabsNum == steelOrder_etabsName[1])
            {
                if (isEqual(curFloor,curEle)) //Then top right node
                {
                    steelOrder_steelName[1] = (nodeItr->second).Num();
                    continue;
                }
                else if (isEqual(curFloor,prevEle)) //Then bottom right node
                {
                    steelOrder_steelName[3] = (nodeItr->second).Num();
                    continue;
                }
            }
        }
        //Save order
        nArea.AddConnectivity(steelOrder_steelName[0]);
        nArea.AddConnectivity(steelOrder_steelName[1]);
        nArea.AddConnectivity(steelOrder_steelName[2]);
        nArea.AddConnectivity(steelOrder_steelName[3]);
        
        //Push onto list
        steelWallElements[steelWallElements.size()] = nArea;
    
    }
}

void Model::setNBEL()
{
    config.AddRawConfigVal("NBEL", std::to_string(steelWallElements.size()));
    config.AddSaveConfigVal("NBEL", std::to_string(steelWallElements.size()));
}

void Model::CalculateSlabs()
{
    //Go through etabsAreaElements and find all Decks, make list by floor, default to None
    std::map<float, std::string> deckListByFloor;
    
    //Loop through every etabs floor to default to None
    std::map<std::string, Story>::iterator storyItr;
    for (storyItr = etabsStories.begin(); storyItr != etabsStories.end(); storyItr++)
        deckListByFloor[(storyItr->second).Elevation()] = "None";

    //Create deckConversion
    deckConversion["None"] = 111;
    
    //Look through etabsAreaElements
    std::map<unsigned long, SteelArea>::iterator areaItr;
    int counter = 1;
    for (areaItr = steelDeckElements.begin(); areaItr != steelDeckElements.end(); areaItr++)
    {
        //Set the proper floor to the type
        float curEle = etabsStories[(areaItr->second).FloorName()].Elevation();
        
        deckListByFloor[curEle] = (areaItr->second).SectionName();
        deckConversion[(areaItr->second).SectionName()] = 111 + counter;
        counter++;
    }
    
    //Go through Primary Elements
    std::map<unsigned int, SteelLine>::iterator eleItr;
    for (eleItr = primarySteelLines.begin(); eleItr != primarySteelLines.end(); eleItr++)
    {
        //Make sure it is a beam
        if ((eleItr->second).Type() == Element::ElementType::BEAM)
        {
            //Get the Floor
            std::vector<unsigned int> nodes = (eleItr->second).NodeConnectivity();
            
            float ele = primarySteelNodes[nodes[1]].Coords(Coord::Direction::Z);
            
            std::string curDeckType = deckListByFloor[ele];
            
            (eleItr->second).ICS(deckConversion[curDeckType]);
        }
        else
            (eleItr->second).ICS(111);
    }
    
    //Go through Secondary Elements
    for (eleItr = secondarySteelLines.begin(); eleItr != secondarySteelLines.end(); eleItr++)
    {
        //Make sure it is a beam
        if ((eleItr->second).Type() == Element::ElementType::BEAM)
        {
            //Get the Floor
            std::vector<unsigned int> nodes = (eleItr->second).NodeConnectivity();
            
            float ele = secondarySteelNodes[nodes[1]].Coords(Coord::Direction::Z);
            
            std::string curDeckType = deckListByFloor[ele];
            
            (eleItr->second).ICS(deckConversion[curDeckType]);
        }
        else
            (eleItr->second).ICS(111);
    }
}
