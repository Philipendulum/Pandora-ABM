
#include "EvacWorld.hxx"
#include "EvacConfig.hxx"
#include "EvacAgent.hxx"

#include <typedefs.hxx>

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
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();

    for(int i=0; i<evacConfig._numAgents; i++)
    {
	int floor = Engine::GeneralState::statistics().getUniformDistValue(1, evacConfig.returnFloorNumber());
	char gender = ' ';

        if( Engine::GeneralState::statistics().getUniformDistValue(0,100) > evacConfig.returnMalePerc()) 
	    gender='M';
	else
	    gender='F';
	int vision=0;
	int speed=0;
	int age=0;
        int panicked = 0;
        Engine::Point2D<int> currGoal(0,0);
        Engine::Point2D<int> tempNextPosition(10,10);

        if ((evacConfig.returnChildPerc() + evacConfig.returnElderlyPerc()) > 100) {
             exit(8);
        }
        int b = Engine::GeneralState::statistics().getUniformDistValue(0,100);
        if (b <= evacConfig.returnChildPerc()){ age = 0;}
        else if ((b>evacConfig.returnChildPerc()) && (b<=(evacConfig.returnChildPerc() + evacConfig.returnElderlyPerc()))) { age = 2;}
        else {age = 1;}
        if (age == 1) {
            speed = 3; 
            vision = 50;
        }
        else if (age == 2) {
            speed = 1; 
            vision = 20;
        }
	else if (age == 0) {
        speed = 2; 
        vision = 35;
        }
        bool exited = false;
        bool isOnStairs = false;
        int evacDist = 0;
        int evacTime = 0;
        int notMoved = 0;
        int notMovedTotal = 0;
	std::ostringstream oss;
	oss << "EvacAgent_"<<i;
	EvacAgent * agent = new EvacAgent(oss.str(),speed,floor,gender,age,vision, isOnStairs, exited, panicked, evacDist, evacTime, notMoved, notMovedTotal);
	addAgent(agent);
	// avoid agent in obstacle and cases where more than 1 agent occupies the same cell
	agent->setRandomPosition();
        int floorValue = floor;
	while((getValue(eObstacles, agent->getPosition())==1) ||/* floorValue != getValue(eFloor, agent->getPosition()) ||*/ (getValue(eDoors, agent->getPosition())==1) || (getValue(eExits, agent->getPosition())==1) || (getValue(eOccupied, agent->getPosition())>0) || (getValue(eFloor, agent->getPosition())==0) )
		{
		agent->setRandomPosition();
		}
	//Engine::Point2D<int> p (76,423);
	//agent->setPosition(p); - constructor for specified position placements
	//setValue(eNumAgents, agent->getPosition(), getValue(eNumAgents, agent->getPosition())+1); - NOT NECESSARY AT ALL AS THE RASTER AS WELL
        setValue(eOccupied, agent->getPosition(), 1);
	std::cout<<agent->agentCharac()<<std::endl;
        al.push_back(agent);
        setValue(eOccupied, agent->getPosition(), 1);
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

	registerDynamicRaster("eSigns", true, eSigns);
	getDynamicRaster(eSigns).setInitValues(0, 1, 0);

	registerDynamicRaster("eTempCells", true, eTempCells);
	getDynamicRaster(eTempCells).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eOccupied", true, eOccupied);
	getDynamicRaster(eOccupied).setInitValues(0, 1, 0);        

	registerDynamicRaster("eStairs", true, eStairs);
	getDynamicRaster(eStairs).setInitValues(0, 4, 0);

	registerDynamicRaster("eDoors", true, eDoors);
	getDynamicRaster(eDoors).setInitValues(0, 1, 0);

	registerDynamicRaster("eRoomOrCoridor", true, eRoomOrCoridor);
	getDynamicRaster(eRoomOrCoridor).setInitValues(0, std::numeric_limits<int>::max(), 0);
        
	registerDynamicRaster("eChemoTaxiTrails", true, eChemoTaxiTrails);
	getDynamicRaster(eChemoTaxiTrails).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eFloor", true, eFloor);
	getDynamicRaster(eFloor).setInitValues(0, evacConfig.floorNumber, 0);
	
	//registerDynamicRaster("numAgents", true, eNumAgents);
	//getDynamicRaster(eNumAgents).setInitValues(0, std::numeric_limits<int>::max(), 0);

	registerDynamicRaster("eKnowledge", true, eKnowledge);
	getDynamicRaster(eKnowledge).setInitValues(0, 1, 0);

	registerDynamicRaster("eDoorValue", true, eDoorValue);
	getDynamicRaster(eDoorValue).setInitValues(0, 1, 0);

	registerDynamicRaster("eDoorDir", true, eDoorDir);
	getDynamicRaster(eDoorDir).setInitValues(0, 4, 0);


// SETTING UP OBSTACLES, BORDERS, STAIRS

// SETTING UP WALLS OUTER AND INNER 
        std::cout << "Rasters2" << std::endl;
        for(auto index : getBoundaries()) 
            {
        // outer boundaries
            if ( ((index._x > 4) && (index._x < 205) && (index._y == 5)) || ((index._x > 4) && (index._x < 205) && (index._y == 204)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._x > 304) && (index._x < 505) && (index._y == 5)) || ((index._x > 304) && (index._x < 505) && (index._y == 204)) ) {setValue(eObstacles, index, 1);}
            //else if ( ((index._x > 604) && (index._x < 805) && (index._y == 5)) || ((index._x > 604) && (index._x < 805) && (index._y == 204)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._y > 4) && (index._y < 205) && (index._x == 5)) || ((index._y > 4) && (index._y < 205) && (index._x == 204)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._y > 4) && (index._y < 205) && (index._x == 305)) || ((index._y > 4) && (index._y < 205) && (index._x == 504)) ) {setValue(eObstacles, index, 1);}
            //else if ( ((index._y > 4) && (index._y < 205) && (index._x == 605)) || ((index._y > 4) && (index._y < 205) && (index._x == 804)) ) {setValue(eObstacles, index, 1);}

        // new third floor 

            else if ( ((index._x > 4) && (index._x < 205) && (index._y == 305)) || ((index._x > 4) && (index._x < 205) && (index._y == 504)) ) {setValue(eObstacles, index, 1);}
            else if ( ((index._y > 304) && (index._y < 505) && (index._x == 5)) || ((index._y > 304) && (index._y < 505) && (index._x == 204)) ) {setValue(eObstacles, index, 1);}
            

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
            // floor 0 
            //else if ( (index._x == 660) && (index._y > 5) && (index._y < 204) ) {setValue(eObstacles, index, 1);}
            // new line
            else if ( (index._x == 60) && (index._y > 305) && (index._y < 504) ) {setValue(eObstacles, index, 1);}
            }
  
            // SETTING UP STAIRS RIGHT NOW 
        for(auto index : getBoundaries()) 
            {
            // floor 2 
            if ( (index._x == 191) && (index._y > 204) && (index._y < 218) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 185) && (index._y > 204) && (index._y < 218) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 188) && (index._y > 206) && (index._y < 215) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 217) && (index._x > 184) && (index._x < 192) ) {setValue(eObstacles, index, 1);}
            

            // floor 1
            else if ( (index._x == 491) && (index._y > 204) && (index._y < 218) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 485) && (index._y > 204) && (index._y < 218) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 488) && (index._y > 206) && (index._y < 215) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 217) && (index._x > 484) && (index._x < 492) ) {setValue(eObstacles, index, 1);}            
        
            // floor 0 - since no stairs, I will just add the extra land piece here
            //else if ( (index._x == 788) && (index._y > 204) && (index._y < 208) ) {setValue(eObstacles, index, 1);}
            //else if ( (index._x == 791) && (index._y > 204) && (index._y < 208) ) {setValue(eObstacles, index, 1);}
            //else if ( (index._y == 207) && (index._x > 787) && (index._y < 792) ) {setValue(eObstacles, index, 1);}
            // new lines
            else if ( (index._x == 188) && (index._y > 504) && (index._y < 508) ) {setValue(eObstacles, index, 1);}
            else if ( (index._x == 191) && (index._y > 504) && (index._y < 508) ) {setValue(eObstacles, index, 1);}
            else if ( (index._y == 507) && (index._x > 187) && (index._y < 192) ) {setValue(eObstacles, index, 1);}
            }
        
         // setting up stairs on the eStairs raster
        for(auto index : getBoundaries()) 
            {
            if ( (index._y > 204) && (index._y < 218) && (index._x > 185) && (index._x < 191) && (getValue(eObstacles, index) != 1) ) {setValue(eStairs, index, 1);}
            else if ( (index._y > 204) && (index._y < 218) && (index._x > 485) && (index._x < 491) && (getValue(eObstacles, index) != 1) ) {setValue(eStairs, index, 1);}        
            }        
            


        // PLACING OUTER WALLS AFTER EXITS
   /* for(auto index : getBoundaries()) 
        {
        if((getValue(eExits, index)==0) && (index._x==0 || index._y==0 || index._x==getBoundaries()._size._width-1 || index._y==getBoundaries()._size._height-1)){setValue(eObstacles, index, 1);}
        }

//        //PLACING INNER WALLS ' TRY To COMMENT IT OUT TO XHECK ' RETURN IT AFTERWARDS
    for(auto index : getBoundaries())
        {
        if(  ((index._y == floor(getBoundaries()._size._height / 2)) && (((index._x >= 0) && (index._x <= floor(getBoundaries()._size._width / 3))) || (((index._x >= floor(getBoundaries()._size._width / 3) + 3)) && (index._x <= floor(getBoundaries()._size._width / 2)))))) {setValue(eObstacles, index, 1);}
        else if ((index._x == floor(getBoundaries()._size._width / 2)) && ( (index._y < floor(getBoundaries()._size._height / 2)) && (index._y >= 0) ) ){setValue(eObstacles, index, 1);}
        }*/


        // probably wrong way to read doors, so i will try another one
	/*Engine::Point2D<int> index(0,0);
        EvacConfig::DoorList::const_iterator itdoor=evacConfig.doorList.begin();
	while(itdoor!=evacConfig.doorList.end())
	{
            const Engine::Point2D<int> & ext = *itdoor;
            Engine::Point2D<int> index;
            index._x = ext._x;
            index._y = ext._y;
            setValue(eExits, index, 1); 
            itdoor++; 
        }*/


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
    while(itturn!=evacConfig.turnList.end())
	{
	const Engine::Point2D<int> & sign = *itturn;
        Engine::Point2D<int> index;
        index._x = sign._x;
        index._y = sign._y;
        setValue(eStairs, index, 3); 
        itturn++; 
        }    


    // Placing Jump Positions - CHECK THIS, BREAK POINT
        std::cout << "Placing Jumps on Raster" << std::endl;
    EvacConfig::JumpCellsList::const_iterator itjump=evacConfig.jumpCellsfrom.begin();
    while(itjump!=evacConfig.jumpCellsfrom.end())
	{
	const Engine::Point2D<int> & sign = *itjump;
        Engine::Point2D<int> index;
        index._x = sign._x;
        index._y = sign._y;
        setValue(eStairs, index, 4); 
        itjump++; 
        }   

     // Starting to number the eRoomOrCoridor & eFloor after everything is done
