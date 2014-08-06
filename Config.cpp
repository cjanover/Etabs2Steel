//
//  Config.cpp
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/10/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#include "Config.h"
#include "Debug.h"

Config::Config()
{
    //Add the appropriate keys to the config list
    
    //Model Geometry
    AddConfigList("TITLE");         //Title of Analysis (Name output data will be saved to)
    AddConfigList("NNP");           //Number of Node Points
    AddConfigList("NEL");           //Number of Line Elements
    AddConfigList("NNPFN");         //Number of Foundation Nodes
    AddConfigList("NBEL");          //Number of basement wall elements (max 100)
    AddConfigList("NCONEL");        //Number of connection elements between parallel frames
    AddConfigList("NVCONEL");       //Number of vertical connection elements
    AddConfigList("NNPBF");         //Number of nodal points along a floor line for each frame
    
    //Convergence
    AddConfigList("MIG");           //Maximum number of global iterations
    AddConfigList("NC");            //Number of columns to recieve P-Delta loads from unmodeled frames
    AddConfigList("MTP");           //Maximum number of turning points in Hysteretic Models (use 20)
    AddConfigList("NDIM");          //Maximum Number of turning point locations (Use 100000)
    
    //Analysis Properties
    AddConfigList("NSS");           //Number of static load steps
    AddConfigList("NDS");           //Number of dynamic load steps
    AddConfigList("ExTH");          //Extra Time History
    AddConfigList("NRTH");          //Number of response time histories
    AddConfigList("DT");            //TIme interval for Dynamic Response - Same as EQ Time Step
    AddConfigList("BETA");          //Newmark Integration Parameter; 0 = Central Difference, 1/4 = Const Average, 1/6 = Linear Average
    AddConfigList("GAMMA");         //Newmark Integration Parameter; 0.5
    AddConfigList("A0");            //Damping Parameter C = A0*M + A1*K
    AddConfigList("A1");            //Damping Parameter C = A0*M + A1*K
    AddConfigList("A2");            //Floor-to-Floor velocity at which shear dampers yield
    AddConfigList("A3");            //Multiplier of yield strength
    AddConfigList("AGRAV");         //Acceleration due to gravity
    AddConfigList("TOL1");          //Force Tolerance for global iterations
    AddConfigList("TOL3");          //Moment Tolerance for global iterations
    AddConfigList("TOL5");          //Force Tolerance for local Iterations
    AddConfigList("TOL7");          //Moment Tolerance for local iterations
    AddConfigList("ALPHAC");        //Connecton element stiffness
    AddConfigList("IRINT");         //Output Interval for response time histories
    AddConfigList("IROUT");         //If 1 also response time histories to unit 4, 0 if not
    AddConfigList("ISTOP");         //Time step at which current dynamic analysis stops
    AddConfigList("SEED");          //Seed Value
    
    //Fiber Properties
    AddConfigList("EEC");           //Axial load eccentricity factor for braces
    AddConfigList("NSEFBC");        //Number of fiber segements for beams or columns (8)
    AddConfigList("NSEFBR");        //Number of Fiber Segments for Braces (7)
    AddConfigList("MILF");          //Maximum Number of element iterations (20)
    AddConfigList("FiberArea");
    AddConfigList("FRAC-BC");       //Segment lengths for beam and column elements
    AddConfigList("FRAC-BR");       //Segment lengths for brace elements

    //Damping
    AddConfigList("FIRSTMODEPERIOD");   //Period of the first mode of the structure, if left blank assume 0.1*N where N = number of stories
    AddConfigList("BASESHEARPERCENT");  //Percent of values used to calculate A2
    AddConfigList("DAMPINGRATIOSTIFF"); //Stiffness proportial rayleigh damping coefficient.
    AddConfigList("DAMPINGRATIOCOL");   //Damping ratio in Columns used to calculate of A2
    AddConfigList("R");             //Ratio = F_push/F_eq_des
    AddConfigList("BASESHEAR");     //Pushover Base Shear
    AddConfigList("BASEDRIFT");     //Drift of the base floor

    //Foundation Nodes
    AddConfigList("DefFndNode");    //Default Properties for Foundation Nodes
    AddConfigList("FndNode");       //Override Properties for Foundation Nodes

    //Basement Wall Elements
    AddConfigList("DefWallShearMod");  //Default Shear Modulus for Basement Wall Elements

    //Load Options
    AddConfigList("LOADCOMBO");     //Name of ETABS load combination to use for loads on steel model
    AddConfigList("MASSCOMBO");     //Name of ETABS load combination to use for mass on steel model
    
    //Vertical Constraints
    AddConfigList("ALPHAVC");       //Vertical stiffness constraint specifier
    AddConfigList("ALPHAVCDEF");    //Default Vertical Connection Stiffness Specifier

    //Output
    AddConfigList("TimeHistory");
    
    //Extra Config Properties
    AddConfigList("PRIMARYETABSDIR"); //DIrection in Etabs model to use for primary frames
    AddConfigList("PlotAll");       //Toggle for plotting every node's x and y displacement
    AddConfigList("PlotSecondary"); //Toggle for plotting secondary Nodes
    //AddConfigList("ExNRTH");        //Number of Extra Response Time Histories
    AddConfigList("NumMaterial");   //Number of Material Models
    AddConfigList("MAT");           //Material Properties
    AddConfigList("MatProp");       //Actually stores material line
    AddConfigList("MATERIALCONV");  //Converts between ETABS material and Steel material
    AddConfigList("SteelMat");      //Steel Properties
    AddConfigList("ConcreteMat");   //Concrete Properties
    
    
    
    //Section information
    AddConfigList("STEELSECTION");  //File name of steel section database file
    AddConfigList("SLABSECTION");   //File name of slab section databse file
    
    //File Locaiton Information
    AddConfigList("SAVELOC");       //Location where the input and output files will be saved to (not including trailing /)
    AddConfigList("ETABSTITLE");    //Title of input etabs file
    AddConfigList("ETABSLOC");      //Location of etabs input file
    
    //Debug Info
    AddConfigList("DEBUGLEVEL");         //Level out debug information 0-3, 0 = failures
    AddConfigList("SECTIONCONVERSION"); //Toggle to enable output of section conversion table
    AddConfigList("MATCONVERSION"); //Toggle to enable output of material conversion table
    
    
    //Garuda Controls
    AddConfigList("USERNAME");      //Garuda Username
    AddConfigList("PASSWORD");      //Garuda Password
    AddConfigList("GARUDASAVELOC"); //Location on Garuda where analysis will be run
    AddConfigList("COPYTOGARUDA");  //Toggle wether to copy files to garuda


    



}



