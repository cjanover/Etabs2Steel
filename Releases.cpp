#include "Releases.h"

const std::string Releases::ReleaseString()
{
    if (releases == ReleaseType::N1_FIX_N2_FIX)
        return "N1_FIX_N2_FIX";
    else if (releases == ReleaseType::N1_FIX_N2_FREE)
        return "N1_FIX_N2_FREE";
    else if (releases == ReleaseType::N1_FREE_N2_FIX)
        return "N1_FREE_N2_FIX";
    else if (releases == ReleaseType::N1_FREE_N2_FREE)
        return "N1_FREE_N2_FREE";
    else
        return "Release String Error";
}


//Operator Overloads
std::istream& operator>>(std::istream& is, Releases& release)
{
    d_out dout;
    if (!is) return is; //check for bad read


    //Get the etabs line nodes


    //Read in line
    std::string line;
    std::getline(is, line);

    //Find matches
    std::smatch matches;
    bool N1Free(false), N2Free(false);
    if (line.empty())
    {
        release.Release(Releases::ReleaseType::N1_FIX_N2_FIX);
        return is;
    }
    if (std::regex_search(line, matches, std::regex{R"(M3I)"}))
        N1Free = true;
    if (std::regex_search(line, matches, std::regex{R"(M3J)"}))
        N2Free = true;
    
    if (N1Free && N2Free)
        release.Release(Releases::ReleaseType::N1_FREE_N2_FREE);
    else if (N1Free && !N2Free)
        release.Release(Releases::ReleaseType::N1_FREE_N2_FIX);
    else if (!N1Free && N2Free)
        release.Release(Releases::ReleaseType::N1_FIX_N2_FREE);
    else
        dout<<"Release Parse Error\n";
        
    


    return is;
}

std::ostream& operator<<(std::ostream& os, Releases& release)
{
    return os;
}