// FLOORS
    for(auto index : getBoundaries()) 
        {
        if ( ( (index._x > 5) && (index._x < 204) ) && ( (index._y > 5) && (index._y < 204) ) ) 
            {
            setValue(eFloor, index, 3);
            setValue(eRoomOrCoridor, index, 1);
            } 
        else if ( ( (index._x > 305) && (index._x < 504) ) && ( (index._y > 5) && (index._y < 204) ) ) 
            {
            setValue(eFloor, index, 2);
            setValue(eRoomOrCoridor, index, 2);
            } 
        else if ( ( (index._x > 5) && (index._x < 204) ) && ( (index._y > 305) && (index._y < 504) ) ) 
            {
            setValue(eFloor, index, 1);
            setValue(eRoomOrCoridor, index, 3);
            } 
        else if ( ( (index._x > 188) && (index._x < 191) ) && ( (index._y > 504) && (index._y < 507) ) ) 
            {
            setValue(eFloor, index, 1);
            }
        }     
// eRoomOrCoridor - TRICKY
    for(auto index : getBoundaries()) 
        {
        // floor 2
        if ( ( (index._x > 5) && (index._x < 50) ) && ( (index._y > 5) && (index._y < 50) ) ) 
            {
            setValue(eRoomOrCoridor, index, 4);
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
            setValue(eRoomOrCoridor, index, 13);
            }
        // floor 0
        else if ( ( (index._x > 5) && (index._x < 60) ) && ( (index._y > 305) && (index._y < 504) ) ) 
            {
            setValue(eRoomOrCoridor, index, 14);
            }
        //else if ( ( (index._x > 5) && (index._x < 60) ) && ( (index._y > 305) && (index._y < 504) ) ) 
          //  {
            //setValue(eRoomOrCoridor, index, 15);
            //}
        else if ( ( (index._x > 188) && (index._x < 191) ) && ( (index._y > 504) && (index._y < 507) ) ) 
            {
            setValue(eRoomOrCoridor, index, 15);
            }
        }

}

        
//PLAING DOORS - BUT I STILL NEED TO MODIFY THE CONFIG FILE !!!!!!!!!!! KEEP IN MIND !!!!!!!!!!

