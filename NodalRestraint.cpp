//
//  NodalRestraint.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/26/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "NodalRestraint.h"

NodalRestraint::NodalRestraint()
{
    U_Free[0] =true;
    U_Free[1] =true;
    U_Free[2] =true;
    U_Free[3] =true;
    
    U_NoSpring[0] = true;
    U_NoSpring[1] = true;
    U_NoSpring[2] = true;
    U_NoSpring[3] = true;
}

void NodalRestraint::SetFixity(NodalRestraint nRestraint)
{
    etabsNodeNum = nRestraint.EtabsNodeNum();
    etabsFloor = nRestraint.etabsFloor;
    
    const int* fixity = nRestraint.NodalFixity();
    U_Free[0] = fixity[0];
    U_Free[1] = fixity[1];
    U_Free[2] = fixity[2];
    U_Free[3] = fixity[3];
    
    const bool* spring = nRestraint.NodalSpring();
    U_NoSpring[0] = spring[0];
    U_NoSpring[1] = spring[1];
    U_NoSpring[2] = spring[2];
    U_NoSpring[3] = spring[3];
    
    const std::string* springName = nRestraint.NodalSpringProp();
    U_SpringProp[0] = springName[0];
    U_SpringProp[1] = springName[1];
    U_SpringProp[2] = springName[2];
    U_SpringProp[3] = springName[3];
}

void NodalRestraint::NodeFixity(int nUX, int nUY, int nUZ, int nRZ)
{
    U_Free[0] = nUX;
    U_Free[1] = nUY;
    U_Free[2] = nUZ;
    U_Free[3] = nRZ;
}

void NodalRestraint::NodeSpring(bool nUS[], std::string nUSP[])
{
    for (int i = 0; i<=3; i++)
    {
        U_NoSpring[i] = nUS[i];
        U_SpringProp[i] = nUSP[i];
    }
}


//Operator Overloads
std::istream& operator>>(std::istream& is, NodalRestraint& restraint)
{
    d_out dout;
    if (!is) return is;
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    std::smatch matches;
    
    if (std::regex_search(line, matches, std::regex{restraint.ParserString()}))
    {
        //matches[1] = Etabs Node #
        //matches[2] = Etabs Floor name
        //matches[3] = Potentially Nodal Restraints or direction for spring
        //matches[4] = Potentially spring props or if none then nothing
        
        
        //Save Node number and floor name
        restraint.EtabsNodeNum(std::stoi(matches[1].str()));
        restraint.EtabsFloor(matches[2].str());
        
        //Parse restraints
        if (std::regex_search(matches[3].str(), std::regex{R"(RESTRAINT)"}) && !std::regex_search(matches[4].str(), std::regex{R"(SPRINGPROP)"}))
        {
            int freeNodes[4] = {1, 1, 1, 1};
            
            //Search for restraints

            if (std::regex_search(matches[3].str(), std::regex{R"(UX)"}))
                freeNodes[0] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(UY)"}))
                freeNodes[1] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(UZ)"}))
                freeNodes[2] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(RX)"}))
                freeNodes[3] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(RY)"}))
                freeNodes[3] = 0;
            
            //if (!freeNodes[0] && !freeNodes[1] && !freeNodes[2])
            //    freeNodes[3] = false;
            //Save restraints
   //         std::cout<<"Saving Etabs Node ("<<matches[1].str()<<") at floor ("<<matches[2].str()<<") with fixity ("<<restraints[0]<<", "<<restraints[1]<<", "<<restraints[2]<<", "<<restraints[3]<<")"<<std::endl;
            restraint.NodeFixity(freeNodes[0], freeNodes[1], freeNodes[2], freeNodes[3]);
        }
        else if (std::regex_search(matches[3].str(), std::regex{R"(RESTRAINT)"}) && std::regex_search(matches[4].str(), std::regex{R"(SPRINGPROP)"}))
        {
            int freeNodes[4] = {1, 1, 1, 1};
            
            //Find the restraint directions
            if (std::regex_search(matches[3].str(), std::regex{R"(UX)"}))
                freeNodes[0] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(UY)"}))
                freeNodes[1] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(UZ)"}))
                freeNodes[2] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(RX)"}))
                freeNodes[3] = 0;
            if (std::regex_search(matches[3].str(), std::regex{R"(RY)"}))
                freeNodes[3] = 0;
            
            restraint.NodeFixity(freeNodes[0], freeNodes[1], freeNodes[2], freeNodes[3]);
            
            //Check for Springs t
            std::smatch springNamesMatch;
            if (std::regex_search(matches[4].str(), springNamesMatch, std::regex{R"(SPRINGPROP\s+\"([^\"]+)\")"}))
            {
                //springNamesMatch[1]->Name
            
                //Create Foundation Node
                FoundationNode tmpFndNode;
                
                //Save Info
                tmpFndNode.EtabsRefName(std::stoi(matches[1].str()));
                tmpFndNode.FloorName(matches[2].str());
                tmpFndNode.Name(springNamesMatch[1].str());
                
        
                restraint.FndNode(tmpFndNode);
                restraint.FndNodeExists(true);
            }
        
        }
        else if (!std::regex_search(matches[3].str(), std::regex{R"(RESTRAINT)"}) && std::regex_search(matches[4].str(), std::regex{R"(SPRINGPROP)"}))
        {
            std::smatch springNamesMatch;
            if (std::regex_search(matches[4].str(), springNamesMatch, std::regex{R"(SPRINGPROP\s+\"([^\"]+)\")"}))
            {
                //springNamesMatch[1]->Name
            
                //Create Foundation Node
                FoundationNode tmpFndNode;
                
                //Save Info
                tmpFndNode.EtabsRefName(std::stoi(matches[1].str()));
                tmpFndNode.FloorName(matches[2].str());
                tmpFndNode.Name(springNamesMatch[1].str());
                
        
                restraint.FndNode(tmpFndNode);
                restraint.FndNodeExists(true);
            }
            
        }
    }
    else
        dout<<"Nodal Fixity Parse Error\n";
    return is;
}

std::ostream& operator<<(std::ostream& os, const NodalRestraint& restraint)
{
    return os;
}