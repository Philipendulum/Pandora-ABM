
#include "EvacWorld.hxx"
#include "EvacConfig.hxx"
#include "EvacAgent.hxx"

#include <typedefs.hxx>
#include <chrono>
#include <iostream>
#include <fstream>

// NEW PIECES
#include <World.hxx>
#include <Agent.hxx>
#include <Exception.hxx>
#include <Scheduler.hxx>
//#include <SpacePartition.hxx>
//#include <OpenMPSingleNode.hxx>

#include <GeneralState.hxx>

#include <Logger.hxx>
#include <Statistics.hxx>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
// END OF NEW PIECES

namespace Evacuation
{

EvacWorld::EvacWorld( EvacConfig * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false)
{
}

EvacWorld::~EvacWorld()
{
}

void EvacWorld::createAgents()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();

    for(int i=0; i<evacConfig._numAgents; i++)
    {
	int floor = Engine::GeneralState::statistics().getUniformDistValue(1, evacConfig.returnFloorNumber());
	char gender = ' ';

        if(Engine::GeneralState::statistics().getUniformDistValue(1,100) <= evacConfig.returnMalePerc()) 
	    gender='M';
	else
	    gender='F';
	int vision=0;
	int speed=0;
        float mass = 0;
        int stairsSpeed=0;
	int age=0;
        int panicked = 0;
        if(Engine::GeneralState::statistics().getUniformDistValue(1,100) <= evacConfig.returnInitialPanicked()){panicked = 1;}
        int directionDarkness = 0;
        int knowledge = 0;

        if ((evacConfig.returnChildPerc() + evacConfig.returnElderlyPerc()) > 100) {
             exit(8);
        }
        int b = Engine::GeneralState::statistics().getUniformDistValue(1,100);
        if (b <= evacConfig.returnChildPerc()){ age = 0;}
        else if ((b>evacConfig.returnChildPerc()) && (b<=(evacConfig.returnChildPerc() + evacConfig.returnElderlyPerc()))) { age = 2;}
        else {age = 1;}
        if (age == 1) {
            speed = evacConfig.adultSpeed; //3
            stairsSpeed = evacConfig.adultStairsSpeed; //2 
            vision = evacConfig.visionAdult;
            //std::normal_distribution<float> distribution (evacConfig.elderlyWeightMean, evacConfig.elderlyWeightStDev);
            //mass = distribution(generator);
            mass = Engine::GeneralState::statistics().getNormalDistValue(evacConfig.elderlyWeightMean, evacConfig.elderlyWeightStDev);
            if (mass > evacConfig.elderlyWeightMean + 3*evacConfig.elderlyWeightStDev) {mass = evacConfig.elderlyWeightMean + 3*evacConfig.elderlyWeightStDev;}
            else if (mass < evacConfig.elderlyWeightMean - 3*evacConfig.elderlyWeightStDev) {mass = evacConfig.elderlyWeightMean - 3*evacConfig.elderlyWeightStDev;}
            else {mass = mass;}
        }
        else if (age == 2) {
            speed = evacConfig.elderlySpeed; // 1
            stairsSpeed = evacConfig.elderlyStairsSpeed;  // 1
            vision = evacConfig.visionElderly;
            if (gender == 'F') 
                {
                //std::normal_distribution<float> distribution (evacConfig.womanWeightMean, evacConfig.womanWeightMean);
                //mass = distribution(generator);
                mass = Engine::GeneralState::statistics().getNormalDistValue(evacConfig.womanWeightMean, evacConfig.womanWeightStDev);
            	if (mass > evacConfig.womanWeightMean + 3*evacConfig.womanWeightStDev) {mass = evacConfig.womanWeightMean + 3*evacConfig.womanWeightStDev;}
            	else if (mass < evacConfig.womanWeightMean - 3*evacConfig.womanWeightStDev) {mass = evacConfig.womanWeightMean - 3*evacConfig.womanWeightStDev;}
            	else {mass = mass;}
                }
            if (gender == 'M') 
                {
                //std::normal_distribution<float> distribution (evacConfig.manWeightMean, evacConfig.manWeightMean);
                //mass = distribution(generator);
                mass = Engine::GeneralState::statistics().getNormalDistValue(evacConfig.manWeightMean, evacConfig.manWeightStDev);
            	if (mass > evacConfig.manWeightMean + 3*evacConfig.manWeightStDev) {mass = evacConfig.manWeightMean + 3*evacConfig.manWeightStDev;}
            	else if (mass < evacConfig.manWeightMean - 3*evacConfig.manWeightStDev) {mass = evacConfig.manWeightMean - 3*evacConfig.manWeightStDev;}
            	else {mass = mass;} 
                }
        }
	else if (age == 0) {
            speed = evacConfig.childSpeed; // 2
            stairsSpeed = evacConfig.childStairsSpeed;  // 1
            vision = evacConfig.visionChild;
            //std::normal_distribution<float> distribution (evacConfig.childWeightMean, evacConfig.childWeightMean);
            //mass = distribution(generator);
            mass = Engine::GeneralState::statistics().getNormalDistValue(evacConfig.childWeightMean, evacConfig.childWeightStDev);
            if (mass > evacConfig.childWeightMean + 3*evacConfig.childWeightStDev) {mass = evacConfig.childWeightMean + 3*evacConfig.childWeightStDev;}
            else if (mass < evacConfig.childWeightMean - 3*evacConfig.childWeightStDev) {mass = evacConfig.childWeightMean - 3*evacConfig.childWeightStDev;}
            else {mass = mass;}
        }
        bool exited = false;
        bool isOnStairs = false;
        bool trampled = false;
        bool justEvaded = false;
        bool touchedWall = false;
        int getuptime = evacConfig.getuptime;
        int startDelay = round (Engine::GeneralState::statistics().getExponentialDistValue(evacConfig.delayMin, evacConfig.delayMax));
        int evacDist = 0;
        int evacTime = 0;
        int notMoved = 0;
        int notMovedTotal = 0;
	std::ostringstream oss;
	oss << "EvacAgent_"<<i;
	EvacAgent * agent = new EvacAgent(oss.str(),speed, mass, stairsSpeed, floor,gender, age, vision, isOnStairs, exited, trampled, getuptime, startDelay, justEvaded, panicked, directionDarkness, touchedWall, evacDist, evacTime, notMoved, notMovedTotal, knowledge);
	addAgent(agent);
	agent->setRandomPosition();
        int floorValue = floor;
	while((getValue(eObstacles, agent->getPosition())==1) || floorValue != getValue(eFloor, agent->getPosition()) || (getValue(eDoors, agent->getPosition())==1) || (getValue(eExits, agent->getPosition())==1) || (getValue(eOccupied, agent->getPosition())>0) || (getValue(eFloor, agent->getPosition())==0) )
		{
		agent->setRandomPosition();
		}
	//Engine::Point2D<int> p (76,423);
	//agent->setPosition(p); - constructor for specified position placements
        setValue(eOccupied, agent->getPosition(), 1);
        agent->_tempNextPosition = agent->getPosition();
	//std::cout<<agent->agentCharac()<<std::endl;
        al.push_back(agent);
    }

}


