
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
	int floor = Engine::GeneralState::statistics().getUniformDistValue(0, evacConfig.returnFloorNumber()-1);
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
        //
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
	std::ostringstream oss;
	oss << "EvacAgent_"<<i;
	EvacAgent * agent = new EvacAgent(oss.str(),speed,floor,gender,age,vision, isOnStairs, exited, panicked, currGoal, evacDist, evacTime, notMoved);
	addAgent(agent);
	// avoid agent in obstacle and cases where more than 1 agent occupies the same cell
	agent->setRandomPosition();
        int floorValue = floor;
	while((getValue(eObstacles, agent->getPosition())==1) || (getValue(eNumAgents, agent->getPosition()) > 0) ||/* floorValue != getValue(eFloor, agent->getPosition()) ||*/ (getValue(eDoors, agent->getPosition())==1) )
	{
		agent->setRandomPosition();
	}
	//Engine::Point2D<int> p (76,423);
	//agent->setPosition(p);
	//computeShortestExit(*agent); // DO I HAVE TO DO THIS THOUGH????
	setValue(eNumAgents, agent->getPosition(), getValue(eNumAgents, agent->getPosition())+1);
	std::cout<<agent->agentCharac()<<std::endl;
    }

}


// this is where my get tempnext cell will be 

void EvacWorld::createRasters()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
        std::cout << "Rasters" << std::endl;
	registerDynamicRaster("obstacles", true, eObstacles);
	//Engine::GeneralState::rasterLoader().fillGDALRaster(getDynamicRaster(eObstacles), scenarioConfig._obstacleFile, getBoundaries());	
	getDynamicRaster(eObstacles).setInitValues(0, 1, 0);
	//getDynamicRaster(eObstacles).setMaxValue(1);

	registerDynamicRaster("exits", true, eExits);
	getDynamicRaster(eExits).setInitValues(0, 1, 0);
	//getDynamicRaster(eExits).setMaxValue(1);

	registerDynamicRaster("eSigns", true, eSigns);
	getDynamicRaster(eSigns).setInitValues(0, 1, 0);
	//getDynamicRaster(eSigns).setMaxValue(1);

	registerDynamicRaster("eTempCells", true, eTempCells);
	getDynamicRaster(eTempCells).setInitValues(0, 80, 0);
	//getDynamicRaster(eTempCells).setMaxValue(8);


        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! I NEED A RASTER WITH VALUES OF CURRENT CELLS OCCUPIED BY AGENTS, SIMILAR OT TEMPCELLS

	registerDynamicRaster("eStairs", true, eStairs);
	getDynamicRaster(eStairs).setInitValues(0, 1, 0);
	//getDynamicRaster(eStairs).setMaxValue(1);

	registerDynamicRaster("eDoors", true, eDoors);
	getDynamicRaster(eDoors).setInitValues(0, 1, 0);
	//getDynamicRaster(eDoors).setMaxValue(1);

	registerDynamicRaster("eRoomOrCoridor", true, eRoomOrCoridor);
	getDynamicRaster(eRoomOrCoridor).setInitValues(0, 2, 0);
	//getDynamicRaster(eRoomOrCoridor).setMaxValue(1);
        
	registerDynamicRaster("eChemoTaxiTrails", true, eChemoTaxiTrails);
	getDynamicRaster(eChemoTaxiTrails).setInitValues(0, std::numeric_limits<int>::max(), 0);

	//Floors will not be used at the moment, but later on will be implemented
	/*registerDynamicRaster("eFloors", false, eChemoTaxiTrails);
	getDynamicRaster(eNumAgents).setInitValues(0, floorNumber-1, 0);*/
	
	registerDynamicRaster("numAgents", true, eNumAgents);
	getDynamicRaster(eNumAgents).setInitValues(0, std::numeric_limits<int>::max(), 0);

	// this seems unnecessary

	/*registerDynamicRaster("deaths", false, eDeaths);
	getDynamicRaster(eDeaths).setInitValues(0, std::numeric_limits<int>::max(), 0);*/


	// compute exit cells - MY UPDATED VERSION
        std::cout << "First rasters" << std::endl;

	Engine::Point2D<int> index(0,0);
	////EvacConfig::ExitConfigList::const_iterator it=scenarioConfig.exitconfiglist.begin(); ' CHANGING THIS
        EvacConfig::ExitConfigList::const_iterator it=evacConfig.exitconfiglist.begin();
	while(it!=evacConfig.exitconfiglist.end())
	{
		const Engine::Point2D<int> & ext = *it;
		// new exit
        Engine::Point2D<int> index;
        index._x = ext._x;
        index._y = ext._y;
        //setMaxValue(eExits, index, 1);
        setValue(eExits, index, 1); 
        it++; 
        }
	fillExitList(); // DO I  REALLY NEED THIS _

        // PLACING OUTER WALLS AFTER EXITS
    for(auto index : getBoundaries()) 
        {
        //if((getStaticRaster(eExits).getValue(index)==0) && (index._x==0 || index._y==0 || index._x==getBoundaries()._size._width-1 || index._y==getBoundaries()._size._height-1)){setMaxValue(eObstacles, index, 1);}
        if((getValue(eExits, index)==0) && (index._x==0 || index._y==0 || index._x==getBoundaries()._size._width-1 || index._y==getBoundaries()._size._height-1)){setValue(eObstacles, index, 1);} // WAS SETMAXVALUE
        }
	//updateRasterToMaxValues(eObstacles);

        std::cout << "fourth rasters" << std::endl;

