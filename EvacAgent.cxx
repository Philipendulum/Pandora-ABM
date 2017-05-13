
#include "EvacAgent.hxx"

#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>
#include <cmath>

#include <EvacWorld.hxx>
#include <EvacConfig.hxx>

namespace Evacuation
{


    EvacAgent::EvacAgent( const std::string & id, double speed, int floor,  char gender, int age, int vision, bool isOnStairs, bool exited, int panicked, Engine::Point2D<int> currGoal,/* Engine::Point2D<int> tempNextPosition,*/ int evacDist, int evacTime, int notMoved) : Agent(id)
    {
	_floor = floor; 
        _vision = vision;
	_age = age;
	_gender = gender;
	_speed = speed;
        _isOnStairs = isOnStairs;
        _exited = exited;
        _panicked = panicked;
        _currGoal._x = currGoal._x;
        _currGoal._y = currGoal._y;
        //_tempNextPosition._x = tempNextPosition._x;
        //_tempNextPosition._y = tempNextPosition._y;
        _evacDist = evacDist;
        _evacTime = evacTime;
        _notMoved = notMoved;
	//it maybe usefull here to throw errors if, for ex: age<0, gender != {"F","M"} ....

    }


EvacAgent::~EvacAgent()
{
}

// SET TEMP NEXT POSITION
void EvacAgent::SetTempNextPosition()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
    

        
    if (getWorld()->getValue(eRoomOrCoridor, getPosition())==1) // IN A ROOM
        {
        
        // WE NEED TO DEFINE HOW TO DEAL WITH EXITED PEOPLE ALREADY AND GO FROM LOCATED TO EXIT TO REMOVED OR SMT - Seems done?>!?!!?!?
        
        // WE NEED TO ADD eTempCells to the movement when it has been decided!!!!!!!! - OK ?!?!
        
        // WE MAYBE NEED TO ADD RANDOM MOVEMENTS IF AGENT PANICKED 
        // ALSO WE MAYBE NEED TO CHANGE PANIC TO SOME LEVEL IF FOR A NUMBER OF STEPS THE AGENT HAS KNOWLEDGE 0 
        
        //WE NEED TO DECIDE WHAT PANIC LEVEL DOES TO A PERSON IN A SITUATION. HOW DOES IT AFFECT HIS ACTIONS????? How do we model trampling?
        //we need to add attributes to count metrics = time of evacuation, total distance travelled. - ADD PARAMETERS FOR EVERY AGENT IN AGENT.CXX
        //let it be agent.this->_evacTime and agent.this->_evacDist - done ?!?!?!?!
        
        //WHAT HAPPENS IF 0 CELLS AROUND ARE AVAILABLE ?!?!?! Model staying in same place, and maybe agressive behaviour. - Staying in same place seems OK - tempNext is position, cell occupied. Aggressive behavopir - NEED TO COME UP WITH IDEA
        
        if (_knowledge == 0)
            {
            int seesigns = 0;
            int seedoors = 0;
            Engine::Point2D<int> index, currentPos;
            currentPos = getPosition();
            // HOW TO DEAL WITH CORNERED VIEW BLOCKED ?!?!?!?!? IT IS STILL IN RADIUS BUT IN REALITY NOT SEEN
            for (index._x = currentPos._x - _vision; index._x <= currentPos._x + _vision; index._x++) 
                {
                for (index._y = currentPos._y - _vision; index._y <= currentPos._y + _vision; index._y++)
                    {
                    if ( (index._x<0) || (index._x > getWorld()->getBoundaries()._size._width-1 ) || (index._y<0) || (index._y > getWorld()->getBoundaries()._size._height-1 )){continue;}
                    else if ((getWorld()->getValue(eSigns, index) == 1) && (getWorld()->getValue(eRoomOrCoridor, index) == 1)) {seesigns += 1;}
                    else if (getWorld()->getValue(eDoors, index) == 1) {seedoors +=1;}
                    }
                }
            if (((seedoors > 0) || (seesigns > 0)) && (this->_knowledge == 0)) // 15.05.2017 WAS if ((seedoors > 0) || (seesigns > 0))
                {
                std::cout << "SEEDORS OR SEESIGNS MORE THAN 0" <<std::endl;
                 int doorfound = 0; // when door is located, it becomes one and is recorded as the temp goal of the agent
                 int i = 1;
                 Engine::Point2D<int> radius, step;
                 while ((doorfound == 0) && (this->_knowledge == 0))
                     {
                            std::cout << "!!!!!!!!!!!!!!!!!!!!!! i EQUAL" << i <<std::endl;
                     //int count = 0;
                     for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
                        {
                        for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
                            {

                            if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 )){continue;}
                            else if (getWorld()->getValue(eDoors, radius) == 1)
                                {
                                this->_currGoal = radius;
                                doorfound += 1;
                                this->_knowledge = 1;
                                std::cout << "!!!!!!!!!!!!!!!!!!!!!! DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;
                                //break;
                                }
                            }
                        }
                     i++;
                     }
                  }
            else if((seedoors == 0) && (seesigns == 0))// RANDOM WALK IF DOESNT SEE SIGN OR DOOR
                {
                std::cout << "SEEDORS OR SEESIGNS EQUAL 0" <<std::endl;
                _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                 // HOW TO FORBID THEM TO GO THROUGH WALLS?
                while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || (getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != 1))
                    {
                    _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    }
                getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                }      
            }
        else if (_knowledge == 1) //else added 13.05.2017
            {
            Engine::Point2D<int> step, currentPos;
            currentPos = getPosition();
            _tempNextPosition = currentPos; //_currGoal; //added 15.05.2017, was _tempNextPosition = currentPos;
            int i;
            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 )){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if (sqrt(pow((_currGoal._x - step._x),2) + pow((_currGoal._y - step._y),2)) <= sqrt(pow((_currGoal._x - _tempNextPosition._x),2) + pow((_currGoal._y - _tempNextPosition._y),2)) )
                        {
                        if ((step._x < 0) || (step._x > getWorld()->getBoundaries()._size._width-1) || (step._y < 0) || (step._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, step) == 1) || (getWorld()->getValue(eRoomOrCoridor, step) == 0) ) {continue;} // here we define that the agent cant jump to the corridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                        _tempNextPosition = step;
                        }
                    }
                }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
            }
        }
    else if (getWorld()->getValue(eRoomOrCoridor, getPosition())==0) // IN THE CORRIDOR
        {
        int seesigns = 0;
        Engine::Point2D<int> index, currentPos;
        currentPos = getPosition();
        // WE NEED TO MAKE SURE THE SIGNS IN THE CORRIDOR ARE DISTINGUISHED FROM THE ONES IN ROOMS AND NOT TAKE INTO ACCOUNT SIGNS IN ROOMS FOR THIS
        for (index._x = currentPos._x - _vision; index._x <= currentPos._x + _vision; index._x++) 
            {
            for (index._y = currentPos._y - _vision; index._y <= currentPos._y + _vision; index._y++)
                {
                if ( (index._x<0) || (index._x > getWorld()->getBoundaries()._size._width-1 ) || (index._y<0) || (index._y > getWorld()->getBoundaries()._size._height-1 )){continue;}
                else if ((getWorld()->getValue(eSigns, index) == 1) && (getWorld()->getValue(eRoomOrCoridor, index)==0 )) {seesigns += 1;}
                }
            }        
            
        if ((seesigns == 0) && (_knowledge == 0)) //RANDOM WALK IF DOESNT SEE SIGNS OR DOESNT KNOW WHERE TO GO
             {
             _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
             _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
             // HOW TO FORBID THEM TO GO THROUGH WALLS?
             while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || (getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != 0) )
                 {
                 _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                 _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                 }
             getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
             }
        
        else if (_knowledge == 1) // HE KNOWS WHERE TO GO 
            {
            Engine::Point2D<int> step, currentPos;
            currentPos = getPosition();
            _tempNextPosition = currentPos;
            int i = 1;
            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 )){continue;} //protect fom out of boundaries
                    else if (sqrt(pow((_currGoal._x - step._x),2) + pow((_currGoal._y - step._y),2)) <= sqrt(pow((_currGoal._x - _tempNextPosition._x),2) + pow((_currGoal._y - _tempNextPosition._y),2)) )
                        {
                        if ((step._x < 0) || (step._x > getWorld()->getBoundaries()._size._width-1) || (step._y < 0) || (step._y > getWorld()->getBoundaries()._size._height-1)) {continue;}
                        else if ((getWorld()->getValue(eObstacles, step) != 1) && (getWorld()->getValue(eRoomOrCoridor, step) == 0)) {_tempNextPosition = step;}
                        }
                    }
                }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
            }
        else if ((_knowledge == 0) && (seesigns > 0))
            {
//// THIS IS THE NEW TEST LINE
	    Engine::World* world  = getWorld();
	    EvacWorld & evacWorld = (EvacWorld&) *world;
	    auto _exits = evacWorld.returnList();


// WE CHANGE THIS
/*            _currGoal = _exits[0];
            int i;
            for (i= 0; i< _exits.size(); i++)
                {
                if (sqrt(pow((_currGoal._x - getPosition()._x),2) + pow((_currGoal._y - getPosition()._y),2)) >= sqrt(pow((_exits[i]._x - getPosition()._x),2) + pow((_exits[i]._y - getPosition()._y),2))) {_currGoal = _exits[i];}
                }*/
        _currGoal = evacConfig.exitconfiglist.front();
        EvacConfig::ExitConfigList::const_iterator it4=evacConfig.exitconfiglist.begin();
	while(it4!=evacConfig.exitconfiglist.end())
	{
		const Engine::Point2D<int> & ext = *it4;

        if (sqrt(pow((_currGoal._x - getPosition()._x),2) + pow((_currGoal._y - getPosition()._y),2)) >= sqrt(pow((ext._x - getPosition()._x),2) + pow((ext._y - getPosition()._y),2))) {_currGoal = ext;}

		// new exit ' THIS LOOKS UNNECESSARY
        /*Engine::Point2D<int> index;
        index._x = ext._x;
        index._y = ext._y;
        //setMaxValue(eExits, index, 1);
        setValue(eExits, index, 1);*/


        it4++; 
        }
            _knowledge = 1;
            // NOW SELECT STEP LIKE PREVIOUS - DONE !!!!
            Engine::Point2D<int> step, currentPos;
            currentPos = getPosition();
            _tempNextPosition = currentPos;
            //i = 1;
            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 )){continue;} //protect from ooboundariies
                    else if (sqrt(pow((_currGoal._x - step._x),2) + pow((_currGoal._y - step._y),2)) <= sqrt(pow((_currGoal._x - _tempNextPosition._x),2) + pow((_currGoal._y - _tempNextPosition._y),2)) )
                        {
                        if ((step._x < 0) || (step._x > getWorld()->getBoundaries()._size._width-1) || (step._y < 0) || (step._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, step) == 1) || (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) {continue;}
                        _tempNextPosition = step;
                        }
                    }
                }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
            }
        }
    else if (getWorld()->getValue(eRoomOrCoridor, getPosition())==2) //WHEN YOU ARE AT THE DOOR
        {
        // _exits - list with exit signs
        _knowledge = 0;
        int seesigns = 0;
        int seeexits = 0;
        Engine::Point2D<int> index, currentPos;
        currentPos = getPosition();
        for (index._x = currentPos._x - _vision; index._x <= currentPos._x + _vision; index._x++) 
            {
            for (index._y = currentPos._y - _vision; index._y <= currentPos._y + _vision; index._y++)
                {
                if ( (index._x<0) || (index._x > getWorld()->getBoundaries()._size._width-1 ) || (index._y<0) || (index._y > getWorld()->getBoundaries()._size._height-1 )){continue;} // protect against OOP
                else if ((getWorld()->getValue(eSigns, index) == 1) && ((getWorld()->getValue(eRoomOrCoridor, index) == 0)) ) {seesigns += 1;}
                else if (getWorld()->getValue(eExits, index) == 1 ) {seeexits +=1;}
                }
            }
        if ((seesigns == 0) && (seeexits == 0)) // RANDOM IF DOESNT KNOW ANYTHING
        {
        _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
        _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
        // HOW TO FORBID THEM TO GO THROUGH WALLS?
        while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || (getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != 0) )
            {
            _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
            _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
            }
        getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
        }
        else if ((seesigns > 0) || (seeexits > 0)) //sees exits or signs 
            {
// THIS IS THE NEW TEST LINES ' TRYING A IFFERENT APPROACH RIGHT NOW
            Engine::World* world = getWorld();
            EvacWorld & evacWorld = (EvacWorld &) *world;
            auto _exits = evacWorld.returnList();

           /* _currGoal = _exits[0];
            int i; 
            for (i= 0; i< _exits.size(); i++)
                {
                if (sqrt(pow((_currGoal._x - getPosition()._x),2) + pow((_currGoal._y - getPosition()._y),2)) >= sqrt(pow((_exits[i]._x - getPosition()._x),2) + pow((_exits[i]._y - getPosition()._y),2))) {_currGoal = _exits[i];}
                }*/


        _currGoal = evacConfig.exitconfiglist.front();
        EvacConfig::ExitConfigList::const_iterator it4=evacConfig.exitconfiglist.begin();
	while(it4!=evacConfig.exitconfiglist.end())
	{
		const Engine::Point2D<int> & ext = *it4;

        if (sqrt(pow((_currGoal._x - getPosition()._x),2) + pow((_currGoal._y - getPosition()._y),2)) >= sqrt(pow((ext._x - getPosition()._x),2) + pow((ext._y - getPosition()._y),2))) {_currGoal = ext;}

        it4++;
        }


            _knowledge = 1;                
            Engine::Point2D<int> step, currentPos;
            currentPos = getPosition();            
            _tempNextPosition = currentPos;
           // i = 1;
            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 )){continue;} //protet against OOBound
                    else if (sqrt(pow((_currGoal._x - step._x),2) + pow((_currGoal._y - step._y),2)) <= sqrt(pow((_currGoal._x - _tempNextPosition._x),2) + pow((_currGoal._y - _tempNextPosition._y),2)) )
                        {
                        if ((step._x < 0) || (step._x > getWorld()->getBoundaries()._size._width-1) || (step._y < 0) || (step._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, step) == 1) || (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) {continue;}
                        _tempNextPosition = step;
                        }
                    }
                }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
            }        
        }