void EvacWorld::createRasters()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
        std::cout << "Rasters" << std::endl;
	registerDynamicRaster("obstacles", true, eObstacles);
	getDynamicRaster(eObstacles).setInitValues(0, 1, 0);

	registerDynamicRaster("exits", true, eExits);
	getDynamicRaster(eExits).setInitValues(0, 1, 0);
        
	registerDynamicRaster("eExitedAgents", true, eExitedAgents);
	getDynamicRaster(eExitedAgents).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eSigns", true, eSigns);
	getDynamicRaster(eSigns).setInitValues(0, 1, 0);

	registerDynamicRaster("eTempCells", true, eTempCells);
	getDynamicRaster(eTempCells).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eOccupied", true, eOccupied);
	getDynamicRaster(eOccupied).setInitValues(0, 1, 0);        

	registerDynamicRaster("eStairs", true, eStairs);
	getDynamicRaster(eStairs).setInitValues(0, 4, 0);

	registerDynamicRaster("eStairsSide", true, eStairsSide);
	getDynamicRaster(eStairsSide).setInitValues(0, 2, 0);

	registerDynamicRaster("eDoors", true, eDoors);
	getDynamicRaster(eDoors).setInitValues(0, 1, 0);

	registerDynamicRaster("eRoomOrCoridor", true, eRoomOrCoridor);
	getDynamicRaster(eRoomOrCoridor).setInitValues(0, std::numeric_limits<int>::max(), 0);
        
	registerDynamicRaster("eChemoTaxiTrails", true, eChemoTaxiTrails);
	getDynamicRaster(eChemoTaxiTrails).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eFloor", true, eFloor);
	getDynamicRaster(eFloor).setInitValues(0, evacConfig.floorNumber, 0);

	registerDynamicRaster("eKnowledge", true, eKnowledge);
	getDynamicRaster(eKnowledge).setInitValues(0, 1, 0);

	registerDynamicRaster("eDoorValue", true, eDoorValue);
	getDynamicRaster(eDoorValue).setInitValues(0, 1, 0);

	registerDynamicRaster("eDoorDir", true, eDoorDir);
	getDynamicRaster(eDoorDir).setInitValues(0, 4, 0);

	registerDynamicRaster("eDoorNumber", true, eDoorNumber);
	getDynamicRaster(eDoorNumber).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eTurnNumber", true, eTurnNumber);
	getDynamicRaster(eTurnNumber).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eJumpNumber", true, eJumpNumber);
	getDynamicRaster(eJumpNumber).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("ePanic", true, ePanic);
	getDynamicRaster(ePanic).setInitValues(0, 1, 0);   

	registerDynamicRaster("eMass", true, eMass);
	getDynamicRaster(eMass).setInitValues(0, std::numeric_limits<int>::max() , 0);
  
	registerDynamicRaster("eDead", true, eDead);
	getDynamicRaster(eDead).setInitValues(0, 1, 0);  

	registerDynamicRaster("eDarkness", true, eDarkness);
	getDynamicRaster(eDarkness).setInitValues(0, 1, 0); 

	registerDynamicRaster("eTime", true, eTime);
	getDynamicRaster(eTime).setInitValues(0, _config->getNumSteps(), 0);     


