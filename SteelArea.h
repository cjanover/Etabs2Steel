//
//  SteelArea.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__SteelArea__
#define __Etabs2Steel__SteelArea__

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <map>

#include "Parse.h"

#include "FloatCompare.h"
#include "Debug.h"
extern int debugLevel;
extern std::map <std::string, std::string> PARSESTRINGS;
extern std::map<unsigned int, unsigned int> nodeNumberLookup;

class SteelArea
{
    public:
        SteelArea(){};
    
        //Get Functions
            //Universial Properties
            unsigned int Num(){return num;};
            std::string EtabsName(){return etabsName;};
            std::string FloorName(){return floorName;};
            std::string SectionName(){return sectionName;};
            std::string Type(){return type;};
    
            //Slab & Wall Properties
            std::vector<unsigned int>::iterator WallConnectivityStart(){return steelNodes.begin();};
            std::vector<unsigned int>::iterator WallConnectivityEnd(){return steelNodes.end();};
    
            float Width(){return width;};
            float Height(){return height;};
            float Thickness(){return thickness;};
    
            float ShearMod(){return shearMod;};
    
            //Deck Properties
    
            std::string DeckType(){return deckType;};
            std::string ConcMaterial(){return concMaterial;};
            std::string DeckMaterial(){return deckMaterial;};

            float DeckSlabDepth(){return deckSlabDepth;};
            float DeckRibDepth(){return deckRibDepth;};
            float DeckRibWidthTop(){return deckRibWidthTop;};
            float DeckRibWidthBottom(){return deckRibWidthBottom;};
            float DeckRibSpacing(){return deckRibSpacing;};
            float DeckShearThickness(){return deckShearThickness;};
            float DeckUnitWeight(){return deckUnitWeight;};
            float ShearStudDiam(){return shearStudDiam;};
            float ShearStudHeight(){return shearStudHeight;};
            float ShearStudFU(){return shearStudFU;};
    
        //Set Functions
            //Universial Properties
            void Num(const unsigned int nNum){num = nNum;};
            void EtabsName(const std::string nEtabsName){etabsName = nEtabsName;};
            void FloorName(const std::string nFloorName){floorName = nFloorName;};
            void SectionName(const std::string nSectionName){sectionName = nSectionName;};
            void Type(const std::string nType){type = nType;};

            //Slab & Wall Properties
            void AddConnectivity(unsigned int nNode){steelNodes.push_back(nNode);};
    
            void Width(const float nWidth){width = nWidth;};
            void Height(const float nHeight){height = nHeight;};
            void Thickness(const float nThickness){thickness = nThickness;};
    
            void ShearMod(const float nShearMod){shearMod = nShearMod;};
    
            const std::string ParserString(){return parseString;};
    
            //Deck Properties
            void DeckType(const std::string nDeckType){deckType = nDeckType;};
            void ConcMaterial(const std::string nDeckType){concMaterial = nDeckType;};
            void DeckMaterial(const std::string nDeckType){deckMaterial = nDeckType;};
    
            void DeckSlabDepth(const float nDeckSlabDepth){deckSlabDepth = nDeckSlabDepth;};
            void DeckRibDepth(const float nDeckRibDepth){deckRibDepth = nDeckRibDepth;};
            void DeckRibWidthTop(const float nDeckRibWidthTop){deckRibWidthTop = nDeckRibWidthTop;};
            void DeckRibWidthBottom(const float nDeckRibWidthBottom){deckRibWidthBottom = nDeckRibWidthBottom;};
            void DeckRibSpacing(const float nDeckRibSpacing){deckRibSpacing = nDeckRibSpacing;};
            void DeckShearThickness(const float nDeckShearThickness){deckShearThickness = nDeckShearThickness;};
            void DeckUnitWeight(const float nDeckUnitWeight){deckUnitWeight = nDeckUnitWeight;};
            void ShearStudDiam(const float nShearStudDiam){shearStudDiam = nShearStudDiam;};
            void ShearStudHeight(const float nShearStudHeight){shearStudHeight = nShearStudHeight;};
            void ShearStudFU(const float nShearStudFU){shearStudFU = nShearStudFU;};
    
    private:
        //Universial Properties
        unsigned int num;
        std::string etabsName;
        std::string floorName;
        std::string sectionName;
        std::string type;
    
        //Slab & Wall Properties
        float width = 0.f;
        float height = 0.f;
        float thickness = 0.f;
    
        float shearMod = 0.f;
    
        std::vector<unsigned int> steelNodes;


        //Deck Properties
        std::string deckType;
        std::string concMaterial;
        std::string deckMaterial;
        float deckSlabDepth;
        float deckRibDepth;
        float deckRibWidthTop;
        float deckRibWidthBottom;
        float deckRibSpacing;
        float deckShearThickness;
        float deckUnitWeight;
        float shearStudDiam;
        float shearStudHeight;
        float shearStudFU;
    
        std::string parseString = PARSESTRINGS["SteelArea"];


};

//Operator Overloads
std::istream& operator>>(std::istream& is, SteelArea& area);
std::ostream& operator<<(std::ostream& os, SteelArea& area);
#endif /* defined(__Etabs2Steel__SteelArea__) */
