//
//  NewElement.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/13/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Model.h"

Coord::Direction Model::GetEquivSteelDirection(Coord::Direction etabsDirection)
{
    d_out dout;
    if (etabsDirection == Coord::Direction::X)
        return Coord::Direction::X;
    else if (etabsDirection == Coord::Direction::Y)
        return Coord::Direction::Z;
    else
    {
        dout<<"EquivDirection Error. Returning X\n";
        return Coord::Direction::X;
    }
}

Coord::Direction Model::GetEtabsSecondaryDirection(Coord::Direction etabsPrimary)
{
    d_out dout;
    if (etabsPrimary == Coord::Direction::X)
        return Coord::Direction::Y;
    else if (etabsPrimary == Coord::Direction::Y)
        return Coord::Direction::X;
    else
    {
        dout<<"Secondary Etabs Direction Error. Returning Y\n";
        return Coord::Direction::X;
    }
}

Coord::Direction Model::GetSteelSecondaryDirection(Coord::Direction steelPrimary)
{
    d_out dout;
    if (steelPrimary == Coord::Direction::X)
        return Coord::Direction::Z;
    else if (steelPrimary == Coord::Direction::Z)
        return Coord::Direction::X;
    else
    {
        dout<<"Secondary Steel Direction Error. Returning X\n";
        return Coord::Direction::X;
    }
}


Coord::Direction Model::GetElementEtabsDirection(SteelLine nLine)
{
    //If nLine is a column return global etabs primary
    
    //If the etabs X coordinate for Node 1 and Node 2 are the same, then it runs in the Etabs-X direction, else Etabs-Y
    d_out dout;
    
    if (nLine.Type() == Element::ElementType::COLUMN)
    {
        //return primaryEtabsDirection;
        int numMatches = 0;
        Coord::Direction matchDir;

        std::vector<unsigned int> etabsConnectivity = etabsLines[nLine.EtabsRefID()].NodeConnectivity();

    
        //If the column lands on a grid with a varying value in the primary direction or lands on an intersection of grids then it is primary
        std::map<unsigned long, Grid>::iterator gridItr;
        for (gridItr = etabsGrids.begin(); gridItr != etabsGrids.end(); gridItr++)
        {
            //Get grid coord and direction
            float curCoord = (gridItr->second).Coordinate();
            Coord::Direction curDir = (gridItr->second).EtabsGridDirection();
            
            unsigned int checkNode = etabsConnectivity[0];
            float checkCoord = etabsNodes[checkNode].Coords(curDir);
            if (isEqual(curCoord,checkCoord)) //Then the node lands on that grid
            {
                matchDir = (gridItr->second).EtabsGridDirection();
                numMatches++;   //Incrment number of matches
            }
            if (numMatches == 2) //Then node lands on 2 grids, so its primary
            {
                if (debugLevel >=2)
                    dout<<"Element: "<<nLine.SteelID()<<" is defined as "<<Coord::FindDirString(primaryEtabsDirection)<<"\n";
                return primaryEtabsDirection;

            }
        }
        if (debugLevel >= 2)
            dout<<"Element: "<<nLine.SteelID()<<" is defined as "<<Coord::FindDirString(matchDir)<<"\n";
        return matchDir;    //Else return which grid the match landed on
    }
    else
    {
        //Get connectivity and Z coordinate for new Line element
        std::vector<unsigned int> etabsConnectivity = etabsLines[nLine.EtabsRefID()].NodeConnectivity();
    
        float etabsNode1X = etabsNodes[etabsConnectivity[0]].Coords(Coord::Direction::X);
        float etabsNode2X = etabsNodes[etabsConnectivity[1]].Coords(Coord::Direction::X);
    
        if ( isEqual(etabsNode1X,etabsNode2X))
        {
            if (debugLevel >= 2)
                dout<<"Element: "<<nLine.SteelID()<<" is defined as "<<"Y"<<"\n";
            return Coord::Direction::Y;
        }
        else
        {
            if (debugLevel >= 2)
                dout<<"Element: "<<nLine.SteelID()<<" is defined as "<<"X"<<"\n";
            return Coord::Direction::X;
        }
    }
}