/*    EvacConfig::DoorList::const_iterator it3=evacConfig.doorList.begin();
    while(it3!=evacConfig.doorList.end())
	{
	const Engine::Point2D<int> & door = *it3;
        Engine::Point2D<int> index;
        index._x = door._x;
        index._y = door._y;
        setValue(eDoors, index, 1); 
        it3++; 
        }*/

//ROOM OR CORRIDOR VALUES 

/*    for(auto index : getBoundaries())
        {
        if( ( (index._y > 0) && (index._y < floor(getBoundaries()._size._height / 2)) ) && ((index._x > 0) && (index._x < floor(getBoundaries()._size._width / 2)) )){setValue(eRoomOrCoridor, index, 1);}
        else if (getValue(eDoors, index) == 1){setValue(eRoomOrCoridor, index, 2);}
        }
    std::cout << "End Rasters" << std::endl;
}*/




//herE WE CREATE A FUNCTION THAT UPDATES etempcells for next choices

void EvacWorld::UpdateTempValues()
{
    for(auto index : getBoundaries())
        {
        setValue(eTempCells, index, 0);
        }
    std::cout <<"TEMPCELLS RESET AGAIN"<<std::endl;
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
        setValue(eTempCells, index, 0);
        setValue(eOccupied, index, 0); 
        it5++; 
        }
    std::cout<<"ResetExits executed correctly"<<std::endl;
}

