//
//  EtabsNode.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 2/11/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__EtabsNode__
#define __Etabs2Steel__EtabsNode__

#include <iostream>
#include "Node.h"

class EtabsNode : public Node
{
    public:
        const std::string ParserString(){return parseString;};
    
    private:
        std::string parseString = PARSESTRINGS["EtabsNode"];
};

//Operator Overloads
std::istream& operator>>(std::istream& is, EtabsNode& node);

#endif /* defined(__Etabs2Steel__EtabsNode__) */