void Model::CheckAddNewNodes(SteelLine nLine, const Coord::Direction curPrimaryEtabsDirection, const Coord::Direction curSecondaryEtabsDirection, SteelNode newNodes[])
{
    d_out dout;
    //Get curPrimary Steel Direction
    Coord::Direction curPrimarySteelDirection = GetEquivSteelDirection(curPrimaryEtabsDirection);
    
    //Get connectivity and Z coordinate for new Line element
    std::vector<unsigned int> etabsConnectivity = etabsLines[nLine.EtabsRefID()].NodeConnectivity();
    
    
    
    float etabsNode1X = etabsNodes[etabsConnectivity[0]].Coords(Coord::Direction::X);
    float etabsNode1Y = etabsNodes[etabsConnectivity[0]].Coords(Coord::Direction::Y);
    float etabsNode1Ver = etabsNodes[etabsConnectivity[0]].Coords(Coord::Direction::Z); //For nodes not on grid
    
    float etabsNode2X = etabsNodes[etabsConnectivity[1]].Coords(Coord::Direction::X);
    float etabsNode2Y = etabsNodes[etabsConnectivity[1]].Coords(Coord::Direction::Y);
    float etabsNode2Ver = etabsNodes[etabsConnectivity[1]].Coords(Coord::Direction::Z); //For nodes not on grid

    float storyElevation = etabsStories[nLine.EtabsStoryID()].Elevation();
    float storyHeight = etabsStories[nLine.EtabsStoryID()].Height();
    
    //Find the elevation of the proposed nodes in Steel
    float steelNode1Ver = 0.f;
    float steelNode2Ver = 0.f;

    if (nLine.Type() == Element::ElementType::BEAM) //Then nodes are on the same floor
    {
        steelNode1Ver = storyElevation - etabsNode1Ver;
        steelNode2Ver = storyElevation - etabsNode2Ver;
    }
    else //They are on different levels, 2nd higher then 1st
    {
        if (etabsNode1Ver != 0)
            storyHeight = 0;
        steelNode1Ver = storyElevation - etabsNode1Ver - storyHeight;
        steelNode2Ver = storyElevation - etabsNode2Ver;
    }
    
    //Search through steelNodes to try to see if this node already exists
    unsigned int node1ID = 0;
    unsigned int node2ID = 0;

    
    std::map<unsigned int, SteelNode>::iterator steelNodeIterator;
    std::map<unsigned int, SteelNode>* curNodes;
    unsigned int* curNumNodes;
    
    //Search through proper list
    if (curPrimarySteelDirection == primarySteelDirection)
    {
        curNodes = &primarySteelNodes;
        curNumNodes = &numPrimaryNodes;
    }
    else
    {
        curNodes = &secondarySteelNodes;
        curNumNodes = &numSecondaryNodes;
    }
    
    for (steelNodeIterator = curNodes->begin(); steelNodeIterator != curNodes->end(); steelNodeIterator++)
    {
        unsigned int curEtabsRefID = (steelNodeIterator->second).EtabsNodeNum();
        float curSteelVer = (steelNodeIterator->second).Coords(Coord::Direction::Z);
        
        //Check node 1 - If the reference ID of the steel node is identical to the incoming ID and they have the same Z value, its a repeat node
        if (curEtabsRefID == etabsConnectivity[0] && isEqual(curSteelVer,steelNode1Ver))
        {
            node1ID = (steelNodeIterator->second).Num(); //Node 1 already exists
            newNodes[0] = (steelNodeIterator->second);
            
            if (debugLevel >=1)
                dout<<"Existing node ("<<node1ID<<") used for Element:"<<nLine.SteelID()<<" at ("<<etabsNode1X<<", "<<etabsNode1Y<<", "<<steelNode1Ver<<")\n";
        }
        
        //Check node 2 - If the reference ID of the steel node is identical to the incoming ID and they have the same Z value, its a repeat node
        if (curEtabsRefID == etabsConnectivity[1] && isEqual(curSteelVer,steelNode2Ver))
        {
            node2ID = (steelNodeIterator->second).Num(); //Node 2 already exists
            newNodes[1] = (steelNodeIterator->second);
            
            if (debugLevel >=1)
                dout<<"Existing node ("<<node2ID<<") used for Element:"<<nLine.SteelID()<<" at ("<<etabsNode1X<<", "<<etabsNode1Y<<", "<<steelNode1Ver<<")\n";
        }
    }

    //If either node hasnt been found then it is a new node
    if (node1ID == 0) //Create node and push onto list and increment counter
    {
        *curNumNodes += 1;
        numNodes += 1;

        //newNodes[0].Num(*curNumNodes);
        newNodes[0].Num(numNodes);
        newNodes[0].ListNum(*curNumNodes);
        newNodes[0].Coords(etabsNode1X, etabsNode1Y, steelNode1Ver);
        newNodes[0].EtabsNodeNum(etabsConnectivity[0]);

        //Set the nodes's AlphaVC


        //node1ID = *curNumNodes;
        node1ID = numNodes;
        
        (*curNodes)[numNodes] = newNodes[0];
        
        if (debugLevel >=1)
            dout<<"New node ("<<numNodes<<") Created for Element:"<<nLine.SteelID()<<" at ("<<etabsNode1X<<", "<<etabsNode1Y<<", "<<steelNode1Ver<<")\n";
    }
    
    
    if (node2ID == 0) //Create node and push onto list and increment counter
    {
        *curNumNodes += 1;
        numNodes += 1;

        //newNodes[1].Num(*curNumNodes);
        newNodes[1].Num(numNodes);
        newNodes[1].ListNum(*curNumNodes);
        newNodes[1].Coords(etabsNode2X, etabsNode2Y, steelNode2Ver);
        newNodes[1].EtabsNodeNum(etabsConnectivity[1]);
        
        //node2ID = *curNumNodes;
        node2ID = numNodes;
        
        (*curNodes)[numNodes] = newNodes[1];
        
        if (debugLevel >=1)
            dout<<"New node ("<<numNodes<<") Created for Element:"<<nLine.SteelID()<<" at ("<<etabsNode1X<<", "<<etabsNode1Y<<", "<<steelNode1Ver<<")\n";
    }
}