// SETTING UP OBSTACLES, BORDERS, STAIRS

// SETTING UP WALLS OUTER AND INNER 
        std::cout << "Rasters2" << std::endl;
        for(auto index : getBoundaries()) 
            {
        // outer boundaries

            if ( ((index._x > 4) && (index._x < 205) && (index._y == 5)) || ((index._x > 4) && (index._x < 205) && (index._y == 204)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._x > 304) && (index._x < 505) && (index._y == 5)) || ((index._x > 304) && (index._x < 505) && (index._y == 204)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._y > 4) && (index._y < 205) && (index._x == 5)) || ((index._y > 4) && (index._y < 205) && (index._x == 204)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._y > 4) && (index._y < 205) && (index._x == 305)) || ((index._y > 4) && (index._y < 205) && (index._x == 504)) ) {setValue(eObstacles, index, 1);}           

        // now placing inner boundaries

            // floor 2
            else if ( (index._y == 50) && (index._x > 5 ) && (index._x < 121) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y < 51) && (index._y > 5) && ( (index._x == 50) || (index._x == 120) ) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 150) && (index._y > 5) && (index._y < 181) ) {setValue(eObstacles, index, 1);}
            else if ( ( (index._y == 180) || (index._y == 150)) && (index._x > 150) && (index._x < 204) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 100) && (index._x > 5) && (index._x< 61) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 60) && (index._y > 100) && (index._y < 204) ) {setValue(eObstacles, index, 1);}

            // floor 1
            else if ( (index._y == 50) && (index._x > 305) && (index._x < 504) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 450) && (index._y > 5) && (index._y < 151) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 150) && (index._x > 450) && (index._x < 504) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 100) && ( index._x > 305) && (index._x < 351) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 350) && (index._y > 100) && (index._y < 161) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 160) && (index._x > 350) && (index._x < 401) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 400) && (index._y >160) && (index._y < 204) ) {setValue(eObstacles, index, 1);}
            }
  
            // SETTING UP STAIRS RIGHT NOW 
        for(auto index : getBoundaries()) 
            {
            // floor 2 
            if ( (index._x == 191) && (index._y > 204) && (index._y < 218) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 185) && (index._y > 204) && (index._y < 218) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 188) && (index._y > 206) && (index._y < 215) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 217) && (index._x > 184) && (index._x < 192) ) {setValue(eObstacles, index, 1);}
            
            // floor 1 - STAIRS EXIT WILL BE HERE SINCE 2 floors - new 3 lines

            else if ( (index._x == 488) && (index._y > 204) && (index._y < 207) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 491) && (index._y > 204) && (index._y < 207) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 207) && (index._x > 487) && (index._x < 492) ) {setValue(eObstacles, index, 1);}
            }
        
         // setting up stairs on the eStairs raster
        for(auto index : getBoundaries()) 
            {
            if ( (index._y > 204) && (index._y < 218) && (index._x > 185) && (index._x < 191) && (getValue(eObstacles, index) != 1) ) {setValue(eStairs, index, 1);}      
            }  
        std::cout<< "NEW PART" << std::endl;
         // setting up stairs sides on the eStairsSides raster
        for(auto index : getBoundaries()) 
            {
            if ( (index._y > 204) && (index._y < 218) && (index._x > 185) && (index._x < 188) && (getValue(eObstacles, index) != 1) ) {setValue(eStairsSide, index, 1);}      
            else if ( (index._y > 204) && (index._y < 218) && (index._x > 188) && (index._x < 191) && (getValue(eObstacles, index) != 1) ) {setValue(eStairsSide, index, 2);}
            else if ( (index._y > 204) && (index._y < 218) && (index._x == 188) && (getValue(eObstacles, index) != 1) ) {setValue(eStairsSide, index, 2);} // THE MIDDLE GROUND
            }     
           
	// compute and set exit cells - MY UPDATED VERSION
        std::cout << "Placing Exits on Raster" << std::endl;
        EvacConfig::ExitConfigList::const_iterator it=evacConfig.exitconfiglist.begin();
	while(it!=evacConfig.exitconfiglist.end())
	{
            const Engine::Point2D<int> & ext = *it;
            Engine::Point2D<int> index;
            index._x = ext._x;
            index._y = ext._y;
            setValue(eExits, index, 1);
            setValue(eObstacles, index, 0); 
            it++; 
        }

        // setting DOORS - NEEDS TO BE DONE AFTER OBSTACLES AND STAIRS
        
        std::cout << "Placing Doors on Raster" << std::endl;    
    EvacConfig::DoorList::const_iterator itdoor1=evacConfig.doorList.begin();
    EvacConfig::DoorValueDirList::const_iterator itdoor2=evacConfig.doorValueList.begin();
    EvacConfig::DoorValueDirList::const_iterator itdoor3=evacConfig.doorDirList.begin();
    int amount = 1;
    int number = 1;
    while(itdoor1!=evacConfig.doorList.end())
	{
	const Engine::Point2D<int> & door = *itdoor1;
        const int & doorValue = *itdoor2;
        const int & doorDir = *itdoor3;
        Engine::Point2D<int> index;
        index._x = door._x;
        index._y = door._y;
        setValue(eDoors, index, 1); 
        setValue(eObstacles, index, 0);
        setValue(eDoorNumber, index, number);  
        if (doorValue == 1) 
            {
            setValue(eDoorValue, index, 1);
            if (doorDir == 4) 
                {
                Engine::Point2D<int> stairsornot;
                stairsornot._x = index._x + 1;
                stairsornot._y = index._y;
                setValue(eDoorDir, index, 4);
                if (getValue(eStairs, stairsornot) == 1) {setValue(eStairs, index, 2);}
                }
            else if (doorDir == 1) 
                {
                Engine::Point2D<int> stairsornot;
                stairsornot._x = index._x;
                stairsornot._y = index._y-1;
                setValue(eDoorDir, index, 1);
                if (getValue(eStairs, stairsornot) == 1) {setValue(eStairs, index, 2);}
                }
            else if (doorDir == 2) 
                {
                Engine::Point2D<int> stairsornot;
                stairsornot._x = index._x - 1;
                stairsornot._y = index._y;
                setValue(eDoorDir, index, 2);
                if (getValue(eStairs, stairsornot) == 1) {setValue(eStairs, index, 2);}
                }
            else if (doorDir == 3) 
                {
                Engine::Point2D<int> stairsornot;
                stairsornot._x = index._x;
                stairsornot._y = index._y + 1;
                setValue(eDoorDir, index, 3);
                if (getValue(eStairs, stairsornot) == 1) {setValue(eStairs, index, 2);}
                }
            }
                std::cout << "Doors done:" << amount << std::endl;
        itdoor1++;
        itdoor2++;
        itdoor3++; 
        amount++;
        number++;
        }

    // PLACING SIGNS
        std::cout << "Placing Signs on Raster" << std::endl;
    EvacConfig::SignList::const_iterator it2=evacConfig.signList.begin();
    while(it2!=evacConfig.signList.end())
	{
	const Engine::Point2D<int> & sign = *it2;
        Engine::Point2D<int> index;
        index._x = sign._x;
        index._y = sign._y;
        setValue(eSigns, index, 1); 
        it2++; 
        }

    // Placing Turn Positions
            std::cout << "Placing Turns on Raster" << std::endl;
    EvacConfig::TurnList::const_iterator itturn=evacConfig.turnList.begin();
    int turnNumber = 1;
    while(itturn!=evacConfig.turnList.end())
	{
	const Engine::Point2D<int> & turn = *itturn;
        Engine::Point2D<int> index;
        index._x = turn._x;
        index._y = turn._y;
        setValue(eStairs, index, 3);
        setValue(eTurnNumber, index, turnNumber); 
        itturn++; 
        turnNumber++;
        }    


    // Placing Jump Positions 
        std::cout << "Placing Jumps on Raster" << std::endl;
    EvacConfig::JumpCellsList::const_iterator itjump=evacConfig.jumpCellsfrom.begin();
    int jumpNumber = 1;
    while(itjump!=evacConfig.jumpCellsfrom.end())
	{
	const Engine::Point2D<int> & jump = *itjump;
        Engine::Point2D<int> index;
        index._x = jump._x;
        index._y = jump._y;
        setValue(eStairs, index, 4);
        setValue(eJumpNumber, index, jumpNumber); 
        itjump++; 
        jumpNumber++;
        }   

     // Starting to number the eRoomOrCoridor & eFloor after everything is done
