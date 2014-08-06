//
//  EtabsAreaProps.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "EtabsAreaProps.h"


//operator overloading
std::istream& operator>>(std::istream& is, EtabsAreaProps& props)         //Input Operator
{
    d_out dout;
    if (!is) return is; //check for bad read
    
    //Read in line
    std::string line;
    std::getline(is, line);
    
    //Find matches
    std::smatch matches1;
    std::smatch matches2;
    
    
    if (std::regex_search(line, matches1, std::regex{props.ParserStringWall()})) //[1] = Name, [2] = Type, [3] = Material, [4] = Thickness
    {
        //std::cout<<"Wall Prop"<<std::endl;
        //std::cout<<matches1[1].str()<<" " <<matches1[2].str()<<" "<<matches1[3].str()<<" "<<matches1[4].str()<<std::endl;
        
        //Save Data
        props.Name(matches1[1].str());
        props.Type(matches1[2].str());
        props.Material(matches1[3].str());
        props.Thickness(std::stof(matches1[4].str()));
        
    }
    else if (std::regex_search(line, matches2, std::regex{props.ParserStringSlab()})) //[1] = Name, [2] = Type, [3] = Material, [4] = Thickness
    {
        //std::cout<<"Slab Prop"<<std::endl;
        //std::cout<<matches2[1].str()<<" " <<matches2[2].str()<<" "<<matches2[3].str()<<" "<<matches2[4].str()<<std::endl;

        //Save Data
        props.Name(matches2[1].str());
        props.Type(matches2[2].str());
        props.Material(matches2[3].str());
        props.Thickness(std::stof(matches2[4].str()));
    
    }
    else
        dout<<"Etabs Area Property Parse Error\n";

    return is;
}