void Model::GetElemOrientation(SteelLine &nLine, SteelNode nodes[], const Coord::Direction curPrimaryEtabsDirection)
{
    d_out dout;
    //Store node IDs
    unsigned int node1ID = nodes[0].Num();
    unsigned int node2ID = nodes[1].Num();
    //unsigned int node1ListID = nodes[0].ListNum();
    //unsigned int node2ListID = nodes[1].ListNum();

    //Setup to read from the proper node list
    std::map<unsigned int, SteelNode>* curNodes;
    

    

    //Coord::Direction useDirection;
    
    if (curPrimaryEtabsDirection == primaryEtabsDirection)
        curNodes = &primarySteelNodes;
    else
        curNodes = &secondarySteelNodes;
    
    //If curPrimaryEtabsDirection = X then compare Y values, else compare X values
    float N1Comp = (*curNodes)[node1ID].Coords(curPrimaryEtabsDirection);
    float N2Comp = (*curNodes)[node2ID].Coords(curPrimaryEtabsDirection);
    
    float N1Ver = (*curNodes)[node1ID].Coords(Coord::Direction::Z);
    float N2Ver = (*curNodes)[node2ID].Coords(Coord::Direction::Z);
    
    if (nLine.Type() == Element::ElementType::COLUMN) //For columns higher node first
    {
        if (N1Ver > N2Ver)
            nLine.NodeConnectivity(node1ID, node2ID);
        else
            nLine.NodeConnectivity(node2ID, node1ID);
        nLine.SetMT(2); //Type Column
    }
    else if (nLine.Type() == Element::ElementType::BEAM) //For beam node with higher X direction comes first
    {
        if (N1Comp > N2Comp)
            nLine.NodeConnectivity(node1ID, node2ID);
        else
            nLine.NodeConnectivity(node2ID, node1ID);
        nLine.SetMT(1); //Type Beam
    }
    else if (nLine.Type() == Element::ElementType::BRACE) //For braces if sloping / then larger X first, of \ then lower X first
    {
        //Orientation is like / if a node is both larger in X and Y or less in both X and Y
        if ( (N1Ver > N2Ver && N1Comp > N2Comp) || (N2Ver > N1Ver && N2Comp > N1Comp) )
        {
            if ( N1Ver > N2Ver )
                nLine.NodeConnectivity(node1ID, node2ID);
            else
                nLine.NodeConnectivity(node2ID, node1ID);
            nLine.SetMT(3); //Type Brace angled /
        }
        else if ( (N1Ver > N2Ver && N1Comp< N2Comp) || (N2Ver > N1Ver && N2Comp < N1Comp) )
        {
            if ( N1Ver > N2Ver )
                nLine.NodeConnectivity(node1ID, node2ID);
            else
                nLine.NodeConnectivity(node2ID, node1ID);
            nLine.SetMT(4); //Type Brace angled not /
        }
        else
        {
            dout<<"Cant figure out brace orientation. Terminating Analysis.\n";
            exit(0);
        }    
    }
    else
    {
        dout<<"Unknown element type for connectivity calculation. Terminating Analysis.\n";
        exit(0);
    }
    
    if (debugLevel >=1)
        dout<<"Element: "<<nLine.SteelID()<<" given connectivity of ("<<nLine.NodeConnectivity()[0]<<", "<<nLine.NodeConnectivity()[1]<<")\n";
}