// FLOORS
    for(auto index : getBoundaries()) 
        {
        if ( ( (index._x > 4) && (index._x < 205) ) && ( (index._y > 4) && (index._y < 205) ) ) 
            {
            setValue(eFloor, index, 2);
            setValue(eRoomOrCoridor, index, 1);
            } 
        else if ( ( (index._x > 304) && (index._x < 505) ) && ( (index._y > 4) && (index._y < 205) ) ) 
            {
            setValue(eFloor, index, 1);
            setValue(eRoomOrCoridor, index, 2);
            } 

        else if ( ( (index._x > 487) && (index._x < 492) ) && ( (index._y > 203) && (index._y < 208) ) ) 
            {
            setValue(eFloor, index, 1);
            setValue(eRoomOrCoridor, index, 3);
            }
        }     
// eRoomOrCoridor
    for(auto index : getBoundaries()) 
        {
        // floor 2
        if ( ( (index._x > 5) && (index._x < 50) ) && ( (index._y > 5) && (index._y < 50) ) ) 
            {
            setValue(eRoomOrCoridor, index, 4);
            //setValue(eDarkness, index, 1); // just for test
            } 
        else if ( ( (index._x > 50) && (index._x < 120) ) && ( (index._y > 5) && (index._y < 50) ) ) 
            {
            setValue(eRoomOrCoridor, index, 5);
            }
        else if ( ( (index._x > 150) && (index._x < 204) ) && ( (index._y > 5) && (index._y < 150) ) ) 
            {
            setValue(eRoomOrCoridor, index, 6);
            }
        else if ( ( (index._x > 5) && (index._x < 60) ) && ( (index._y > 100) && (index._y < 204) ) ) 
            {
            setValue(eRoomOrCoridor, index, 7);
            }
        else if ( ( (index._x > 150) && (index._x < 204) ) && ( (index._y > 150) && (index._y < 180) ) ) 
            {
            setValue(eRoomOrCoridor, index, 8);
            }

        // floor 1
        else if ( ( (index._x > 305) && (index._x < 450) ) && ( (index._y > 5) && (index._y < 50) ) ) 
            {
            setValue(eRoomOrCoridor, index, 9);
            }
        else if ( ( (index._x > 450) && (index._x < 504) ) && ( (index._y > 5) && (index._y < 50) ) ) 
            {
            setValue(eRoomOrCoridor, index, 10);
            }
        else if ( ( (index._x > 450) && (index._x < 504) ) && ( (index._y > 50) && (index._y < 150) ) ) 
            {
            setValue(eRoomOrCoridor, index, 11);
            }
        else if ( ( (index._x > 305) && (index._x < 350) ) && ( (index._y > 100) && (index._y < 161) ) ) 
            {
            setValue(eRoomOrCoridor, index, 12);
            }
        else if ( ( (index._x > 305) && (index._x < 400) ) && ( (index._y > 160) && (index._y < 204) ) ) 
            {
            setValue(eRoomOrCoridor, index, 12);
            }
        }

