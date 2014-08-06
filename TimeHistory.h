//
//  TimeHistory.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/10/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__TimeHistory__
#define __Etabs2Steel__TimeHistory__

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <regex>


extern std::map <std::string, std::string> PARSESTRINGS;

class TimeHistory
{
    public:
        const std::string ParserString(){return parseString;};
    
    
        //Get Functions
        std::map<float, float>::iterator GetDataStart(){return data.begin();};
        std::map<float, float>::iterator GetDataEnd(){return data.end();};
        std::string Name(){return name;};
    
    
        //Set Functions
        void AddData(const float time, const float value){data[time]=value;};
        void Name(const std::string nName){name = nName;};
    private:
        std::string parseString = PARSESTRINGS["Functions"];
    
        std::string name;
    
    
        std::map<float, float> data;
};

//Operator Overloads
std::istream& operator>>(std::istream& is, TimeHistory& timeHistory);
std::ostream& operator<<(std::ostream& os, TimeHistory& timeHistory);


#endif /* defined(__Etabs2Steel__TimeHistory__) */
