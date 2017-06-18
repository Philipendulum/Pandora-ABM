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
    talkingRadius = getParamInt("agents", "talkingRadius");
    talkingProb = getParamInt("agents", "talkingProb");


    floorNumber = getParamInt("environment", "floorNumber");
    doorNumber = getParamInt("environment", "doorNumber"); 
    maxDoorWidth = getParamInt("environment", "maxDoorWidth"); // maximum doorwidth for iteration of currgoal vector.
    jumpNumber = getParamInt("environment", "jumpNumber"); 
    turnNumber = getParamInt("environment", "turnNumber");    

    // signs
    Engine::Point2D<int> sign, sign2;
    sign._x = getParamInt("signs/sign1", "x");
    sign._y = getParamInt("signs/sign1", "y");
    signList.push_back(sign);
    sign2._x = getParamInt("signs/sign2", "x");
    sign2._y = getParamInt("signs/sign2", "y");
    signList.push_back(sign2);

    // doors - Change from engine::point 2 to struct?!?!?!!?
    /*Engine::Point2D<int> door, door2;
    door._x = getParamInt("doors/door1", "x");
    door._y = getParamInt("doors/door1", "y");
    doorList.push_back(door);    
    door2._x = getParamInt("doors/door2", "x");
    door2._y = getParamInt("doors/door2", "y");
    doorList.push_back(door2);  */

    // doors 
 
    door doors[doorNumber]; 
    //doors = malloc(doorNumber*sizeof(door));
   /* int m; // TIS IS THE LOOP VERSION, UNCOMMENT IT AFTER
    for (m=1; m<= doorNumber; m++){
    doors[m-1].location._x = getParamInt("doors/door", "x"); // NEED TO GET SOME READSTRING SCRIPT TO INPUT 100s of doors.like "doors/"+getString("door"+char(m))
    doors[m-1].location._y = getParamInt("doors/door", "y");    
    doors[m-1]._DIR = getParamInt("doors/door", "DIR");
    doors[m-1]._VALUE = getParamInt("doors/door", "VALUE"); 
    }*/

    doors[0].location._x = getParamInt("doors/door1", "x");
    doors[0].location._y = getParamInt("doors/door1", "y");   
    doors[0]._DIR = getParamInt("doors/door1", "DIR");
    doors[0]._VALUE = getParamInt("doors/door1", "VALUE");
    doorList.push_back(doors[0].location);
    doorValueList.push_back(doors[0]._VALUE);
    doorDirList.push_back(doors[0]._DIR);   

    doors[1].location._x = getParamInt("doors/door2", "x");
    doors[1].location._y = getParamInt("doors/door2", "y");    
    doors[1]._DIR = getParamInt("doors/door2", "DIR");
    doors[1]._VALUE = getParamInt("doors/door2", "VALUE"); 
    doorList.push_back(doors[1].location);
    doorValueList.push_back(doors[1]._VALUE);
    doorDirList.push_back(doors[1]._DIR);   



    doors[2].location._x = getParamInt("doors/door3", "x");
    doors[2].location._y = getParamInt("doors/door3", "y");    
    doors[2]._DIR = getParamInt("doors/door3", "DIR");
    doors[2]._VALUE = getParamInt("doors/door3", "VALUE"); 
    doorList.push_back(doors[2].location);
    doorValueList.push_back(doors[2]._VALUE);
    doorDirList.push_back(doors[2]._DIR);   



    doors[3].location._x = getParamInt("doors/door4", "x");
    doors[3].location._y = getParamInt("doors/door4", "y");    
    doors[3]._DIR = getParamInt("doors/door4", "DIR");
    doors[3]._VALUE = getParamInt("doors/door4", "VALUE"); 
    doorList.push_back(doors[3].location);
    doorValueList.push_back(doors[3]._VALUE);
    doorDirList.push_back(doors[3]._DIR);   



    doors[4].location._x = getParamInt("doors/door5", "x");
    doors[4].location._y = getParamInt("doors/door5", "y");    
    doors[4]._DIR = getParamInt("doors/door5", "DIR");
    doors[4]._VALUE = getParamInt("doors/door5", "VALUE"); 
    doorList.push_back(doors[4].location);
    doorValueList.push_back(doors[4]._VALUE);
    doorDirList.push_back(doors[4]._DIR);   


    doors[5].location._x = getParamInt("doors/door6", "x");
    doors[5].location._y = getParamInt("doors/door6", "y");    
    doors[5]._DIR = getParamInt("doors/door6", "DIR");
    doors[5]._VALUE = getParamInt("doors/door6", "VALUE"); 
    doorList.push_back(doors[5].location);
    doorValueList.push_back(doors[5]._VALUE);
    doorDirList.push_back(doors[5]._DIR);   


    doors[6].location._x = getParamInt("doors/door7", "x");
    doors[6].location._y = getParamInt("doors/door7", "y");    
    doors[6]._DIR = getParamInt("doors/door7", "DIR");
    doors[6]._VALUE = getParamInt("doors/door7", "VALUE"); 
    doorList.push_back(doors[6].location);
    doorValueList.push_back(doors[6]._VALUE);
    doorDirList.push_back(doors[6]._DIR);   



    doors[7].location._x = getParamInt("doors/door8", "x");
    doors[7].location._y = getParamInt("doors/door8", "y");    
    doors[7]._DIR = getParamInt("doors/door8", "DIR");
    doors[7]._VALUE = getParamInt("doors/door8", "VALUE"); 
    doorList.push_back(doors[7].location);
    doorValueList.push_back(doors[7]._VALUE);
    doorDirList.push_back(doors[7]._DIR);   


    doors[8].location._x = getParamInt("doors/door9", "x");
    doors[8].location._y = getParamInt("doors/door9", "y");    
    doors[8]._DIR = getParamInt("doors/door9", "DIR");
    doors[8]._VALUE = getParamInt("doors/door9", "VALUE"); 
    doorList.push_back(doors[8].location);
    doorValueList.push_back(doors[8]._VALUE);
    doorDirList.push_back(doors[8]._DIR);   

    doors[9].location._x = getParamInt("doors/door10", "x");
    doors[9].location._y = getParamInt("doors/door10", "y");    
    doors[9]._DIR = getParamInt("doors/door10", "DIR");
    doors[9]._VALUE = getParamInt("doors/door10", "VALUE"); 
    doorList.push_back(doors[9].location);
    doorValueList.push_back(doors[9]._VALUE);
    doorDirList.push_back(doors[9]._DIR);   



    doors[10].location._x = getParamInt("doors/door11", "x");
    doors[10].location._y = getParamInt("doors/door11", "y");    
    doors[10]._DIR = getParamInt("doors/door11", "DIR");
    doors[10]._VALUE = getParamInt("doors/door11", "VALUE");
    doorList.push_back(doors[10].location);
    doorValueList.push_back(doors[10]._VALUE);
    doorDirList.push_back(doors[10]._DIR);   


    doors[11].location._x = getParamInt("doors/door12", "x");
    doors[11].location._y = getParamInt("doors/door12", "y");    
    doors[11]._DIR = getParamInt("doors/door12", "DIR");
    doors[11]._VALUE = getParamInt("doors/door12", "VALUE"); 
    doorList.push_back(doors[11].location);
    doorValueList.push_back(doors[11]._VALUE);
    doorDirList.push_back(doors[11]._DIR);   



    doors[12].location._x = getParamInt("doors/door13", "x");
    doors[12].location._y = getParamInt("doors/door13", "y");    
    doors[12]._DIR = getParamInt("doors/door13", "DIR");
    doors[12]._VALUE = getParamInt("doors/door13", "VALUE");
    doorList.push_back(doors[12].location);
    doorValueList.push_back(doors[12]._VALUE);
    doorDirList.push_back(doors[12]._DIR);   

 

    doors[13].location._x = getParamInt("doors/door14", "x");
    doors[13].location._y = getParamInt("doors/door14", "y");    
    doors[13]._DIR = getParamInt("doors/door14", "DIR");
    doors[13]._VALUE = getParamInt("doors/door14", "VALUE");
    doorList.push_back(doors[13].location);
    doorValueList.push_back(doors[13]._VALUE);
    doorDirList.push_back(doors[13]._DIR);   

 

    doors[14].location._x = getParamInt("doors/door15", "x");
    doors[14].location._y = getParamInt("doors/door15", "y");    
    doors[14]._DIR = getParamInt("doors/door15", "DIR");
    doors[14]._VALUE = getParamInt("doors/door15", "VALUE");
    doorList.push_back(doors[14].location);
    doorValueList.push_back(doors[14]._VALUE);
    doorDirList.push_back(doors[14]._DIR);   

 

    doors[15].location._x = getParamInt("doors/door16", "x");
    doors[15].location._y = getParamInt("doors/door16", "y");    
    doors[15]._DIR = getParamInt("doors/door16", "DIR");
    doors[15]._VALUE = getParamInt("doors/door16", "VALUE");
    doorList.push_back(doors[15].location);
    doorValueList.push_back(doors[15]._VALUE);
    doorDirList.push_back(doors[15]._DIR);   

 

    doors[16].location._x = getParamInt("doors/door17", "x");
    doors[16].location._y = getParamInt("doors/door17", "y");    
    doors[16]._DIR = getParamInt("doors/door17", "DIR");
    doors[16]._VALUE = getParamInt("doors/door17", "VALUE");
    doorList.push_back(doors[16].location);
    doorValueList.push_back(doors[16]._VALUE);
    doorDirList.push_back(doors[16]._DIR);   

 

    doors[17].location._x = getParamInt("doors/door18", "x");
    doors[17].location._y = getParamInt("doors/door18", "y");    
    doors[17]._DIR = getParamInt("doors/door18", "DIR");
    doors[17]._VALUE = getParamInt("doors/door18", "VALUE"); 
    doorList.push_back(doors[17].location);
    doorValueList.push_back(doors[17]._VALUE);
    doorDirList.push_back(doors[17]._DIR);   



    doors[18].location._x = getParamInt("doors/door19", "x");
    doors[18].location._y = getParamInt("doors/door19", "y");    
    doors[18]._DIR = getParamInt("doors/door19", "DIR");
    doors[18]._VALUE = getParamInt("doors/door19", "VALUE"); 
    doorList.push_back(doors[18].location);
    doorValueList.push_back(doors[18]._VALUE);
    doorDirList.push_back(doors[18]._DIR);   



    doors[19].location._x = getParamInt("doors/door20", "x");
    doors[19].location._y = getParamInt("doors/door20", "y");    
    doors[19]._DIR = getParamInt("doors/door20", "DIR");
    doors[19]._VALUE = getParamInt("doors/door20", "VALUE"); 
    doorList.push_back(doors[19].location);
    doorValueList.push_back(doors[19]._VALUE);
    doorDirList.push_back(doors[19]._DIR);   



    doors[20].location._x = getParamInt("doors/door21", "x");
    doors[20].location._y = getParamInt("doors/door21", "y");    
    doors[20]._DIR = getParamInt("doors/door21", "DIR");
    doors[20]._VALUE = getParamInt("doors/door21", "VALUE");
    doorList.push_back(doors[20].location);
    doorValueList.push_back(doors[20]._VALUE);
    doorDirList.push_back(doors[20]._DIR);   

 

    doors[21].location._x = getParamInt("doors/door22", "x");
    doors[21].location._y = getParamInt("doors/door22", "y");    
    doors[21]._DIR = getParamInt("doors/door22", "DIR");
    doors[21]._VALUE = getParamInt("doors/door22", "VALUE");
    doorList.push_back(doors[21].location);
    doorValueList.push_back(doors[21]._VALUE);
    doorDirList.push_back(doors[21]._DIR);   

 

    doors[22].location._x = getParamInt("doors/door23", "x");
    doors[22].location._y = getParamInt("doors/door23", "y");    
    doors[22]._DIR = getParamInt("doors/door23", "DIR");
    doors[22]._VALUE = getParamInt("doors/door23", "VALUE");
    doorList.push_back(doors[22].location);
    doorValueList.push_back(doors[22]._VALUE);
    doorDirList.push_back(doors[22]._DIR);   

 

    doors[23].location._x = getParamInt("doors/door24", "x");
    doors[23].location._y = getParamInt("doors/door24", "y");    
    doors[23]._DIR = getParamInt("doors/door24", "DIR");
    doors[23]._VALUE = getParamInt("doors/door24", "VALUE");
    doorList.push_back(doors[23].location);
    doorValueList.push_back(doors[23]._VALUE);
    doorDirList.push_back(doors[23]._DIR);   

 

    doors[24].location._x = getParamInt("doors/door25", "x");
    doors[24].location._y = getParamInt("doors/door25", "y");    
    doors[24]._DIR = getParamInt("doors/door25", "DIR");
    doors[24]._VALUE = getParamInt("doors/door25", "VALUE");
    doorList.push_back(doors[24].location);
    doorValueList.push_back(doors[24]._VALUE);
    doorDirList.push_back(doors[24]._DIR);   

 

    doors[25].location._x = getParamInt("doors/door26", "x");
    doors[25].location._y = getParamInt("doors/door26", "y");    
    doors[25]._DIR = getParamInt("doors/door26", "DIR");
    doors[25]._VALUE = getParamInt("doors/door26", "VALUE");
    doorList.push_back(doors[25].location);
    doorValueList.push_back(doors[25]._VALUE);
    doorDirList.push_back(doors[25]._DIR);   

 

    doors[26].location._x = getParamInt("doors/door27", "x");
    doors[26].location._y = getParamInt("doors/door27", "y");    
    doors[26]._DIR = getParamInt("doors/door27", "DIR");
    doors[26]._VALUE = getParamInt("doors/door27", "VALUE");
    doorList.push_back(doors[26].location);
    doorValueList.push_back(doors[26]._VALUE);
    doorDirList.push_back(doors[26]._DIR);   

 

    doors[27].location._x = getParamInt("doors/door28", "x");
    doors[27].location._y = getParamInt("doors/door28", "y");    
    doors[27]._DIR = getParamInt("doors/door28", "DIR");
    doors[27]._VALUE = getParamInt("doors/door28", "VALUE"); 
    doorList.push_back(doors[27].location);
    doorValueList.push_back(doors[27]._VALUE);
    doorDirList.push_back(doors[27]._DIR);   



    doors[28].location._x = getParamInt("doors/door29", "x");
    doors[28].location._y = getParamInt("doors/door29", "y");    
    doors[28]._DIR = getParamInt("doors/door29", "DIR");
    doors[28]._VALUE = getParamInt("doors/door29", "VALUE");
    doorList.push_back(doors[28].location);
    doorValueList.push_back(doors[28]._VALUE);
    doorDirList.push_back(doors[28]._DIR);   

 

    doors[29].location._x = getParamInt("doors/door30", "x");
    doors[29].location._y = getParamInt("doors/door30", "y");    
    doors[29]._DIR = getParamInt("doors/door30", "DIR");
    doors[29]._VALUE = getParamInt("doors/door30", "VALUE"); 
    doorList.push_back(doors[29].location);
    doorValueList.push_back(doors[29]._VALUE);
    doorDirList.push_back(doors[29]._DIR);   



    doors[30].location._x = getParamInt("doors/door31", "x");
    doors[30].location._y = getParamInt("doors/door31", "y");    
    doors[30]._DIR = getParamInt("doors/door31", "DIR");
    doors[30]._VALUE = getParamInt("doors/door31", "VALUE"); 
    doorList.push_back(doors[30].location);
    doorValueList.push_back(doors[30]._VALUE);
    doorDirList.push_back(doors[30]._DIR);   



    doors[31].location._x = getParamInt("doors/door32", "x");
    doors[31].location._y = getParamInt("doors/door32", "y");    
    doors[31]._DIR = getParamInt("doors/door32", "DIR");
    doors[31]._VALUE = getParamInt("doors/door32", "VALUE");
    doorList.push_back(doors[31].location);
    doorValueList.push_back(doors[31]._VALUE);
    doorDirList.push_back(doors[31]._DIR);   

 
    
    // turn positions


    Engine::Point2D<int> turn, turn2, turn3, turn4, turn5, turn6;
    turn._x = getParamInt("turns/turn1", "x");
    turn._y = getParamInt("turns/turn1", "y");
    turnList.push_back(turn);
    turn2._x = getParamInt("turns/turn2", "x");
    turn2._y = getParamInt("turns/turn2", "y");
    turnList.push_back(turn2);
    turn3._x = getParamInt("turns/turn3", "x");
    turn3._y = getParamInt("turns/turn3", "y");
    turnList.push_back(turn3);
    turn4._x = getParamInt("turns/turn4", "x");
    turn4._y = getParamInt("turns/turn4", "y");
    turnList.push_back(turn4);
    /*turn5._x = getParamInt("turns/turn5", "x");
    turn5._y = getParamInt("turns/turn5", "y");
    turnList.push_back(turn5);
    turn6._x = getParamInt("turns/turn6", "x");
    turn6._y = getParamInt("turns/turn6", "y");
    turnList.push_back(turn6);*/

   
    // jump positions
    Engine::Point2D<int> jfrom, jto;
    jumping jumps[jumpNumber];    

    //jumps = malloc(jumpNumber*sizeof(jumping));
    //int n;
    //for (n=1; n<= jumpNumber; n++){
    jumps[0].jfrom._x = getParamInt("jumps/jump1", "xfrom"); // NEED TO GET SOME READSTRING SCRIPT TO INPUT 100s of doors.like "doors/"+getString("door"+char(m))
    jumps[0].jfrom._y = getParamInt("jumps/jump1", "yfrom");
    jumps[0].jto._x = getParamInt("jumps/jump1", "xto");
    jumps[0].jto._y = getParamInt("jumps/jump1", "yto");
    jumpCellsfrom.push_front(jfrom);
    jumpCellsto.push_front(jto);
    //}

    jumps[1].jfrom._x = getParamInt("jumps/jump2", "xfrom"); 
    jumps[1].jfrom._y = getParamInt("jumps/jump2", "yfrom");
    jumps[1].jto._x = getParamInt("jumps/jump2", "xto");
    jumps[1].jto._y = getParamInt("jumps/jump2", "yto");
    jumpCellsfrom.push_front(jfrom);
    jumpCellsto.push_front(jto);

    jumps[2].jfrom._x = getParamInt("jumps/jump3", "xfrom"); 
    jumps[2].jfrom._y = getParamInt("jumps/jump3", "yfrom");
    jumps[2].jto._x = getParamInt("jumps/jump3", "xto");
    jumps[2].jto._y = getParamInt("jumps/jump3", "yto");
    jumpCellsfrom.push_front(jfrom);
    jumpCellsto.push_front(jto);

    jumps[3].jfrom._x = getParamInt("jumps/jump4", "xfrom"); 
    jumps[3].jfrom._y = getParamInt("jumps/jump4", "yfrom");
    jumps[3].jto._x = getParamInt("jumps/jump4", "xto");
    jumps[3].jto._y = getParamInt("jumps/jump4", "yto");
    jumpCellsfrom.push_front(jfrom);
    jumpCellsto.push_front(jto);


    // exits - add more similar lines if you have more exits (since usuallyt there is 2-3 exits
    Engine::Point2D<int> ext, ext2, ext3, ext4, ext5, ext6;
    ext._x = getParamInt("exits/exit1", "x");
    ext._y = getParamInt("exits/exit1", "y");
    exitconfiglist.push_back(ext);
    ext2._x = getParamInt("exits/exit2", "x");
    ext2._y = getParamInt("exits/exit2", "y");
    exitconfiglist.push_back(ext2);
    ext3._x = getParamInt("exits/exit3", "x");
    ext3._y = getParamInt("exits/exit3", "y");
    exitconfiglist.push_back(ext3);
    ext._x = getParamInt("exits/exit4", "x");
    ext._y = getParamInt("exits/exit4", "y");
    exitconfiglist.push_back(ext4);
    /*ext2._x = getParamInt("exits/exit5", "x");
    ext2._y = getParamInt("exits/exit5", "y");
    exitconfiglist.push_back(ext5);
    ext3._x = getParamInt("exits/exit6", "x");
    ext3._y = getParamInt("exits/exit6", "y");
    exitconfiglist.push_back(ext6);*/

}

} // namespace Evacuation