// eRoomOrCoridor - SETTING DOORS TO 0
    for(auto index : getBoundaries()) 
        {
        if (getValue(eDoors, index) == 1) {setValue(eRoomOrCoridor, index, 0);}
        }

    int k;
// LOOP TO SET ALL RASTERS WITH 

    std::cout<< "RASTER LOOP DOORS"<<std::endl;
    for(k = 1; k <= evacConfig.doorNumber; k++)
        {
        Engine::Point2D<int> index, cell, goal, target;
        std::ostringstream doorstr;
	doorstr << "d" << k;
	registerDynamicRaster(doorstr.str(), true, k+evacConfig.systemRasters);
	getDynamicRaster(k+evacConfig.systemRasters).setInitValues(-1, std::numeric_limits<int>::max(), 0); 

        std::cout<< "k =" << k <<std::endl;

        for(auto index : getBoundaries())
            {
            if (getValue(eDoorNumber, index) == k) 
                {
                target._x = index._x;
                target._y = index._y;
                }
            }

        if (getValue(eDoorDir, target) == 1)
            {
            goal._x = target._x;
            goal._y = target._y + 1;
            }

        else if (getValue(eDoorDir, target) == 2)
            {
            goal._x = target._x + 1;
            goal._y = target._y;
            }


        else if (getValue(eDoorDir, target) == 3)
            {
            goal._x = target._x;
            goal._y = target._y - 1;
            }

        else if (getValue(eDoorDir, target) == 4)
            {
            goal._x = target._x - 1;
            goal._y = target._y;
            }

        for(auto cell : getBoundaries())
            {
            if ((getValue(eRoomOrCoridor, cell) == getValue(eRoomOrCoridor, goal)) || (getValue(eDoorNumber, cell) == k) )
                {
                //float distance = 100*sqrt(pow((target._x - cell._x),2) + pow((target._y - cell._y),2));
                // MANHATTAN DISTANCE
                //float distance = abs(target._x - cell._x) + abs(target._y - cell._y);
                float distance;
                int absx = abs(target._x - cell._x);
                int absy = abs(target._y - cell._y);
                if (absx > absy){distance = 100 *(absx + absy*(sqrt(2)-1));}
                else if (absy > absx){distance = 100 * (absy + absx*(sqrt(2)-1));}
                else if (absy == absx){distance = 100 * (absx + absy*(sqrt(2)-1));}
                setValue(k+evacConfig.systemRasters, cell, distance);
                }
            else {setValue(k+evacConfig.systemRasters, cell, -1.0);}
            }
            
        }

