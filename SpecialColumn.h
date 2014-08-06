//
//  SpecialColumn.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/28/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__SpecialColumn__
#define __Etabs2Steel__SpecialColumn__

#include <iostream>

#include "FloatCompare.h"

#include "Debug.h"
extern int debugLevel;

class SpecialColumn
{
    public:
        SpecialColumn(){};
    
        //Get Functions
        int Num(){return eleNumber;};
        float PDL1(){return pdl1;};
        float PDL4(){return pdl4;};
        float STRMULT(){return STRmult;};
        float STFMULT(){return STFmult;};
    
        //Set Functions
        void Num(const int nNum){eleNumber = nNum;};
        void PDL1(const float nPDL1){pdl1 = nPDL1;};
        void PDL4(const float nPDL4){pdl4 = nPDL4;};
        void STRMULT(const float nSTRMULT){STRmult = nSTRMULT;};
        void STFMULT(const float nSTFMULT){STFmult = nSTFMULT;}
    
    
    private:
        int eleNumber;
        float pdl1 = 0;
        float pdl4;
        float STRmult;
        float STFmult;
};


std::ostream& operator<<(std::ostream& os, SpecialColumn& SpecialColumn);

#endif /* defined(__Etabs2Steel__SpecialColumn__) */
