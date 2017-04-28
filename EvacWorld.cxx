
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
        std::cout << "creating agents" << std::endl;
	for(int i=0; i<evacConfig._numAgents; i++)
	{
		std::ostringstream oss;
		oss << "EvacAgent_"<<i;
                std::cout << "creating agents 2" << std::endl;
		EvacAgent * agent = new EvacAgent(oss.str());
                std::cout << "creating agents 3" << std::endl;
		addAgent(agent);
		// avoid agent in obstacle and cases where more than 1 agent occupies the same cell
		agent->setRandomPosition();
		int curNumAg = getValue(eObstacles, agent->getPosition());
                int currFloor = agent->floor;		
		while( (curNumAg ==1) || (getValue(eNumAgents, agent->getPosition()) > 0) || (currFloor != getValue(eFloor, agent->getPosition())) || (getValue(eDoors, agent->getPosition())==1) )
		{
			agent->setRandomPosition();
		}
		//agent->setPosition(Engine::Point2D<int>(76,423));
		//computeShortestExit(*agent); // DO I HAVE TO DO THIS THOUGH????
		setValue(eNumAgents, agent->getPosition(), getValue(eNumAgents, agent->getPosition())+1);
                std::cout<< "Creating agent"<< agent << "at position" << agent->getPosition() << std::endl;

	}
}


// this is where my get tempnext cell will be 

void EvacWorld::createRasters()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
        std::cout << "Rasters" << std::endl;
	registerDynamicRaster("obstacles", false, eObstacles);
	//Engine::GeneralState::rasterLoader().fillGDALRaster(getDynamicRaster(eObstacles), scenarioConfig._obstacleFile, getBoundaries());	
	getDynamicRaster(eObstacles).setInitValues(0, 1, 0);
	//getDynamicRaster(eObstacles).setMaxValue(1);

	registerDynamicRaster("exits", false, eExits);
	getDynamicRaster(eExits).setInitValues(0, 1, 0);
	//getDynamicRaster(eExits).setMaxValue(1);

	registerDynamicRaster("eSigns", false, eSigns);
	getDynamicRaster(eSigns).setInitValues(0, 1, 0);
	//getDynamicRaster(eSigns).setMaxValue(1);

	registerDynamicRaster("eTempCells", false, eTempCells);
	getDynamicRaster(eTempCells).setInitValues(0, 80, 0);
	//getDynamicRaster(eTempCells).setMaxValue(8);

	registerDynamicRaster("eStairs", false, eStairs);
	getDynamicRaster(eStairs).setInitValues(0, 1, 0);
	//getDynamicRaster(eStairs).setMaxValue(1);

	registerDynamicRaster("eDoors", false, eDoors);
	getDynamicRaster(eDoors).setInitValues(0, 1, 0);
	//getDynamicRaster(eDoors).setMaxValue(1);

	registerDynamicRaster("eRoomOrCoridor", false, eRoomOrCoridor);
	getDynamicRaster(eRoomOrCoridor).setInitValues(0, 2, 0);
	//getDynamicRaster(eRoomOrCoridor).setMaxValue(1);
        
	registerDynamicRaster("eChemoTaxiTrails", false, eChemoTaxiTrails);
	getDynamicRaster(eChemoTaxiTrails).setInitValues(0, std::numeric_limits<int>::max(), 0);

	//Floors will not be used at the moment, but later on will be implemented
	/*registerDynamicRaster("eFloors", false, eChemoTaxiTrails);
	getDynamicRaster(eNumAgents).setInitValues(0, floorNumber-1, 0);*/
	
	registerDynamicRaster("numAgents", false, eNumAgents);
	getDynamicRaster(eNumAgents).setInitValues(0, std::numeric_limits<int>::max(), 0);

	// this seems unnecessary

	/*registerDynamicRaster("deaths", false, eDeaths);
	getDynamicRaster(eDeaths).setInitValues(0, std::numeric_limits<int>::max(), 0);*/


	// compute exit cells - MY UPDATED VERSION
        std::cout << "First rasters" << std::endl;

	/*Engine::Point2D<int> index(0,0);
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
        std::cout << "Sec rasters" << std::endl;
	fillExitList();*/
        std::cout << "third rasters" << std::endl;

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
        if(((index._y == floor(getBoundaries()._size._height / 2)) && ((index._x >= 0 && index._x <= floor(getBoundaries()._size._width / 3))) || (((index._x >= floor(getBoundaries()._size._width / 3) + 2)) && (index._x <= floor(getBoundaries()._size._width / 2))))) {setMaxValue(eObstacles, index, 1);}
        else if ((index._x == floor(getBoundaries()._size._width / 2)) && ((index._y < floor(getBoundaries()._size._height / 2)) || (index._y >= 0))){setValue(eObstacles, index, 1);} // WAS SETMAXVALUE
        }
	//updateRasterToMaxValues(eObstacles);


        //PLACING SIGNS - BUT I STILL NEED TO MODIFY THE CONFIG FILE !!!!!!!!!!! KEEP IN MIND !!!!!!!!!! SIGNS NEED TO BE CLOSE TO OBSTACLES(WALLS)

        std::cout << "fifth rasters" << std::endl;