void EvacWorld::ResetOccupied()
{
    for(auto index : getBoundaries())
        {
        setValue(eOccupied, index, 0);
        }
    std::cout <<"EOCCUPIED RESET AGAIN"<<std::endl;    
}

void EvacWorld::ResetKnowledge()
{
    for(auto index : getBoundaries())
        {
        setValue(eKnowledge, index, 0);
        }
    std::cout <<"EKNOWLEDGE RESET AGAIN"<<std::endl;    
}

// alSO NEED TO TAKE INTO ACCOUNT SOMEHOW PANICKING THAT INVOLVES TAKING OTHER PEOPLE POSITIOS

void EvacWorld::NextPosition() 
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();

    AL::const_iterator iter=al.begin();
    while (iter != al.end())
        {
        EvacAgent * agent = *iter;  
        Engine::Point2D<int> curPos = agent->_tempNextPosition;    
        if ( (agent->_exited == false) && (getDynamicRaster(eTempCells).getValue(curPos) == 1) && (curPos._y > 0) && (curPos._y < getBoundaries()._size._height) && (curPos._x > 0) && (curPos._x < getBoundaries()._size._width) && (getDynamicRaster(eObstacles).getValue(curPos) == 0))
            {
	    // NEED TO ADD CONDITION SO THAT THIS ONLY WORKS IF THE FLOOR IS KIND OF THE SAME - next line ???? 
                if ( getValue(eFloor, agent->getPosition()) == getValue(eFloor, curPos)) {agent->_evacDist = agent->_evacDist + sqrt( pow((agent->_tempNextPosition._x - curPos._x),2) + pow((agent->_tempNextPosition._y - curPos._y),2) );}
                else if (getValue(eFloor, agent->getPosition()) != getValue(eFloor, curPos)) {agent->_evacDist = agent->_evacDist + 1;}
            // Other lines should be executed always
		    agent->setPosition(curPos);
                    setValue(eOccupied, agent->_tempNextPosition, getValue(eOccupied, (agent->_tempNextPosition))+1);
                    if (agent->_knowledge == 1) {setValue(eKnowledge, agent->getPosition(), 1);}
		    agent->_evacTime++;
		    setValue(eChemoTaxiTrails, agent->_tempNextPosition, getValue(eChemoTaxiTrails, (agent->_tempNextPosition))+1);
	    }
	//else { setValue(eOccupied, agent->getPosition(), 1);} // TEMPORARY LINE WHILE RESOLVE COMPETITION IS NOT WORKING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	std::cout<<"NextPosition for" << agent->getId() << "correctly"<<std::endl;
        iter++;
        }

    std::cout<<"NextPosition correctly"<<std::endl;
}


