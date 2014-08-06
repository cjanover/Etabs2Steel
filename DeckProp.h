//
//  DeckProp.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 6/3/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__DeckProp__
#define __Etabs2Steel__DeckProp__

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
class DeckProp
{
    public:
        DeckProp(){};
    
        //Get Functions
        std::string Name(){return name;};
        std::string Type(){return type;};
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
        void Name(const std::string nName){name = nName;};
        void Type(const std::string nType){type = nType;};
    
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

    
    
        const std::string ParserStringWall(){return parseString;};

    
    private:
        std::string name = "111";
        std::string type;
        std::string deckType;
        std::string concMaterial;
        std::string deckMaterial;
        float deckSlabDepth = 0.f;
        float deckRibDepth = 0.f;
        float deckRibWidthTop = 0.f;
        float deckRibWidthBottom = 0.f;
        float deckRibSpacing = 0.f;
        float deckShearThickness = 0.f;
        float deckUnitWeight = 0.f;
        float shearStudDiam = 0.f;
        float shearStudHeight = 0.f;
        float shearStudFU = 0.f;
    
    
        std::string parseString = PARSESTRINGS["EtabsDeckProp"];


};

//Operator Overloads
std::istream& operator>>(std::istream& is, DeckProp& props);
#endif /* defined(__Etabs2Steel__DeckProp__) */