std::cout<<"SetTempNextPos correctly"<<std::endl;
}



void EvacAgent::updateState()
{
std::cout<<"UPDATE STATE BEGINS"<<std::endl;  

       const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
       // IS THIS THE CORRECT WAY TO REMOVE EXITED AGENTS ?!?!??!?!!?!?
       if (_exited == true)
        {
        return;
        }
    else if (getWorld()->getValue(eExits, getPosition()) == 1)
        {
        _exited = true;
        getWorld()->removeAgent(this);
        }
    
    else if(_panicked >= 1) // panic increases with more not moving
        {
        if (_notMoved > evacConfig.returnPanicTresh()) 
            {
            _panicked +=1;
            _notMoved = 0;
            }
        }
        
    else if((_notMoved > evacConfig.returnPanicTresh()) && (_panicked == 0))
        {
        _panicked = 1;
        _knowledge = 0;
        _notMoved = 0;
        }

    std::cout<<"UpdateState continued correctly until settempnext"<<std::endl;  
        SetTempNextPosition(); // NOT SURE IF THIS HAS TO BE HERE -
//        NextPosition();

std::cout<<"Updatestate correctly"<<std::endl;

// IDE on windows for methods ( or devian c++)


}

void EvacAgent::registerAttributes()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
        registerIntAttribute("panicked");
        registerIntAttribute("speed");
        registerIntAttribute("knowledge");
	registerIntAttribute("notMoved");
        registerFloatAttribute("evacDist"); 
	registerIntAttribute("evacTime"); 
	registerIntAttribute("tempNextPosition_x"); 
	registerIntAttribute("tempNextPosition_y");
	registerIntAttribute("currGoal_x");
	registerIntAttribute("currGoal_y");
        //registerIntAttribute("tempNextPosition");  
        std::cout<<"RegisterAttributes correctly"<<std::endl;          
}

void EvacAgent::serialize()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
        serializeAttribute("panicked", _panicked);
        serializeAttribute("speed", _speed);
        serializeAttribute("knowledge", _knowledge);
        serializeAttribute("notMoved", _notMoved);
        serializeAttribute("evacTime", _evacTime);
        serializeAttribute("evacDist", _evacDist);
        serializeAttribute("tempNextPosition_x", _tempNextPosition._x);      
        serializeAttribute("tempNextPosition_y", _tempNextPosition._y);   
        serializeAttribute("currGoal_x", _currGoal._x);   
        serializeAttribute("currGoal_y", _currGoal._y);     
        std::cout<<"Serialize correctly"<<std::endl;  
}
	
std::string EvacAgent::agentCharac()
{
	std::ostringstream charac;
	charac << getId() <<" is a "<<this->_gender<<" of "<<this->_age<<" yo with v:"<<this->_vision<<" and s:"<<this->_speed<<" and position"<<this->getPosition();
	return charac.str();
    
}
/*void EvacAgent::setExit( const Engine::Point2D<int> & exit )
{
	_exit = exit;
}*/

} // namespace Evacuation


// rabndom number seed 