// POTENTIAL FIELD FOR THE STAIRS TURN SECTION

    std::cout<< "RASTER LOOP TURNS"<<std::endl;
    for(k = 1; k <= evacConfig.turnNumber; k++)
        {
        Engine::Point2D<int> index, cell, goal, target;
        std::ostringstream turnstr;
	turnstr << "t" << k;
	registerDynamicRaster(turnstr.str(), true, k+evacConfig.systemRasters+evacConfig.doorNumber);
	getDynamicRaster(k+evacConfig.systemRasters+evacConfig.doorNumber).setInitValues(-1, std::numeric_limits<int>::max(), 0); 
        std::cout<< "k =" << k <<std::endl;

        for(auto index : getBoundaries())
            {
            if (getValue(eTurnNumber, index) == k) 
                {
                target._x = index._x;
                target._y = index._y;
                }
            }

        for(auto cell : getBoundaries())
            {
            if ((getValue(eStairsSide, cell) == 1) || (getValue(eStairs, cell) == 3) || (getValue(eStairs, cell) == 2) )
                {
                //float distance = 100*sqrt(pow((target._x - cell._x),2) + pow((target._y - cell._y),2));
                // MANHATTAN DISTANCE
                //float distance = abs(target._x - cell._x) + abs(target._y - cell._y);
                float distance;
                int absx = abs(target._x - cell._x);
                int absy = abs(target._y - cell._y);
                if (absx > absy){distance = 100 *(absx + absy*(sqrt(2)-1));}
                else if (absy > absx){distance = 100 * (absy + absx*(sqrt(2)-1));}
                else if (absy == absx){distance = 100 * (absx + absy*(sqrt(2)-1));}
                setValue(k+evacConfig.systemRasters+evacConfig.doorNumber, cell, distance);
                }
            else {setValue(k+evacConfig.systemRasters+evacConfig.doorNumber, cell, -1.0);}
            }

        }

// POTENTIAL FIELD VALUES FOR THE STAIRS JUMP SECTION

    std::cout<< "RASTER LOOP JUMPS"<<std::endl;
    for(k = 1; k <= evacConfig.jumpNumber; k++)
        {
        Engine::Point2D<int> index, cell, goal, target;
        std::ostringstream jumpstr;
	jumpstr << "j" <<k;
	registerDynamicRaster(jumpstr.str(), true, k+evacConfig.systemRasters+evacConfig.doorNumber+evacConfig.turnNumber);
	getDynamicRaster(k+evacConfig.systemRasters+evacConfig.doorNumber+evacConfig.turnNumber).setInitValues(-1, std::numeric_limits<int>::max(), 0); 
        std::cout<< "k =" << k <<std::endl;

        for(auto index : getBoundaries())
            {
            if (getValue(eJumpNumber, index) == k) 
                {
                target._x = index._x;
                target._y = index._y;
                }
            }

        for(auto cell : getBoundaries())
            {
            if ((getValue(eStairsSide, cell) == 2) || (getValue(eStairs, cell) == 4))
                {
                //float distance = 100*sqrt(pow((target._x - cell._x),2) + pow((target._y - cell._y),2));
                // MANHATTAN DISTANCE
                //float distance = abs(target._x - cell._x) + abs(target._y - cell._y);
                float distance;
                int absx = abs(target._x - cell._x);
                int absy = abs(target._y - cell._y);
                if (absx > absy){distance = 100 *(absx + absy*(sqrt(2)-1));}
                else if (absy > absx){distance = 100 * (absy + absx*(sqrt(2)-1));}
                else if (absy == absx){distance = 100 * (absx + absy*(sqrt(2)-1));}
                setValue(k+evacConfig.systemRasters+evacConfig.doorNumber+evacConfig.turnNumber, cell, distance);
                }
            else {setValue(k+evacConfig.systemRasters+evacConfig.doorNumber+evacConfig.turnNumber, cell, -1.0);}
            }

        }

//SET VARIABLE TO DETERMINE DECAY RATE
DecayStatus = evacConfig.chemoDecayRate;

}

//A FUNCTION THAT UPDATES etempcells for next choices

void EvacWorld::UpdateTempValues()
{
    for(auto index : getBoundaries())
        {
        setValue(eTempCells, index, 0);
        }
}

void EvacWorld::ResetExits() // RESET EXIT LIST TEMP VALUE BEFORE STARTING THE NEW ROUND OF SELECTING tempNextPos !!!!
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
    EvacConfig::ExitConfigList::const_iterator it5=evacConfig.exitconfiglist.begin();
	while(it5!=evacConfig.exitconfiglist.end())
	{
	const Engine::Point2D<int> & ext = *it5;
        Engine::Point2D<int> index;
        index._x = ext._x;
        index._y = ext._y;
        setValue(eOccupied, index, 0); 
        it5++; 
        }
}

void EvacWorld::ResetOccupied()
{
    for(auto index : getBoundaries())
        {
        setValue(eOccupied, index, 0);
        }  
}

void EvacWorld::ResetMass()
{
    for(auto index : getBoundaries())
        {
        setValue(eMass, index, 0);
        }  
}

void EvacWorld::ResetPanic()
{
    for(auto index : getBoundaries())
        {
        setValue(ePanic, index, 0);
        }
}


void EvacWorld::ResetKnowledge()
{
    for(auto index : getBoundaries())
        {
        setValue(eKnowledge, index, 0);
        }
}