//        //PLACING INNER WALLS ' TRY To COMMENT IT OUT TO XHECK ' RETURN IT AFTERWARDS
    for(auto index : getBoundaries())
        {
        if(  ((index._y == floor(getBoundaries()._size._height / 2)) && (((index._x >= 0) && (index._x <= floor(getBoundaries()._size._width / 3))) || (((index._x >= floor(getBoundaries()._size._width / 3) + 3)) && (index._x <= floor(getBoundaries()._size._width / 2)))))) {setValue(eObstacles, index, 1);}
        else if ((index._x == floor(getBoundaries()._size._width / 2)) && ( (index._y < floor(getBoundaries()._size._height / 2)) && (index._y >= 0) ) ){setValue(eObstacles, index, 1);} // WAS SETMAXVALUE
        }
	//updateRasterToMaxValues(eObstacles);


        //PLACING SIGNS - BUT I STILL NEED TO MODIFY THE CONFIG FILE !!!!!!!!!!! KEEP IN MIND !!!!!!!!!! SIGNS NEED TO BE CLOSE TO OBSTACLES(WALLS)

        std::cout << "fifth rasters" << std::endl;



	EvacConfig::SignList::const_iterator it2=evacConfig.signList.begin();
	while(it2!=evacConfig.signList.end())
	{
		const Engine::Point2D<int> & sign = *it2;
		// new sign
        Engine::Point2D<int> index;
        index._x = sign._x;
        index._y = sign._y;
        //setMaxValue(eSigns, index, 1);
        setValue(eSigns, index, 1); 
        it2++; 
        }
        
        //updateRasterToMaxValues(eSigns);


        //PLAING DOORS - BUT I STILL NEED TO MODIFY THE CONFIG FILE !!!!!!!!!!! KEEP IN MIND !!!!!!!!!!

	EvacConfig::DoorList::const_iterator it3=evacConfig.doorList.begin();
	while(it3!=evacConfig.doorList.end())
	{
		const Engine::Point2D<int> & door = *it3;
		// new door
        Engine::Point2D<int> index;
        index._x = door._x;
        index._y = door._y;
        //setMaxValue(eDoors, index, 1);
        setValue(eDoors, index, 1); 
        it3++; 
        }
        //updateRasterToMaxValues(eDoors);


        //ROOM OR CORRIDOR VALUES 

    for(auto index : getBoundaries())
        {
        if( ( (index._y > 0) && (index._y < floor(getBoundaries()._size._height / 2)) ) && ((index._x > 0) && (index._x < floor(getBoundaries()._size._width / 2)) )){setValue(eRoomOrCoridor, index, 1);}
        //else if (getDynamicRasterValue(eDoors).getValue(index) == 1){setMaxValue(eRoomOrCoridor, index, 2}
        else if (getValue(eDoors, index) == 1){setValue(eRoomOrCoridor, index, 2);}// WAS SETMAXVALUE
        }
    //updateRasterToMaxValues(eRoomOrCoridor);
   
       std::cout << "End Rasters" << std::endl;
}

