#include <EvacConfig.hxx>

namespace Evacuation
{

EvacConfig::EvacConfig( const std::string & xmlFile ) : Config(xmlFile)
{
}

EvacConfig::~EvacConfig()
{
}

void EvacConfig::loadParams()
{
    int n;
    _numAgents = getParamInt("agents", "num");
    initialPanicked = getParamInt("agents", "initialPanicked");
    childPercentage = getParamInt("agents", "childPercentage");
    elderlyPercentage = getParamInt("agents", "elderlyPercentage");
    malePercentage = getParamInt("agents", "malePercentage");
    adultSpeed = getParamInt("agents", "adultSpeed");  
    elderlySpeed = getParamInt("agents", "elderlySpeed");  
    childSpeed = getParamInt("agents", "childSpeed");  
    adultStairsSpeed = getParamInt("agents", "adultStairsSpeed");  
    elderlyStairsSpeed = getParamInt("agents", "elderlyStairsSpeed");  
    childStairsSpeed = getParamInt("agents", "childStairsSpeed");  
    notMovedPanicTreshold = getParamInt("agents", "notMovedPanicTreshold");
    talkingRadius = getParamInt("agents", "talkingRadius");
    talkingProb = getParamInt("agents", "talkingProb");
    getuptime = getParamInt("agents", "getuptime");
    visionAdult = getParamInt("agents", "visionAdult");
    visionElderly = getParamInt("agents", "visionElderly");
    visionChild = getParamInt("agents", "visionChild");
    childTrampling = getParamFloat("agents", "childTrampling");
    manTrampling = getParamFloat("agents", "manTrampling");
    womanTrampling = getParamFloat("agents", "womanTrampling");
    elderlyTrampling = getParamFloat("agents", "elderlyTrampling");
    elderlyWeightMean = getParamFloat("agents", "elderlyWeightMean"); 
    elderlyWeightStDev = getParamFloat("agents", "elderlyWeightStDev"); 
    childWeightMean = getParamFloat("agents", "childWeightMean"); 
    childWeightStDev = getParamFloat("agents", "childWeightStDev"); 
    manWeightMean = getParamFloat("agents", "manWeightMean"); 
    manWeightStDev = getParamFloat("agents", "manWeightStDev"); 
    womanWeightMean = getParamFloat("agents", "womanWeightMean"); 
    womanWeightStDev = getParamFloat("agents", "womanWeightStDev");    

    evacuationThreshold = getParamInt("environment", "evacuationThreshold");
    floorNumber = getParamInt("environment", "floorNumber");
    doorNumber = getParamInt("environment", "doorNumber"); 
    maxDoorWidth = getParamInt("environment", "maxDoorWidth"); // maximum doorwidth for iteration of currgoal vector.
    jumpNumber = getParamInt("environment", "jumpNumber"); 
    turnNumber = getParamInt("environment", "turnNumber"); 
    exitNumber = getParamInt("environment", "exitNumber");
    signNumber = getParamInt("environment", "signNumber");
    systemRasters = getParamInt("environment", "systemRasters");
    delayMin = getParamFloat("environment", "delayMin");
    delayMax = getParamFloat("environment", "delayMax");
    chemoDecayRate = getParamInt("environment", "chemoDecayRate");

    // signs
    Engine::Point2D<int> signs[signNumber];
    for (n=1; n<= signNumber; n++){
        std::ostringstream s;
	s << "signs/sign" << n;     
        signs[n-1]._x = getParamInt(s.str(), "x");
        signs[n-1]._y = getParamInt(s.str(), "y");
        signList.push_back(signs[n-1]);
        }

    // doors 
 
    door doors[doorNumber]; 
    for (n=1; n<= doorNumber; n++)
        {
        std::ostringstream d;
	d << "doors/door" << n;     
        doors[n-1].location._x = getParamInt(d.str(), "x");
        doors[n-1].location._y = getParamInt(d.str(), "y");   
        doors[n-1]._DIR = getParamInt(d.str(), "DIR");
        doors[n-1]._VALUE = getParamInt(d.str(), "VALUE");
        doorList.push_back(doors[n-1].location);
        doorValueList.push_back(doors[n-1]._VALUE);
        doorDirList.push_back(doors[n-1]._DIR); 
        } 
 
    // turn positions
    Engine::Point2D<int> turn[turnNumber];
    for (n=1; n<= turnNumber; n++){
        std::ostringstream t;
	t << "turns/turn" << n;    
        turn[n-1]._x = getParamInt(t.str(), "x");
        turn[n-1]._y = getParamInt(t.str(), "y");
        turnList.push_back(turn[n-1]);
    }

    // jump positions
    Engine::Point2D<int> jfrom, jto;
    jumping jumps[jumpNumber];    
    for (n=1; n<= jumpNumber; n++)
        {
        std::ostringstream j;
	j << "jumps/jump" << n;
        jumps[n-1].jfrom._x = getParamInt(j.str(), "xfrom");
        jumps[n-1].jfrom._y = getParamInt(j.str(), "yfrom");
        jumps[n-1].jto._x = getParamInt(j.str(), "xto");
        jumps[n-1].jto._y = getParamInt(j.str(), "yto");
        jumpCellsfrom.push_front(jumps[n-1].jfrom);
        jumpCellsto.push_front(jumps[n-1].jto); 
        }       

    // exits
    Engine::Point2D<int> ext[exitNumber];
    for (n=1; n<= exitNumber; n++)
        {
        std::ostringstream e;
	e << "exits/exit" << n;       
        ext[n-1]._x = getParamInt(e.str(), "x");
        ext[n-1]._y = getParamInt(e.str(), "y");
        exitconfiglist.push_back(ext[n-1]);
        }

}

} // namespace Evacuation


