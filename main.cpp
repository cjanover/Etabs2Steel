        //
//  main.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <map>

#include "Coord.h"
#include "Node.h"
#include "Model.h"
#include "Parse.h"
#include "Debug.h"

#include "FloatCompare.h"

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif



using namespace std;

std::map <string, string> PARSESTRINGS;
std::map<unsigned int, unsigned int> nodeNumberLookup;  //Lookup from original node numbering to more efficient
std::string saveDirectory;
std::string workingDirectory;
std::ofstream debugFile;
int debugLevel;
bool developerMode = 1;
float tol = 0.01;


void ParseStrings();


int main(int argc, const char * argv[])
{
    d_out dout;
    //Setup debugFile
    debugFile.open("Debug.err");

    workingDirectory = argv[0];
    //search through workingDirectory and find final occurance of /
    int index=0;
    int totIndex=0;
    std::string tmpString = workingDirectory;
    while (tmpString.find("/")!=-1)
    {
        index = tmpString.find("/");
        tmpString = tmpString.substr(index+1,tmpString.size());
        totIndex = totIndex+index+1;
    }
    workingDirectory = workingDirectory.substr(0,totIndex-1);
   


    //Setup Parse Strings
    ParseStrings();

    //Redirect cerr to error.txt
    std::stringstream savePath;
    std::string saveLoc = saveDirectory;
    savePath<<saveLoc << "/" << "errorLog.txt";
    std::ofstream errOut(savePath.str());
    std::cerr.rdbuf(errOut.rdbuf());
    
    //Create Model
    Model model;

    //Parse Config File
    model.ParseConfig();
    
    //Read Input File
    model.ReadInput();
    
    //Prepare Model
    model.PrepareModel();
    
    //Run Modle
    model.RunModel();
    

    return 0;
}

