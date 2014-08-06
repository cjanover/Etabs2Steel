//
//  SteelSection.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/8/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "SteelSection.h"

void SteelSection::AddSectionDefinition(const std::string label, const float d, const float tw, const float b, const float tf)
{
    d_out dout;
    std::ostringstream def;
    std::ostringstream tmp;
    //Create Conversion
    AddConversion(label);
    
    //Check to see if label begins with HSS, if so double the tw since there are two walls
    float f_tw = tw;
    if (label.substr(0,3) == "HSS")
        f_tw = tw*2;
    
    def << GetConversion(label) << " " << std::to_string(d) << " " << std::to_string(f_tw) << " " << std::to_string(b) << " " << std::to_string(tf) << std::endl;
    //DebugOut("Custom Section: "<<def.str(),3);
    sectionDefinition.push_back(def.str());
    sectionList.push_back(label);
    
    if (debugLevel>=2)
        dout<<"Adding Section: "<<label<<"d = "<<d<<" tw = "<<tw<<" b = "<<b<<" tf = "<<tf<<"\n";
    
}


void SteelSection::AddConversion(std::string section)
{
    //Generate new ID
    unsigned long id = 10000 + sectionConversion.size();
    
    sectionConversion[section] = id;
}

void SteelSection::ParsePrebuildDatabse(std::vector<std::string>::iterator listBegin, std::vector<std::string>::iterator listEnd, float SF)
{
    d_out dout;
    //Iterate through steel section databse list
    std::vector<std::string>::iterator listItr;
    for (listItr = listBegin; listItr != listEnd; listItr++)
    {
        //Setup File Location
        std::string curFileName = *listItr;


        
        
        std::ostringstream curFilePath;
        curFilePath << workingDirectory << "/SectionDatabase/SteelMembers/" << *listItr;
    
        //Open the file
        std::ifstream curFile(curFilePath.str());
        
        if (!curFile)
        {
            dout<<"Cannot Open Steel Section Databse File:" << *listItr << "\n";
            continue;
        }
        curFile.exceptions(curFile.exceptions()|std::ios_base::badbit);
        
        //Setup Regex Searches
        std::regex r_box_start(R"(<STEEL_BOX>)");
        std::regex r_box_end(R"(<\/STEEL_BOX>)");
        
        std::regex r_w_start(R"(<STEEL_I_SECTION>)");
        std::regex r_w_end(R"(<\/STEEL_I_SECTION>)");
        
        std::regex r_label(R"(<LABEL>(.+)<\/LABEL>)");
        
        //Wide Flange
        std::regex r_w_d(R"(<D>([\w.]+)<\/D>)");
        std::regex r_w_tw(R"(<TW>([\w.]+)<\/TW>)");
        std::regex r_w_b(R"(<BF>([\w.]+)<\/BF>)");
        std::regex r_w_tf(R"(<TF>([\w.]+)<\/TF>)");
        
        //Box
        std::regex r_box_d(R"(<HT>([\w.]+)<\/HT>)");
        std::regex r_box_tw(R"(<TW>([\w.]+)<\/TW>)");
        std::regex r_box_b(R"(<B>([\w.]+)<\/B>)");
        std::regex r_box_tf(R"(<TF>([\w.]+)<\/TF>)");
        
        std::string line;
        
        //Section Info
        std::string curLabel, curD, curB, curTF, curTW;

        while (getline(curFile, line))
        {
            std::smatch matches;
            
            if (std::regex_search(line, matches, r_w_start))  //Found Wide Flange Section or box
            {
                while (!std::regex_search(line, matches, r_w_end))  //Keep Reading until end of section definition
                {
                    getline(curFile, line);
                    std::smatch secMatch;
                    
                    if (std::regex_search(line, secMatch, r_label))
                        curLabel = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_w_d))
                        curD = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_w_tw))
                        curTW = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_w_b))
                        curB = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_w_tf))
                        curTF = secMatch[1].str();
                }
                //Save Data
                AddSectionList(curLabel);   //Add to Secion List
                std::ostringstream section;
                
                //Add Conversion
                AddConversion(curLabel);
                

                
                section << GetConversion(curLabel) << " " << std::stof(curD)*SF << " " << std::stof(curTW)*SF << " " << std::stof(curB)*SF << " " << std::stof(curTF)*SF << std::endl;
                
                AddSectionDefinition(section.str());
                
                if (debugLevel>=2)
                    dout<<"Adding Prebuilt Wide Flange Section: "<<curLabel<<"d = "<<curD<<" tw = "<<curTW<<" b = "<<curB<<" tf = "<<curTF<<"\n";
                
                
            }
            else if (std::regex_search(line, matches, r_box_start))
            {
                while (!std::regex_search(line, matches, r_box_end))
                {
                    getline(curFile, line);
                    std::smatch secMatch;
                    
                    if (std::regex_search(line, secMatch, r_label))
                        curLabel = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_box_d))
                        curD = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_box_tw))
                        curTW = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_box_b))
                        curB = secMatch[1].str();
                    else if (std::regex_search(line, secMatch, r_box_tf))
                        curTF = secMatch[1].str();
                
                
                }
                //Save Data
                AddSectionList(curLabel);   //Add to Secion List
                std::ostringstream section;
                
                //Add Conversion
                AddConversion(curLabel);
                
                //Check to see if label begins with HSS, if so double the tw since there are two walls
                float f_tw = std::stof(curTW);
                if (curLabel.substr(0,3) == "HSS")
                    f_tw = 2*f_tw;
                
                section << GetConversion(curLabel) << " " << std::stof(curD)*SF << " " << std::to_string(f_tw*SF) << " " << std::stof(curB)*SF << " " << std::stof(curTF)*SF << std::endl;
                
                AddSectionDefinition(section.str());
            
                if (debugLevel>=2)
                    dout<<"Adding Prebuilt HSS Section: "<<curLabel<<"d = "<<curD<<" tw = "<<curTW<<" b = "<<curB<<" tf = "<<curTF<<"\n";
            }
        }
    }
}

//Operator Overloads
std::ostream& operator<<(std::ostream& os, SteelSection& steelSection)
{
    //Loop through section databse
    std::vector<std::string>::iterator sectItr;
    
    for (sectItr = steelSection.GetSectionDefinitionStart(); sectItr != steelSection.GetSectionDefinitionEnd(); sectItr++)
        os << *sectItr;

    return os;
}
    