void EvacWorld::fillExitList()
{
    for(auto index : getBoundaries())
	{
        if(getDynamicRaster(eExits).getValue(index)==0)
		{
			continue;
		}
		_exits.push_back(index);
	}
        //std::cout << "EXITLISTFILLED" << std::endl;
}



// seLECT NEXT TEMP NEXT POSITION LOOKING AROUND FOR DOORS OR SIGNS.
// stILL NEED TO WRITE HOW CURRGOAL CHANGES FROM DOOR TO EXIT - ALSO WRITE HOW HE CHOOSES THE FIRST SIGN OR EXIT VISIBLE



//herE WE CREATE A FUNCTION THAT UPDATES EXIT LISTS AND EXIT/REMOVES AGENTS - MOVED TO AGENT.CXX UPDATE STATE

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
		// new exit
        Engine::Point2D<int> index;
        index._x = ext._x;
        index._y = ext._y;
        setValue(eTempCells, index, 0); 
        it5++; 
        }
    std::cout<<"ResetExits executed correctly"<<std::endl;
}




// seT TEMP NEXT POSITION MOVED TO AGENT.CXX SINCE I BELIEVE ITS BETTER OFF THERE





// after tempNextPosition has been set ??? CHECK IF THIS IS WORKING CORRECTLY



// i  BELIEVE THIS SHOULD BE IN AGENT.CXX 
/*void EvacWorld::NextPosition()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
    for (int i=0; i<evacConfig._numAgents; i++)
    {
        id << "EvacAgent_" << i;
        EvacAgent & agent = (EvacAgent &) (*world->getAgent(id));
//    for (int i=0; i<evacConfig._numAgents; i++){
  
        if ((agent.exited == false) && (getDynamicRaster(eTempCells).getValue(agent.position) == 1))
        {
            agent.evacDist = agent.evacDist + sqrt(pow((agent.tempNextPosition._x - agent.position._x),2) + pow((agent.tempNextPosition._y - agent.position._y),2));
            agent->setPosition(agent.tempNextPosition);
            agent.evacTime++;
            setValue(eChemoTaxiTrails, agent.tempNextPosition, getValue(eChemoTaxiTrails, agent.tempNextPosition)+1));
        }
        else {continue;}
     }
}*/

// in RESOLVE COMPETITION WE NEED TO CHANGE THE SELECTION SINCE 6 GUYS MIGHT BE WANTING TO JUMP AND RANDOMLY GUYS FROM BACK WILL BE WINNING - ADDING PRIORITY FOR CLOSER AGENTS. 
// alSO NEED TO TAKE INTO ACCOUNT SOMEHOW PANICKING THAT INVOLVES TAKING OTHER PEOPLE POSITIOS