void ParseStrings()
{
    PARSESTRINGS["Units"] = R"(^\s+UNITS\s+\"(.+)\"\s+\"(.+)\"\s+\"(.+)\")";

//                            R"(\s+STORY\s+\"(\w+)\"\s+(\w+)\s+(\d+).+)"
    PARSESTRINGS["Story"] = R"(^\s+STORY\s+\"(.+)\"\s+(\w+)\s+([^\s]+).+)";
    
                            //\s+GRID\s+\"\w+\"\s+LABEL\s+\"(\w+)\"\s+DIR\s+\"(\w+)\"\s+COORD\s+(\w+))
    PARSESTRINGS["Grid"] = R"(^\s+GRID\s+\"\w+\"\s+LABEL\s+\"([^/"]+)\"\s+DIR\s+\"(\w+)\"\s+COORD\s+([^\s]+))";
    PARSESTRINGS["Coord"] = R"(([^\s]+)\s([^\s]+)\s([^\s]+)?)";
    
    PARSESTRINGS["PointSpring"] = R"(\s+POINTSPRING\s+\"([^\"]+)\"(.+))";
    
    
    PARSESTRINGS["EtabsNode"] = R"(^\s+POINT\s+\"(\d+)\"\s+(.+))";
    PARSESTRINGS["SteelNode"] = R"(\s+POINTASSIGN\s+\"(\d+)\"\s+\"(\w+)\".+)";

    //(  LINE  "C1"  COLUMN  "1"  "1"  1)
        //Save C1 COLUMN 1 1 1
    PARSESTRINGS["EtabsLine"] = R"(\s+LINE\s+\"(\w+)\"\s+(\w+)\s+\"(\d+)\"\s+\"(\d+)\"\s+(\d+))";
    
    PARSESTRINGS["LineConnect"] = R"(\s+LINE\s+\"(\w+)\"\s+(\w+)\s+\"(\d+)\"\s+\"(\d+)\".+)";
    
                                //   \s+POINTASSIGN\s+\"([^\"]+)\"\s+\"([^\"]+)\"\s+SPRINGPROP\s+\"([^\"]+)\"
    //PARSESTRINGS["PointAssign"] = R"(\s+POINTASSIGN\s+\"([^\"]+)\"\s+\"([^\"]+)\"\s+SPRINGPROP\s+\"([^\"]+)\")";
    
    
    //LINEASSIGN  "B3"  "Story1"  SECTION "A-LatBm"  RELEASE "TI M2I M3I"CARDINALPT 8  MAXSTASPC 24 MESH "POINTSANDLINES"
        //Save B3, Story1, A-LatBeam, and releases if they are there. Ignore the rest
                           //      \s+LINEASSIGN\s+\"(\w+)\"\s+\"(\w+)\"\s+SECTION\s+\"(\w+)\"\s+(RELEASE\s+\")?(.+)?.+
    PARSESTRINGS["SteelLine"] = R"(\s+LINEASSIGN\s+\"(\w+)\"\s+\"(\w+)\"\s+SECTION\s+\"([^\"]+)\"\s+(ANG\s+\w+)?(RELEASE\s+\")?([\w\s]+)?.+)?)";
    
    // TI M2I M3I
    PARSESTRINGS["ElementReleases"] = R"((\w+))";
    
    // POINTASSIGN  "1"  "Base"  RESTRAINT "UX UY UZ"
    // or
    // POINTASSIGN  "2"  "Base"  RESTRAINT "UY"  SPRINGPROP "PSpr1"  USERJOINT  "Yes"
    PARSESTRINGS["NodalFixity"] = R"(\s+POINTASSIGN\s+\"(\w+)\"\s+\"(\w+)\"\s+(RESTRAINT\s+\"[\w\s]+\")?\s+(SPRINGPROP\s+\"\w+\")?)";
    //PARSESTRINGS["NodalFixity"] = R"(\s+POINTASSIGN\s+\"(\w+)\"\s+\"(\w+)\"\s+(RESTRAINT\s+\"[\w\s]+\")+)";
    //LOADPATTERN "Dead"  TYPE  "Dead"  SELFWEIGHT  1
                                    //\s+LOADPATTERN\s+\"(\w+)\"\s+TYPE\s+\"(\w+)\".+
    PARSESTRINGS["LoadPatterns"] = R"(\s+LOADPATTERN\s+\"(\w+)\"\s+TYPE\s+\"(\w+)\".+)";
    
    
    PARSESTRINGS["Functions"] = R"(\s+FUNCTION\s+\"(.+)\"\s+FUNCTYPE)";
    //PARSESTRINGS["LoadCase"] = R"(LOADCASE\s+)";
    
    
    //POINTLOAD  "1"  "Base"  TYPE "FORCE"  LC "Dead"    FZ -10000
    PARSESTRINGS["PointObjectLoads"] = R"(\s+POINTLOAD\s+\"(\w+)\"\s+\"(\w+)\"\s+TYPE\s+\"(\w+)\"\s+LC\s+\"(\w+)\"\s+(.+))";
    
    //  COMBO "D+L"  TYPE "Linear Add"
    //or
    //  COMBO "D+L"  LOADCASE "Live"  SF 1.2
    //or
    //  COMBO "TimeHistCombo"  LOADCOMBO "ToTDead"  SF 1.2
                                        //\s+COMBO\s+\"(\w+)\"\s+(\w+)\s+\"(\w+)\"
                                        //\s+COMBO\s+\"(.+)\"\s+(.+)
    PARSESTRINGS["LoadCombinations"] = R"(\s+COMBO\s\"(.+)\"\s+(\w+)\s+\"(.+)\"\s*(SF)?\s*(.+)?)";
    
    
    PARSESTRINGS["WideFlangeCustom"] = R"(\s+FRAMESECTION\s+\"([^\"]+)\"\s+MATERIAL\s+\"([^\"]+)\".+\"Steel I\/Wide Flange\"\s+D\s+([\w.]+)\s+B\s+([\w.]+)\s+TF\s+([\w.]+)\s+TW\s+([\w.]+))";
    PARSESTRINGS["BoxCustom"] = R"(\s+FRAMESECTION\s+\"([^\"]+)\"\s+MATERIAL\s+\"([^\"]+)\".+\"Steel Tube\"\s+D\s+([\w.]+)\s+B\s+([\w.]+)\s+TF\s+([\w.]+)\s+TW\s+([\w.]+))";