/*
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
        }*/
        
        //updateRasterToMaxValues(eSigns);

        std::cout << "sixth rasters" << std::endl;


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

        std::cout << "seenth rasters" << std::endl;

        //ROOM OR CORRIDOR VALUES 

    for(auto index : getBoundaries())
        {
        if( ( (index._y > 0) || (index._y < floor(getBoundaries()._size._height / 2)) ) && ((index._x > 0) || (index._x < floor(getBoundaries()._size._width / 2)) )){setMaxValue(eRoomOrCoridor, index, 1);}
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

void EvacWorld::UpdateExitValues()
{
}

void EvacWorld::ResetExits() // RESET EXIT LIST TEMP VALUE BEFORE STARTING THE NEW ROUND OF SELECTING tempNextPos !!!!
{
//engine::Point2D<int> ex;
//int i;
//for (i=0; i< _exits.size(); i++)
//    {
//    ex = _exits[i];
//    setValue(eTempCells, ex, 0) 
//    }
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

void EvacWorld::ResolveCompetition()
{
    //const EvacConfig & evacConfig = (const EvacConfig &)getConfig();
    //for(auto index : getBoundaries())
    //{
    //    if (getDynamicRaster(eTempCells).getValue(index)< 2){continue;}
    //    else {
    //           typedef std::list<EvacAgent*>  EvacAgentListTemp; // dots to arrows , GIT !!!!!!!!!!
    //           EvacAgentListTemp evacTemps = {};
    //           EvacAgentListTemp fastestCraziest = {};
    //           for (int i=0; i<evacConfig._numAgents; i++)
    //               {
    //                // HOW TO ITERATE OVER ALL AGENTS ?!?!?!!?!
    //               id << "EvacAgent_" << i;
    //               EvacAgent & agent = (EvacAgent &) (*world->getAgent(id)) ;
    //               if (agent.tempNextPos == index){evacTemps.insert(1, agent);}
    //               }
    //           for (int i=0; i<evacTemps.size(); i++){
    //                EvacAgent *maxAgent = evacTemps[0]; 
    //                if (evacTemps[i]->speed > maxAgent.speed){maxAgent = evacTemps[i];}
    //                else if ((evacTemps[i]->speed == maxAgent->speed) && (evacTemps[i]->panicLevel >= maxAgent->panicLevel)){maxAgent = evacTemps[i];}
    //                else continue;
    //              }
    //           // HOW ABOUT WE DON'T INSERT THE MAXAGENT AND JUST INSER ALL OF THE AGENTS THAT HAVE SAME DATA???
    //           //fastestCraziest.insert(1, maxAgent); // THINK HOW TO REMOVE THE DUPLICATE OF THIS maxAgent -> Removed by adding NOT EQUAL to maxAgent.id ????
    //           for (int i=0; i<evacTemps.size(); i++){
    //                if ((evacTemps[i]->speed == maxAgent->speed) && (evacTemps[i]->panicLevel == maxAgent->panicLevel) /*&& (evacTemps[i].id != maxAgent.id)*/){fastestCraziest.insert(1, evacTemps[i]);}
    //                else continue;
    //              }
    //           EvacAgent *closest = fastestCraziest[0]; // points with arrows 
    //           for (int i = 0; i< fastestCraziest.size(); i++)
    //               {
    //               if (sqrt(pow((index._x - closest->position._x),2) + pow((index._y - closest->position._y),2)) >= sqrt(pow((index._x - fastestCraziest[i]->position._x),2) + pow((index._y - fastestCraziest[i]->position._y),2))) {closest = fastestCraziest[i];}
    //               }
    //            
    //           EvacAgentListTemp *closestList;
    //           for (int i = 0; i< fastestCraziest.size(); i++)
    //                {
    //                if (sqrt(pow((index._x - closest->position._x),2) + pow((index._y - closest->position._y),2)) == sqrt(pow((index._x - fastestCraziest[i]->position._x),2) + pow((index._y - fastestCraziest[i]->position._y),2)))
    //                    {
    //                    closestList.insert(1, fastestCraziest[i]);
    //                    }
    //                }
    //           b = Engine::GeneralState::statistics().getUniformDistValue(0,closestList.size()-1);
    //           fastestCraziest[b]->evacDist = fastestCraziest[b]->evacDist + sqrt(pow((fastestCraziest[b]->position._x - index._x),2) + pow((fastestCraziest[b]->position._y - index._y),2));
    //           fastestCraziest[b]->setPosition(index);
    //           setValue(eChemoTaxiTrails, index, getValue(eChemoTaxiTrails, index)+1));
    //           fastestCraziest[b]->evacTime++;
    //           fastestCraziest.erase(b); //SOMEHOW REMOVE THE ITEM !!!!!!!!!!!!!!!!!!!!!!!!THIS SHOULD WORK ??? Is the size adjusted?!?!?!
    //           for (int i=0; i<fastestCraziest.size(); i++){
    //               
    //                //for the ones that don't move, maybe there is a chance for some random movement ??????
    //                
    //                fastestCraziest[i]->notMoved = fastestCraziest[i]->notMoved + 1;
    //                fastestCraziest[i]->evacTime++;
    //               }                   
    //        }
    //}
}

void EvacWorld::stepEnvironment()
{
        ResolveCompetition();
        //UpdateExitValues();
        ResetExits();

}

} // namespace Evacuation