void EvacWorld::ChemoDecay()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
    if (DecayStatus > 0){DecayStatus--;}
    else if (DecayStatus == 0)
        {
        for(auto index : getBoundaries())
            {
            if (getValue(eChemoTaxiTrails, index) > 0) setValue(eChemoTaxiTrails, index, (getValue(eChemoTaxiTrails, index))-1);
            }        
        DecayStatus = evacConfig.chemoDecayRate;
        }   
}

void EvacWorld::NextPosition() 
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();

    AL::const_iterator iter=al.begin();
    while (iter != al.end())
        {
        EvacAgent * agent = *iter;  
        Engine::Point2D<int> wishPos = agent->_tempNextPosition;  
        if ( (agent->_exited == false) && (agent->_trampled == false) && (getDynamicRaster(eTempCells).getValue(wishPos) == 1) && (wishPos._y > 0) && (wishPos._y < getBoundaries()._size._height) && (wishPos._x > 0) && (wishPos._x < getBoundaries()._size._width) && (getDynamicRaster(eObstacles).getValue(wishPos) == 0))
            { 
                if ( getValue(eFloor, agent->getPosition()) == getValue(eFloor, wishPos)) {agent->_evacDist = agent->_evacDist + sqrt( pow((agent->getPosition()._x - wishPos._x),2) + pow((agent->getPosition()._y - wishPos._y),2) );}
                else if (getValue(eFloor, agent->getPosition()) != getValue(eFloor, wishPos)) {agent->_evacDist = agent->_evacDist + 1;}
		    agent->setPosition(wishPos);
                    setValue(eOccupied, agent->getPosition(), 1);
                    setValue(eMass, agent->getPosition(), agent->returnMass()*100);
                    if (agent->_knowledge == 1) {setValue(eKnowledge, agent->getPosition(), 1);}
                    if (agent->_panicked >= 1) {setValue(ePanic, agent->getPosition(), 1);}
		    agent->_evacTime++;
                    // chemotaxitrails only for agents who know where they are going
		    if (agent->_knowledge == 1) {setValue(eChemoTaxiTrails, agent->_tempNextPosition, getValue(eChemoTaxiTrails, (agent->_tempNextPosition))+1);}
	    }
        iter++;
        }

    //std::cout<<"NextPosition correctly"<<std::endl;
}


