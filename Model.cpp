//
//  Model.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"

void Model::ReadInput()
{
    d_out dout;
    //Get Name and location of Etabs File
    std::string etabsName = *config.GetRawConfigStart("ETABSTITLE");
    std::string etabsLoc = *config.GetRawConfigStart("ETABSLOC");
    
    //Create Path
    std::ostringstream etabsPath;
    etabsPath << etabsLoc << "/" << etabsName;
    
    //Open File
    std::ifstream etabsInput(etabsPath.str());
    
    if (!etabsInput)
    {
        dout<<"Cannot Open Input File: "<<etabsPath.str()<<". Terminating Analysis.\n";
        exit(0);
    }
    etabsInput.exceptions(etabsInput.exceptions()|std::ios_base::badbit);
    
    //Parse the input file
    std::string line;
    while (getline(etabsInput, line))
        GetSStream(line)>>*this;
}

void Model::newEtabsStory(Story nStory)
{
    //Save story
    etabsStories[nStory.Name()] = nStory;
    
    //Push onto order list
    etabsStoriesOrder.push_back(nStory.Name());
    
    //if story has a height of 0 update all elevations
    if (nStory.ElvType() == "ELEV")
    {
//        std::map<std::string, Story>::iterator storyIterator = etabsStories.begin();
        std::vector<std::string>::reverse_iterator storyIterator = etabsStoriesOrder.rbegin();
        storyIterator++;
        float elevation = nStory.Height();
        etabsStories[nStory.Name()].Elevation(0);

        
        for (; storyIterator!=etabsStoriesOrder.rend(); storyIterator++)
        {
            //Set the elevation
            etabsStories[*storyIterator].Elevation(elevation);
            
            //Increment elevation
            elevation += etabsStories[*storyIterator].Height();

            
            //std::cout<<storyIterator->first<<" -> "<<(storyIterator->second).Elevation()<<std::endl;
        
        }
    }
}

void Model::newEtabsGrid(Grid nGrid)
{
    etabsGrids[etabsGrids.size()] = nGrid;

}

void Model::newSteelNode(SteelNode nNode)
{
}

void Model::newEtabsLine(EtabsLine nLine)
{
    etabsLines[nLine.EtabsID()] = nLine;
}

void Model::newSteelLine(SteelLine nLine)
{
    d_out dout;
    //Check validity of section name
    if (std::find(steelSection.GetSectionListStart(), steelSection.GetSectionListEnd(), nLine.SectionName()) == steelSection.GetSectionListEnd())
    {
        dout<<"Element attempting to use invalid section: "<<nLine.SectionName()<< ". Stopping Execution.\n";
        exit(0);
    }

    //Set the element type for nLine
    nLine.Type(etabsLines[nLine.EtabsRefID()].Type());
    
    //Set Section ID
    nLine.SteelSectionID(steelSection.GetConversion(nLine.SectionName()));
    
    //Set Material ID
    nLine.SetMAT(steelSection.GetMaterialConversion(nLine.SectionName()));
    
    //Determine direction of current element
    Coord::Direction curPrimaryEtabsDirection = GetElementEtabsDirection(nLine);
    Coord::Direction curSecondaryEtabsDirection = GetEtabsSecondaryDirection(curPrimaryEtabsDirection);
    
    //Check for new nodes
    SteelNode SteelNodes[2];
    CheckAddNewNodes(nLine, curPrimaryEtabsDirection, curSecondaryEtabsDirection, SteelNodes);
    
    //Determine Element Orientation
    GetElemOrientation(nLine, SteelNodes, curPrimaryEtabsDirection);
    
    //Set IOR
    nLine.SetIOR(primaryEtabsDirection, curPrimaryEtabsDirection);
    
    //Set ICAT
    unsigned int etabsNode1 = (etabsLines[nLine.EtabsRefID()].NodeConnectivity())[0];
    
    //Get the first nodes Etabs node
    unsigned int node1Ref;
    if ((nLine.NodeConnectivity())[0] == SteelNodes[0].Num()) //THen matching
        node1Ref = SteelNodes[0].EtabsNodeNum();
    else
        node1Ref = SteelNodes[1].EtabsNodeNum();
    
    nLine.SetICAT(etabsNode1, node1Ref);
    
    //If current direction is secondary, check to see if element already exists in primary list
    if (curPrimaryEtabsDirection == secondaryEtabsDirection)
    {
        if  (!SecondaryElementExists(nLine))
        {   //Add to secondary lists and set ID
            numSecLineElements += 1;
            numLineElements += 1;
            nLine.SteelID(numLineElements);
            secondarySteelLines[numSecLineElements] = nLine;
            
            if (nLine.Type() == Element::ElementType::COLUMN)
                numSecCol += 1;
            else if (nLine.Type() == Element::ElementType::BEAM)
                numSecBeam += 1;
            else if (nLine.Type() == Element::ElementType::BRACE)
                numSecBrace += 1;
            else
                dout<<"Unknown Secondary Element Type\n";
        }
    }
    else
    {   //Add to primary Lists and set ID
        numPrimLineElements += 1;
        numLineElements += 1;
        nLine.SteelID(numLineElements);
        primarySteelLines[numPrimLineElements] = nLine;
            
        if (nLine.Type() == Element::ElementType::COLUMN)
            numPrimCol += 1;
        else if (nLine.Type() == Element::ElementType::BEAM)
            numPrimBeam += 1;
        else if (nLine.Type() == Element::ElementType::BRACE)
            numPrimBrace += 1;
        else
            dout<<"Unknown Primary Element Type\n";
    }
}

