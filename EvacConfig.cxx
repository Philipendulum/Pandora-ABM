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

    // Newly Added parameters still not in the config file || FINISH THE CORRECT PATH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    _numAgents = getParamInt("agents", "num");
    childPercentage = getParamInt("agents", "childPercentage");
    elderlyPercentage = getParamInt("agents", "elderlyPercentage");
    malePercentage = getParamInt("agents", "malePercentage");
    notMovedPanicTreshold = getParamInt("agents", "notMovedPanicTreshold");


    floorNumber = getParamInt("environment", "floorNumber");

    // signs
    Engine::Point2D<int> sign, sign2;
    sign._x = getParamInt("signs/sign1", "x");
    sign._y = getParamInt("signs/sign1", "y");
    signList.push_back(sign);
    sign2._x = getParamInt("signs/sign2", "x");
    sign2._y = getParamInt("signs/sign2", "y");
    signList.push_back(sign2);

    // doors
    Engine::Point2D<int> door, door2;
    door._x = getParamInt("doors/door1", "x");
    door._y = getParamInt("doors/door1", "y");
    doorList.push_back(door);    
    door2._x = getParamInt("doors/door2", "x");
    door2._y = getParamInt("doors/door2", "y");
    doorList.push_back(door2);    
    // exits
    Engine::Point2D<int> ext, ext2;
    ext._x = getParamInt("exits/exit1", "x");
    ext._y = getParamInt("exits/exit1", "y");
    exitconfiglist.push_back(ext);
    ext2._x = getParamInt("exits/exit2", "x");
    ext2._y = getParamInt("exits/exit2", "y");
    exitconfiglist.push_back(ext2);

}

} // namespace Evacuation