std::string Config::RemoveComment(std::string line)
{
    std::string::iterator strItr;
    unsigned int comLoc = 0;
    unsigned int numSpace = 0;
    for (strItr = line.begin(); strItr != line.end(); strItr++)
    {
        if (*strItr == ' ')
            numSpace += 1;
        else if (*strItr == '%')
            break;
        else
            numSpace = 0;
        comLoc++;
    }
    std::string cleanString = line.substr(0, comLoc - numSpace);

    //std::cout<<"Original: "<<line<<std::endl;
    //std::cout<<"Returning: "<<cleanString<<std::endl;

    return cleanString;
}



void Config::ParseConfig()
{
    d_out dout;
    
    std::stringstream savePath;
    savePath<<workingDirectory << "/" << "Etabs2Steel.config";

    std::ifstream configH(savePath.str());

    if (!configH)
    {
        dout<<"Can't open Config File. Terminating Analysis. Searching at: "<<workingDirectory<<"\n";
        exit(0);
    }
    
    configH.exceptions(configH.exceptions()|std::ios_base::badbit); //throw for bad()
    
   
    
    std::string line;
    std::regex configR(R"(\s*\[(.+)\]\s+(.+))");
    std::smatch matches;
    //Parse the file
    while (getline(configH,line))
    {
        //Remove Comments
        line = RemoveComment(line);

        if (std::regex_search(line, matches, configR)) //Found a Config Option
        {
            //Store Config Option Name
            std::string optName = matches[1].str();
            
            //Store config line to its appropriate list
            AddRawConfigVal(optName, matches[2].str());
        }
    }
    
    //SaveConfg
}