//    PARSESTRINGS["RegSection"] = R"(\s+FRAMESECTION\s+\"([^\"]+)\"\s+MATERIAL\s+\"([^\"]+)\".+SHAPE\s+\"([^\"]+)\"$)";
    PARSESTRINGS["RegSection"] =R"(\s+FRAMESECTION\s+\"([^\"]+)\"\s+MATERIAL\s+\"([^\"]+)\".+)";

    

    PARSESTRINGS["EtabsWallProp"] = R"(\s+SHELLPROP\s+\"([^\"]+)\"\s+PROPTYPE\s+\"([^\"]+)\"\s+MATERIAL\s+\"([^\"]+)\"\s+MODELINGTYPE\s+\"[^\"]+\"\s+WALLTHICKNESS\s+([\d.]+))";
    PARSESTRINGS["EtabsSlabProp"] = R"(\s+SHELLPROP\s+\"([^\"]+)\"\s+PROPTYPE\s+\"([^\"]+)\"\s+MATERIAL\s+\"([^\"]+)\"\s.+SLABTHICKNESS\s+([\d.]+))";
    
                                    //(\s+SHELLPROP\s+\"([^\"]+)\"\s+PROPTYPE\s+\"([^\"]+)\"\s+DECKTYPE\s+\"([^\"]+)\"\s+CONCMATERIAL\s+\"([^\"]+)\"\s+DECKMATERIAL\s+\"([^\"]+)\"\s+DECKSLABDEPTH\s+([\d.]+)\s+DECKRIBDEPTH\s+([\d.]+)\s+DECKRIBWIDTHTOP\s+([\d.]+)
    //
    PARSESTRINGS["EtabsDeckProp"] = R"(\s+SHELLPROP\s+\"([^\"]+)\"\s+PROPTYPE\s+\"([^\"]+)\"\s+DECKTYPE\s+\"([^\"]+)\"\s+CONCMATERIAL\s+\"([^\"]+)\"\s+DECKMATERIAL\s+\"([^\"]+)\"\s+DECKSLABDEPTH\s+([\d.]+)\s+DECKRIBDEPTH\s+([\d.]+)\s+DECKRIBWIDTHTOP\s+([\d.]+)\s+DECKRIBWIDTHBOTTOM\s+([\d.]+)\s+DECKRIBSPACING\s+([\d.]+)\s+DECKSHEARTHICKNESS\s+([\d.]+)\s+DECKUNITWEIGHT\s+([\d.]+)\s+SHEARSTUDDIAM\s+([\d.]+)\s+SHEARSTUDHEIGHT\s+([\d.]+)\s+SHEARSTUDFU\s+([\d.]+))";

    PARSESTRINGS["EtabsWallConnect"] = R"(\s+AREA\s+\"([^\"]+)\"\s+PANEL\s+\d+\s+\"(\w+)\"\s+\"(\w+)\"\s+\"(\w+)\"\s+\"(\w+)\")";
    PARSESTRINGS["EtabsSlabConnect"] = R"(\s+AREA\s+\"([^\"]+)\"\s+FLOOR\s+\d+\s+\"(\w+)\"\s+\"(\w+)\"\s+\"(\w+)\"\s+\"(\w+)\")";
                                // \s+AREAASSIGN\s+\"([^\"]+)\"\s+\"([\w^\"]+)\"\s+SECTION\s+\"([^\"]+)\"
    PARSESTRINGS["SteelArea"] = R"(\s+AREAASSIGN\s+\"([^\"]+)\"\s+\"([^\"]+)\"\s+SECTION\s+\"([^\"]+)\".+)";
}