void Model::newNodalRestraint(NodalRestraint restraint)
{
    //Add to list
    restrainedNodes[std::make_pair(restraint.EtabsNodeNum(), restraint.EtabsFloor())] = restraint;
    
    //Check to see if FndNode Exists
    if (restraint.FndNodeExists())
        NewFndNode(restraint.FndNode());

}


std::string Model::FindStoryName(float elev)
{
    //Loop through all floors
    std::map<std::string, Story>::iterator iter;
    for (iter = etabsStories.begin(); iter != etabsStories.end(); iter++)
    {
        if ((iter->second).Elevation() == elev)
            return (iter->second).Name();
    }
    return "Error";
}

//operator overloading
std::istream& operator>>(std::istream& is, Model& model)         //Input Operator
{
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{PARSESTRINGS["Units"]}))
    {
        //Call Parse Units
        model.ParseUnits(line);
        
        //Parse Prebuild Databse
        model.ParsePrebuildSteel();
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["Story"]})) //Check for Story Grid Info
    {
        //Create new Story, Parse Info, then push back;
        Story story;
        GetSStream(line)>>story;
        model.newEtabsStory(story);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["Grid"]})) //Check for Etabs Grids
    {
        //Create new grid, parse it, then push back
        Grid grid;
        GetSStream(line)>>grid;
        model.newEtabsGrid(grid);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsWallProp"]})) //Check for Etabs Wall Property
    {
        EtabsAreaProps prop;
        GetSStream(line)>>prop;
        model.newEtabsAreaProp(prop);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsSlabProp"]})) //Check for Etabs Slab Property
    {
        EtabsAreaProps prop;
        GetSStream(line)>>prop;
        model.newEtabsAreaProp(prop);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsDeckProp"]})) //Check for Etabs Deck Property
    {
        DeckProp prop;
        GetSStream(line)>>prop;
        model.newDeckProp(prop);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["PointSpring"]})) //Check for Point Springs
    {
        PointSpring spring;
        GetSStream(line)>>spring;
        model.newFndNodeDef(spring);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsNode"]})) //Check for EtabsNodes
    {
        //Create new node, Parse Info, then push back;
        EtabsNode node;
        GetSStream(line)>>node;
        model.newEtabsNode(node);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsLine"]})) //Adding EtabsLines
    {
        //Create new Line, Parse Info, then push back;
        EtabsLine etabsLine;
        GetSStream(line)>>etabsLine;
        model.newEtabsLine(etabsLine);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsWallConnect"]})) //Adding New Wall Connectivity
    {
        //Create new wall, Parse Info, then push back;
        EtabsWall etabsWall;
        GetSStream(line)>>etabsWall;
        model.newEtabsWall(etabsWall);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["EtabsSlabConnect"]})) //Adding New Wall Connectivity
    {
        //Create new wall, Parse Info, then push back;
        EtabsWall etabsWall;
        GetSStream(line)>>etabsWall;
        model.newEtabsWall(etabsWall);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["NodalFixity"]})) // Adding nodal restraints and springs
    {
        //Create new fixed node, then push back
        NodalRestraint restraint;
        GetSStream(line)>>restraint;
        model.newNodalRestraint(restraint);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["WideFlangeCustom"]}))    //Adding Custom wide flange section
    {
        std::string label = matches[1].str();
        float D = std::stof(matches[3].str());
        float B = std::stof(matches[4].str());
        float TF = std::stof(matches[5].str());
        float TW = std::stof(matches[6].str());
        
        std::string material = matches[2].str();
        //std::cout<<"Material for " << label<<std::endl;
        model.newCustomSection(label, D, B, TF, TW, material);
        
        //Get Material Conv
        
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["BoxCustom"]}))    //Adding Custom wide flange section
    {
        std::string label = matches[1].str();
        float D = std::stof(matches[3].str());
        float B = std::stof(matches[4].str());
        float TF = std::stof(matches[5].str());
        float TW = std::stof(matches[6].str());

        std::string material = matches[2].str();
        //std::cout<<"Material for " << label<<std::endl;
        model.newCustomSection(label, D, B, TF, TW, material);
        
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["RegSection"]}))    //Adding Custom wide flange section
    {
        std::string label = matches[1].str();
        std::string material = matches[2].str();
        model.newRegSection(label, material);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["SteelLine"]})) //Adding SteelLines
    {
        //Create new line, parse info, then pusb back;
        SteelLine steelLine;
        GetSStream(line)>>steelLine;
        model.newSteelLine(steelLine);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["SteelArea"]})) //Adding Area Elements
    {
        SteelArea steelArea;
        GetSStream(line)>>steelArea;
        model.newSteelArea(steelArea);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["LoadPatterns"]})) //Adding a load Pattern
    {
        //Create new pattern, parse, then push back
        LoadPattern pattern;
        GetSStream(line)>>pattern;
        model.newLoadPattern(pattern);
    }
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["PointObjectLoads"]})) //Adding Joint Load
    {
        //Create new load, parse info, then push back
        StaticLoad load;
        GetSStream(line)>>load;
        model.newStaticLoad(load);
    
    }
    //else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["LoadCase"]}))
    //{
        
    //}
    else if (std::regex_search(line, matches, std::regex{PARSESTRINGS["LoadCombinations"]})) //Adding load combo
    {
        LoadCombination combo;
        GetSStream(line)>>combo;
        model.newLoadCombo(combo);
    
    }
    //else
        //std::cout<<"Model Parse Error: "<<line<<std::endl;


    return is;
}