std::ostream& operator<<(std::ostream& os, Config& config)
{

    //Get variables
    std::string title = *config.GetConfigStart("TITLE");
    
    //Title
    os<<title<<std::endl;
    
    

    unsigned long nnp = std::stol(*config.GetConfigStart("NNP"));
    unsigned long nel = std::stol(*config.GetConfigStart("NEL"));
    unsigned int nnpfn = std::stoi(*config.GetConfigStart("NNPFN"));
    unsigned int nbel = std::stoi(*config.GetConfigStart("NBEL"));
    unsigned int nconel = std::stoi(*config.GetConfigStart("NCONEL"));
    unsigned int nnpbf = std::stoi(*config.GetConfigStart("NNPBF"));
    unsigned int nvconel = std::stoi(*config.GetConfigStart("NVCONEL"));
    unsigned int nss = std::stoi(*config.GetConfigStart("NSS"));

    std::string nds = *config.GetConfigStart("NDS");
    unsigned long nrth = std::stol(*config.GetConfigStart("NRTH"));
    
    unsigned int mig = std::stoi(*config.GetConfigStart("MIG"));
    unsigned int nc = std::stoi(*config.GetConfigStart("NC"));
    unsigned int mtp = std::stoi(*config.GetConfigStart("MTP"));
    unsigned int ndim = std::stoi(*config.GetConfigStart("NDIM"));
    
    //NNP, NEL, NNPFN, NBEL, NCONEL, NNPBF, NVCONEL, NSS, NDS, NRTH, MIG, NC, MTP, NDIM
    os<<nnp<<" "<<nel<<" "<<nnpfn<<" "<<nbel<<" "<<nconel<<" "<<nnpbf<<" "<<nvconel<<" "<<nss<<" "<<nds<<" "<<nrth<<" "<<mig<<" "<<nc<<" "<<mtp<<" "<<ndim<<std::endl;
    
    
    float dt = std::stof(*config.GetConfigStart("DT"));
    float beta = std::stof(*config.GetConfigStart("BETA"));
    float gamma = std::stof(*config.GetConfigStart("GAMMA"));
    float a0 = std::stof(*config.GetConfigStart("A0"));
    float a1 = std::stof(*config.GetConfigStart("A1"));
    float a2 = std::stof(*config.GetConfigStart("A2"));
    float a3 = std::stof(*config.GetConfigStart("A3"));
    float agrav = std::stof(*config.GetConfigStart("AGRAV"));
    float tol1 = std::stof(*config.GetConfigStart("TOL1"));
    float tol3 = std::stof(*config.GetConfigStart("TOL3"));
    float tol5 = std::stof(*config.GetConfigStart("TOL5"));
    float tol7 = std::stof(*config.GetConfigStart("TOL7"));
    float alphac = std::stof(*config.GetConfigStart("ALPHAC"));
    
    //DT, BETA, GAMMA, A0, A1, A2, A3, AGRAV, (TOL(I), I=1,7,2)
    os<<dt<<" "<<beta<<" "<<gamma<<" "<<a0<<" "<<a1<<" "<<a2<<" "<<a3<<" "<<agrav<<" "<<tol1<<" "<<tol3<<" "<<tol5<<" "<<tol7<<" "<<alphac<<std::endl;
    
    
    float eec = std::stof(*config.GetConfigStart("EEC"));
    float nsefbc = std::stof(*config.GetConfigStart("NSEFBC"));
    float nsefr = std::stof(*config.GetConfigStart("NSEFBR"));
    float milf = std::stof(*config.GetConfigStart("MILF"));
    
    //EEC, NSEFBC, NSEFBR, MILF
    os<<eec<<" "<<nsefbc<<" "<<nsefr<<" "<<milf<<std::endl;


    float irint = std::stof(*config.GetConfigStart("IRINT"));
    int irout = std::stoi(*config.GetConfigStart("IROUT"));
    
    std::string istop;
    if (config.GetConfigSize("ISTOP") == 0)
        istop = "ASNI3";
    else
        istop = *config.GetConfigStart("ISTOP");
    
    //IRINT, IROUT, ISTOP
    os<<irint<<" "<<irout<<" "<<istop<<std::endl;
    

    
    return os;
}