void EvacWorld::NextPosition() // I BELIEVE THIS SHOULD BE IN WORLD CXX
{
    const EvacConfig & evacConfig = (const EvacConfig &)/*getWorld()->*/getConfig();

    for (int i=0; i<evacConfig._numAgents; i++)
                   {
                    //TO ITERATE OVER ALL AGENTS
            std::ostringstream id; 
            id << "EvacAgent_" << i;
            // TWO TEST LINES
//            Engine::World* world = getWorld();
//            EvacWorld & evacWorld = (EvacWorld &) *world;

        EvacAgent * agent = (EvacAgent *) getAgent(id.str()) ;
        Engine::Point2D<int> curPos = agent->_tempNextPosition;    
        if ( (agent->_exited == false) && (/*getWorld()->*/getDynamicRaster(eTempCells).getValue(curPos) == 1) && (curPos._y > 0) && (curPos._y < getBoundaries()._size._height) && (curPos._x > 0) && (curPos._x < getBoundaries()._size._width) && (getDynamicRaster(eObstacles).getValue(curPos) == 0))
        {
            
            /*this->*/agent->_evacDist = /*this->*/agent->_evacDist + sqrt(pow((agent->_tempNextPosition._x - curPos._x),2) + pow((agent->_tempNextPosition._y - curPos._y),2));
            agent->setPosition(curPos);
            /*this->*/agent->_evacTime++;
            /*getWorld()->*/setValue(eChemoTaxiTrails, agent->_tempNextPosition, /*getWorld()->*/getValue(eChemoTaxiTrails, (agent->_tempNextPosition))+1);
        }
        //else {continue;}
        std::cout<<"NextPosition for" << i << "correctly"<<std::endl;
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
               typedef std::list<EvacAgent*>  EvacAgentListTemp; // dots to arrows , GIT !!!!!!!!!!
               EvacAgentListTemp evacTemps = {};
               EvacAgentListTemp fastestCraziest = {};

// MAKE LIST ITERATOR !!!!!
               for (int i=0; i<evacConfig._numAgents; i++)
                   {
                    //TO ITERATE OVER ALL AGENTS
                   std::ostringstream id; 
                   id << "EvacAgent_" << i;
            // TWO TEST LINES
            //Engine::World* world = getWorld();
            //EvacWorld & evacWorld = (EvacWorld &) *world;
               EvacAgent * agent = (EvacAgent *) getAgent(id.str()) ;
                   if (agent->_tempNextPosition == index){evacTemps.push_front(agent);}
                   }
               EvacAgent *maxAgent = evacTemps.front(); 
               /*for (int i=0; i<evacTemps.size(); i++){ // PREVIOUS WAY
                    
                    if (evacTemps[i]->speed > maxAgent.speed){maxAgent = evacTemps[i];}
                    else if ((evacTemps[i]->speed == maxAgent->speed) && (evacTemps[i]->panicLevel >= maxAgent->panicLevel)){maxAgent = evacTemps[i];}
                    else continue;
                  }*/



		//_currGoal = evacConfig.exitconfiglist.front();
		/*EvacWorld::*/EvacAgentListTemp::const_iterator it10=evacTemps.begin();
		while(it10!=evacTemps.end())
		{
			//const Engine::Point2D<int> & ext = *it5;
                        /*EvacWorld::*/EvacAgent *ext = *it10; // CHANGED FROM const EvacWorld::EvacAgent & ext = *it10;

		if ( ext->returnSpeed() > maxAgent->returnSpeed() ) {maxAgent = ext;}
                else if ((ext->returnSpeed() == maxAgent->returnSpeed()) && (ext->_panicked >= maxAgent->_panicked)){maxAgent = ext;}
        	it10++;
        	}

               // HOW ABOUT WE DON'T INSERT THE MAXAGENT AND JUST INSER ALL OF THE AGENTS THAT HAVE SAME DATA???
               //fastestCraziest.insert(1, maxAgent); // THINK HOW TO REMOVE THE DUPLICATE OF THIS maxAgent -> Removed by adding NOT EQUAL to maxAgent.id ????
               /*for (int i=0; i<evacTemps.size(); i++){
                    if ((evacTemps[i]->speed == maxAgent->speed) && (evacTemps[i]->panicLevel == maxAgent->panicLevel)){fastestCraziest.insert(1, evacTemps[i]);}
                    else continue;
                  }*/



		/*EvacWorld::*/EvacAgentListTemp::const_iterator it6=evacTemps.begin();
		while(it6!=evacTemps.end())
		{
                        EvacAgent *ext = *it6; // BEFORE WAS const EvacWorld::EvacAgent & ext = *it6;

                if ((ext->returnSpeed() == maxAgent->returnSpeed()) && (ext->_panicked == maxAgent->_panicked)){fastestCraziest.push_front(ext);}
        	it6++;
        	}

               EvacAgent *closest = fastestCraziest.front(); 

               /*for (int i = 0; i< fastestCraziest.size(); i++)
                   {
                   if (sqrt(pow((index._x - closest->position._x),2) + pow((index._y - closest->position._y),2)) >= sqrt(pow((index._x - fastestCraziest[i]->position._x),2) + pow((index._y - fastestCraziest[i]->position._y),2))) {closest = fastestCraziest[i];}
                   }*/


		/*EvacWorld::*/EvacAgentListTemp::const_iterator it7=fastestCraziest.begin();
		while(it7!=fastestCraziest.end())
		{
                        EvacAgent *ext = *it7; // WAS LIKE const EvacWorld::EvacAgent & ext = *it7;

                if (sqrt(pow((index._x - closest->getPosition()._x),2) + pow((index._y - closest->getPosition()._y),2)) >= sqrt(pow((index._x - ext->getPosition()._x),2) + pow((index._y - ext->getPosition()._y),2))) {closest = ext;}                
        	it7++;
        	}

               EvacAgentListTemp *closestList;


               /*for (int i = 0; i< fastestCraziest.size(); i++)
                    {
                    if (sqrt(pow((index._x - closest->position._x),2) + pow((index._y - closest->position._y),2)) == sqrt(pow((index._x - fastestCraziest[i]->position._x),2) + pow((index._y - fastestCraziest[i]->position._y),2)))
                        {
                        closestList.insert(1, fastestCraziest[i]);
                        }
                    }*/


		/*EvacWorld::*/EvacAgentListTemp::const_iterator it8=fastestCraziest.begin();
		while(it8!=fastestCraziest.end())
		{
                        EvacAgent *ext = *it8; // WAS LIKE THIS const EvacWorld::EvacAgent & ext = *it8;

                if (sqrt(pow((index._x - closest->getPosition()._x),2) + pow((index._y - closest->getPosition()._y),2)) == sqrt(pow((index._x - ext->getPosition()._x),2) + pow((index._y - ext->getPosition()._y),2))) { closestList->push_front(ext);} //closestList.push_front(ext);}                
        	it8++;
        	}



                int c = Engine::GeneralState::statistics().getUniformDistValue(0,closestList->size()-1);
		/*EvacWorld::*/EvacAgentListTemp::const_iterator it11=fastestCraziest.begin();
                while (c>0){
                           it11++;
                           c--;
                           }
                EvacAgent *winner = *it11; ///*const EvacWorld::*/EvacAgent & winner = *it11;
                winner->_evacDist = winner->_evacDist + sqrt(pow((winner->getPosition()._x - index._x),2) + pow((winner->getPosition()._y - index._y),2));
                winner->setPosition(index);                
                setValue(eChemoTaxiTrails, index, getValue(eChemoTaxiTrails, index)+1);
                winner->_evacTime++;
                //fastestCraziest.erase(winner); - DO NOT NEED TO ERASE BECAUSE IT IS SKIPPED LATER ON ?



		/*EvacWorld::*/EvacAgentListTemp::const_iterator it9=fastestCraziest.begin();
		while(it9!=fastestCraziest.end())
		{
                        EvacAgent *ext = *it9;//const EvacWorld::EvacAgent & ext = *it9;
                    if (ext->getId() != winner->getId() ) //->id
                    { 
                         ext->_notMoved = ext->_notMoved + 1;
                         ext->_evacTime++;
                    }                
        	it9++;
        	}


//ALL OF THIS IS PREVIOUS AND NOT USED
               /*fastestCraziest[b]->evacDist = fastestCraziest[b]->evacDist + sqrt(pow((fastestCraziest[b]->position._x - index._x),2) + pow((fastestCraziest[b]->position._y - index._y),2));
               fastestCraziest[b]->setPosition(index);
               setValue(eChemoTaxiTrails, index, getValue(eChemoTaxiTrails, index)+1));
               fastestCraziest[b]->evacTime++;
               fastestCraziest.erase(b); //SOMEHOW REMOVE THE ITEM !!!!!!!!!!!!!!!!!!!!!!!!THIS SHOULD WORK ??? Is the size adjusted?!?!?!
               for (int i=0; i<fastestCraziest.size(); i++){
    //               
                    //for the ones that don't move, maybe there is a chance for some random movement ??????
                    
                    fastestCraziest[i]->notMoved = fastestCraziest[i]->notMoved + 1;
                    fastestCraziest[i]->evacTime++;
                   }    */               
            }
    }
}

void EvacWorld::stepEnvironment()
{
        NextPosition();
        //ResolveCompetition();
        UpdateTempValues();
        ResetExits();
        std::cout<<"STEP ENV TEST"<<std::endl;

}

} // namespace Evacuation