std::ostream& operator<<(std::ostream& os, Model& model)   //Output Operator
{
        //Output Config File
        os << (model.OutputConfig()).str()<<std::endl;
        os <<std::endl;
    
        //Output nodes
        //os << "NODE        XCOOR        YCOOR"<<std::endl;
        os << (model.OutputNodes()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Line elements
        //os << "ELEM MEM  MAT     ISS"<<std::endl;
        os << (model.OutputLineElements()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Special Columns
        os << (model.OutputSpecialColumns()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Foundation Nodes
        os << (model.OutputFoundationNodes()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Basement Wall Elements
        os << (model.OutputBasementWallElements()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Diaphragms
        os << (model.OutputNNPBF()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //NVCONEL Output
        os << (model.OutputNVCONEL()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Displacement Output
        os << (model.OutputTH()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Material Info
        os << (model.OutputMaterial()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Steel Fiber Segment Modifications
        os << (model.OutputFiberSectionMod()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Fiber Probability Distribution Modifier
        os << (model.OutputFiberStrainSectionMod()).str();
        os << std::endl<<std::endl<<std::endl;
    
        //Output Segment Lengths
        os << (model.OutputSegmentLengths()).str();
        os <<std::endl;
    
        //GAMULT
        os << (model.OutputGAMULT()).str();
        os <<std::endl;
    
    return os;
}