void EvacWorld::ResolveCompetition()
{
    std::cout<<"RESOLVE COMP"<<std::endl;

    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
    for(auto index : getBoundaries())
    {
        if (getDynamicRaster(eTempCells).getValue(index)< 2){continue;}
        else {
               std::cout<<"resolve index at"<<index<<std::endl;
               typedef std::list<EvacAgent*>  EvacAgentListTemp;
               EvacAgentListTemp evacTemps;
               EvacAgentListTemp fastestCraziest;

		 AL::const_iterator iter=al.begin();
		 while (iter != al.end())
		     {
	 
		     EvacAgent *agent = *iter;
		     std::cout<<"ID ="<<agent->getId()<<std::endl;
		     if (agent->_tempNextPosition == index){evacTemps.push_front(agent);}
		     iter++;
		     }  
		 std::cout<<"evacTemps succesfully created"<<index<<std::endl;

		 EvacAgent *maxAgent = evacTemps.front(); 
		 std::cout<<"MaxAgent front at"<<index<<std::endl;

		 EvacAgentListTemp::const_iterator it10=evacTemps.begin();
		 while(it10!=evacTemps.end())
		     {
		     EvacAgent *ext = *it10;
		     if ( ext->returnSpeed() > maxAgent->returnSpeed() ) {maxAgent = ext;}
		     else if ((ext->returnSpeed() == maxAgent->returnSpeed()) && (ext->_panicked >= maxAgent->_panicked)){maxAgent = ext;}
		     it10++;
		     }
		 std::cout<<"real MaxAgent at"<<index<<std::endl;

		 EvacAgentListTemp::const_iterator it6=evacTemps.begin();
		 while(it6!=evacTemps.end())
		     {
		     EvacAgent *ext = *it6;
		     if ((ext->returnSpeed() == maxAgent->returnSpeed()) && (ext->_panicked == maxAgent->_panicked)){fastestCraziest.push_front(ext);}
		     it6++;
		     }
		 std::cout<<"added all competition of MaxAgent at"<<index<<std::endl;

		 EvacAgent *closest = fastestCraziest.front(); 

		 EvacAgentListTemp::const_iterator it7=fastestCraziest.begin();
		 while(it7!=fastestCraziest.end())
		     {
		     EvacAgent *ext = *it7;
		     if (sqrt(pow((index._x - closest->getPosition()._x),2) + pow((index._y - closest->getPosition()._y),2)) >= sqrt(pow((index._x - ext->getPosition()._x),2) + pow((index._y - ext->getPosition()._y),2))) {closest = ext;}                
		     it7++;
		     }
		 std::cout<<"choose closest from competition at"<<index<<std::endl;

		 EvacAgentListTemp closestList;

		 EvacAgentListTemp::const_iterator it8=fastestCraziest.begin();
		 while(it8!=fastestCraziest.end())
		     {
		     EvacAgent *ext = *it8;
		     if (sqrt(pow((index._x - ext->getPosition()._x),2) + pow((index._y - ext->getPosition()._y),2)) == sqrt(pow((index._x - closest->getPosition()._x),2) + pow((index._y - closest->getPosition()._y),2))) { closestList.push_front(ext);} 
		     it8++;
		     }
		 std::cout<<"add all closest from competition at"<<index<<std::endl;

		 int c = Engine::GeneralState::statistics().getUniformDistValue(0,closestList.size()-1);
		 EvacAgentListTemp::const_iterator it11=fastestCraziest.begin();
		 while (c>0)
		     {
		     it11++;
		     c--;
		     }
		 EvacAgent *winner = *it11;
		 winner->_evacDist = winner->_evacDist + sqrt(pow((winner->getPosition()._x - index._x),2) + pow((winner->getPosition()._y - index._y),2));
		 winner->setPosition(index);  
		 setValue(eOccupied, index, 1); 
                 if (winner->_knowledge == 1) {setValue(eKnowledge, index, 1);}             
		 setValue(eChemoTaxiTrails, index, getValue(eChemoTaxiTrails, index)+1);
		 winner->_evacTime++;
		 // Deal with agents that are not the winner
		 EvacAgentListTemp::const_iterator it9=fastestCraziest.begin();
		 while(it9!=fastestCraziest.end())
		     {
		     EvacAgent *ext = *it9;
		     if (ext->getId() != winner->getId() ) 
		         { 
		         ext->_notMoved = ext->_notMoved + 1;
                         ext->_notMovedTotal = ext->_notMovedTotal + 1;
		         ext->_evacTime++;
		         setValue(eOccupied, ext->getPosition(), 1); // Setting eOccupied to 1 for the agents that have not moved as well
                         if (ext->_knowledge == 1) {setValue(eKnowledge, ext->getPosition(), 1);}      
		         }                
		     it9++;
		     }
              
            }
    }
}

void EvacWorld::stepEnvironment()
{
        //NEW FUNCTION TO RESET the eOccupied raster 
        ResetOccupied();
        ResetKnowledge();
        NextPosition();
        ResolveCompetition();
        UpdateTempValues();
        ResetExits();
        std::cout<<"STEP ENV TEST"<<std::endl;

}

} // namespace Evacuation