void EvacWorld::ResolveCompetition()
{
    //std::cout<<"RESOLVE COMP"<<std::endl;

    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
    for(auto index : getBoundaries())
    {
        if (getDynamicRaster(eTempCells).getValue(index)< 2){continue;}
        else {
               typedef std::list<EvacAgent*>  EvacAgentListTemp;
               EvacAgentListTemp evacTemps;
               EvacAgentListTemp fastestCraziest;

		 AL::const_iterator iter=al.begin();
		 while (iter != al.end())
		     {
	 
		     EvacAgent *agent = *iter;
		     //USE std::cout<<"ID ="<<agent->getId()<<std::endl;
		     if (agent->_tempNextPosition == index){evacTemps.push_front(agent);}
		     iter++;
		     }  
		 EvacAgent *maxAgent = evacTemps.front(); 
		 EvacAgentListTemp::const_iterator it10=evacTemps.begin(); // here we select the craziest fastest agent
		 while(it10!=evacTemps.end())
		     {
		     EvacAgent *ext = *it10;
		     if (ext->_panicked > maxAgent->_panicked) {maxAgent = ext;}
		     else if ((ext->_currentSpeed >= maxAgent->_currentSpeed) && (ext->_panicked == maxAgent->_panicked)){maxAgent = ext;}
		     it10++;
		     }

		 EvacAgentListTemp::const_iterator it6=evacTemps.begin(); // here we gather all the craziest fastest agents (1 or more)
		 while(it6!=evacTemps.end())
		     {
		     EvacAgent *ext = *it6;
		     if ((ext->_currentSpeed == maxAgent->_currentSpeed) && (ext->_panicked == maxAgent->_panicked)){fastestCraziest.push_front(ext);}
		     it6++;
		     }

		 EvacAgent *closest = fastestCraziest.front(); 

		 EvacAgentListTemp::const_iterator it7=fastestCraziest.begin(); // we select the closest agent from fastest craziest
		 while(it7!=fastestCraziest.end())
		     {
		     EvacAgent *ext = *it7;
		     if (sqrt(pow((index._x - closest->getPosition()._x),2) + pow((index._y - closest->getPosition()._y),2)) >= sqrt(pow((index._x - ext->getPosition()._x),2) + pow((index._y - ext->getPosition()._y),2))) {closest = ext;}                
		     it7++;
		     }

		 EvacAgentListTemp closestList;

		 EvacAgentListTemp::const_iterator it8=fastestCraziest.begin(); // we gather all closest agents (1 or more) from fastest craziest
		 while(it8!=fastestCraziest.end())
		     {
		     EvacAgent *ext = *it8;
		     if (sqrt(pow((index._x - ext->getPosition()._x),2) + pow((index._y - ext->getPosition()._y),2)) == sqrt(pow((index._x - closest->getPosition()._x),2) + pow((index._y - closest->getPosition()._y),2))) { closestList.push_front(ext);} 
		     it8++;
		     }
                 EvacAgent *winner;
                 if (closestList.size() > 1) 
                     {
			 int c = Engine::GeneralState::statistics().getUniformDistValue(0,closestList.size()-1);
			 EvacAgentListTemp::const_iterator it11=closestList.begin();
			 while (c>0)
			     {
			     it11++;
			     c--;
			     }
                         winner = *it11;
			 winner->_evacDist = winner->_evacDist + sqrt(pow((winner->getPosition()._x - index._x),2) + pow((winner->getPosition()._y - index._y),2));
			 winner->setPosition(index);  
			 setValue(eOccupied, index, 1);
		         setValue(eMass, index, winner->returnMass()*100); 
		         if (winner->_knowledge == 1) 
                             {
                             setValue(eKnowledge, index, 1);
                             setValue(eChemoTaxiTrails, index, getValue(eChemoTaxiTrails, index)+1);
                             }   
		         if (winner->_panicked >= 1) {setValue(ePanic, index, 1);}
			 winner->_evacTime++;
                      }
                  else if (closestList.size() == 1)
                      {
                         winner = closestList.front();
			 winner->_evacDist = winner->_evacDist + sqrt(pow((winner->getPosition()._x - index._x),2) + pow((winner->getPosition()._y - index._y),2));
			 winner->setPosition(index);  
			 setValue(eOccupied, index, 1);
		         setValue(eMass, index, winner->returnMass()*100); 
		         if (winner->_knowledge == 1) 
                             {
                             setValue(eKnowledge, index, 1);
                             setValue(eChemoTaxiTrails, index, getValue(eChemoTaxiTrails, index)+1);
                             }   
		         if (winner->_panicked >= 1) {setValue(ePanic, index, 1);}      
			 winner->_evacTime++;
                      }

		 // Deal with agents that are not the winner
		 EvacAgentListTemp::const_iterator it9=evacTemps.begin();
		 while(it9!=evacTemps.end())
		     {
		     EvacAgent *ext = *it9;
		     if (ext->getId() != winner->getId() ) 
		         { 
		         ext->_notMoved = ext->_notMoved + 1;
                         ext->_notMovedTotal = ext->_notMovedTotal + 1;
		         ext->_evacTime++;
                         ext->_tempNextPosition = ext->getPosition();
		         setValue(eOccupied, ext->getPosition(), 1);
                         setValue(eMass, ext->getPosition(), ext->returnMass()*100);
                         if (ext->_knowledge == 1) {setValue(eKnowledge, ext->getPosition(), 1);}
                         if (ext->_panicked >= 1) {setValue(ePanic, ext->getPosition(), 1);}       
		         }           
		     it9++;
		     }
              
            }
    }
}

void EvacWorld::stepEnvironment()
{
        std::cout<<"START STEP ENV"<<std::endl;
        ResetMass();
        ResetPanic();
        ResetOccupied();
        ResetKnowledge();
        NextPosition();
        ResolveCompetition();
        UpdateTempValues();
        ResetExits();
        std::cout<<"STEP ENV TEST"<<std::endl;
        std::cout<<"STEP "<< _step <<std::endl;
        std::cout<<"agents left "<< al.size() <<std::endl;
        
 
}

void EvacWorld::run()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
	std::stringstream logName;
	logName << "simulation_" << getId();
	log_INFO(logName.str(), getWallTime() << " executing " << _config->getNumSteps() << " steps...");
        std::cout << "size=" << al.size() << "and bigger than" << ((100 - evacConfig.evacuationThreshold)*evacConfig._numAgents/100) <<std::endl;
        _step=0;
	while( (al.size()> ((100 - evacConfig.evacuationThreshold)*evacConfig._numAgents/100)) && _step < _config->getNumSteps()) // was _agents.size()
	{
		step();
                _step++;
		    for(auto index : getBoundaries())
			{
			setValue(eTime, index, (getValue(eTime, index) + 1));
			}
	}
        if ((al.size() <= ((100 - evacConfig.evacuationThreshold)*evacConfig._numAgents/100)) || _step == _config->getNumSteps())
            {
              int dead = 0;
              for(auto index : getBoundaries()) 
                  {
                  if (getValue(eDead, index) == 1) {dead++;}
                  } 
              std::ofstream evacTime;
              evacTime.open ("Results.txt");
              evacTime << "EvacTime = ";
              evacTime << _step << std::endl;
              evacTime << "Dead = ";
              evacTime << dead << std::endl;
              evacTime.close();
            }
	// storing last step data
	if(_step%_config->getSerializeResolution()==0)
	{
		_scheduler->serializeRasters(_step);
		_scheduler->serializeAgents(_step);
	}
	log_INFO(logName.str(), getWallTime() << " closing files");
	_scheduler->finish();
}

} // namespace Evacuation