bool Model::SecondaryElementExists(SteelLine nLine)
{
    d_out dout;
    //Get the proposed nodes connectivity and coordinates
    std::vector<unsigned int> pConnectivity = nLine.NodeConnectivity();
    //Get current x, y, z coordinates
    float p1X = secondarySteelNodes[pConnectivity[0]].Coords(Coord::Direction::X);
    float p1Y = secondarySteelNodes[pConnectivity[0]].Coords(Coord::Direction::Y);
    float p1Z = secondarySteelNodes[pConnectivity[0]].Coords(Coord::Direction::Z);
        
    float p2X = secondarySteelNodes[pConnectivity[1]].Coords(Coord::Direction::X);
    float p2Y = secondarySteelNodes[pConnectivity[1]].Coords(Coord::Direction::Y);
    float p2Z = secondarySteelNodes[pConnectivity[1]].Coords(Coord::Direction::Z);


    //Setup iterator through Primary Element List
    std::map<unsigned int, SteelLine>::iterator curSteelLine;
    
    for (curSteelLine = primarySteelLines.begin(); curSteelLine != primarySteelLines.end(); curSteelLine++)
    {
        //Get current connectivity
        std::vector<unsigned int> curConnectivity = (curSteelLine->second).NodeConnectivity();
        //Get current x, y, z coordinates
        float cur1X = primarySteelNodes[curConnectivity[0]].Coords(Coord::Direction::X);
        float cur1Y = primarySteelNodes[curConnectivity[0]].Coords(Coord::Direction::Y);
        float cur1Z = primarySteelNodes[curConnectivity[0]].Coords(Coord::Direction::Z);
        
        float cur2X = primarySteelNodes[curConnectivity[1]].Coords(Coord::Direction::X);
        float cur2Y = primarySteelNodes[curConnectivity[1]].Coords(Coord::Direction::Y);
        float cur2Z = primarySteelNodes[curConnectivity[1]].Coords(Coord::Direction::Z);
        
        //If both nodes coordinates match then it is a duplicate element, ignore
        if ( isEqual(p1X,cur1X) && isEqual(p1Y,cur1Y) && isEqual(p1Z,cur1Z) && isEqual(p2X,cur2X) && isEqual(p2Y,cur2Y) && isEqual(p2Z,cur2Z) )
        {
            dout<<"Duplicate Element: "<<nLine.EtabsRefID()<<" with: "<<(curSteelLine->second).SteelID()<<"\n";
            return true;
        }
        
    }
    return false;
}
