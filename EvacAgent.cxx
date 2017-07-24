
#include "EvacAgent.hxx"

#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>
#include <cmath>

#include <EvacWorld.hxx>
#include <EvacConfig.hxx>

namespace Evacuation
{


    EvacAgent::EvacAgent( const std::string & id, double speed, int floor,  char gender, int age, int vision, bool isOnStairs, bool exited, int panicked, int evacDist, int evacTime, int notMoved, int notMovedTotal) : Agent(id)
/*Engine::Point2D<int> currGoal(/,/* Engine::Point2D<int> tempNextPosition,*/ 
    {
	_floor = floor; 
        _vision = vision;
	_age = age;
	_gender = gender;
	_speed = speed;
        _isOnStairs = isOnStairs;
        _exited = exited;
        _panicked = panicked;
        //_currGoal.push_front(currGoal);
        //_currGoal._x = currGoal._x;
        //_currGoal._y = currGoal._y;
        //_tempNextPosition._x = tempNextPosition._x;
        //_tempNextPosition._y = tempNextPosition._y;
        _evacDist = evacDist;
        _evacTime = evacTime;
        _notMoved = notMoved;
        _notMovedTotal = notMovedTotal;
	//it maybe usefull here to throw errors if, for ex: age<0, gender != {"F","M"} ....

    }


EvacAgent::~EvacAgent()
{
}

void EvacAgent::SetTempNextPosition()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();

    if ((getWorld()->getValue(eRoomOrCoridor, getPosition())!=0) && (getWorld()->getValue(eStairs, getPosition())==0) ) // IN ONE OF THE FLOOR ROOMS
        {
        std::cout << "In one of the floor rooms" << std::endl;
        if (_knowledge == 0)
            {
             std::cout << "if knowledge == 0" << std::endl; 
            int seesigns = 0;
            int seedoors = 0;
            Engine::Point2D<int> index, currentPos;
            currentPos = getPosition();
            // HOW TO DEAL WITH CORNERED VIEW BLOCKED ?!?!?!?!? IT IS STILL IN RADIUS BUT IN REALITY NOT SEEN
            for (index._x = currentPos._x - _vision; index._x <= currentPos._x + _vision; index._x++) 
                {
                for (index._y = currentPos._y - _vision; index._y <= currentPos._y + _vision; index._y++)
                    {
                    if ( (index._x<0) || (index._x > getWorld()->getBoundaries()._size._width-1 ) || (index._y<0) || (index._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, index) != getWorld()->getValue(eFloor, currentPos))){continue;}
                    else if ( (getWorld()->getValue(eSigns, index) == 1) && (getWorld()->getValue(eRoomOrCoridor, index) == getWorld()->getValue(eRoomOrCoridor, currentPos)) ) {seesigns += 1;}
                    else if (getWorld()->getValue(eDoors, index) == 1) 
                        {
                            std::cout << "if (getWorld()->getValue(eDoorValue, index) == 1)" <<std::endl;
                        if (getWorld()->getValue(eDoorValue, index) == 1)
                            {
                            if (getWorld()->getValue(eDoorDir, index) == 4) //L to R
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x - 1;
                                check._y = index._y;
                                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            else if (getWorld()->getValue(eDoorDir, index) == 1) //D to U
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x;
                                check._y = index._y + 1;
                                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            else if (getWorld()->getValue(eDoorDir, index) == 2) // R to L
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x + 1;
                                check._y = index._y;
                                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            else if (getWorld()->getValue(eDoorDir, index) == 3) // U TO D
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x;
                                check._y = index._y - 1;
                                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            }
                        }
                    }
                }

            if (((seedoors > 0) || (seesigns > 0)) && (this->_knowledge == 0)) 
                {    
                std::cout << "SEEDORS OR SEESIGNS MORE THAN 0 one" <<std::endl;
                int doorfound = 0; // when door is located, it becomes one and is recorded as the temp goal of the agent
                int i = 1;
                Engine::Point2D<int> radius, step;
                while ((doorfound == 0) && (this->_knowledge == 0))
                    {
                    std::cout << "!!!!!!!!!!!!!!!!!!!!!! i EQUAL FIRST" << i << "and position " << currentPos <<std::endl;
                    for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
                        {
                        for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
                            {
                            if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, radius) != getWorld()->getValue(eFloor, currentPos)))
                                {
                                continue;}
                            else if (getWorld()->getValue(eDoors, radius) == 1)
                                {
                                if (getWorld()->getValue(eDoorValue, radius) == 1)
                                    {
                                    if (getWorld()->getValue(eDoorDir, radius) == 4) //L to R
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x - 1;
                                        check._y = radius._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)) 
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._y - evacConfig.maxDoorWidth; j <= radius._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = radius._x;
                                                point._y = j;
                                                if ((j!= radius._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, radius) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, radius) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, radius) == 1) //D to U
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x;
                                        check._y = radius._y + 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check))
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._x - evacConfig.maxDoorWidth; j <= radius._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = radius._y;
                                                if ((j!= radius._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, radius) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, radius) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, radius) == 2) // R to L
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x + 1;
                                        check._y = radius._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check))
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._y - evacConfig.maxDoorWidth; j <= radius._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = radius._x;
                                                point._y = j;
                                                if ((j!= radius._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, radius) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, radius) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, radius) == 3) // U TO D
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x;
                                        check._y = radius._y - 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check))
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._x - evacConfig.maxDoorWidth; j <= radius._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = radius._y;
                                                if ((j!= radius._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, radius) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, radius) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    }
                                }
                            }
                        }
                     i++;
                     }
                 i = 1; // NEW LINE 
                 currentPos = getPosition();
                 _tempNextPosition = currentPos;
                 float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
               

                 // ITERATOR OVER CURRGOAL
                 currGoalsList::const_iterator itcurr=_currGoal.begin();
            
	         while(itcurr!=_currGoal.end())
	             {
		     const Engine::Point2D<int> & ext = *itcurr;

                     for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                         {
                         for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                             {
                             if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                             else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
                                 {
                                 if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) &&  (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) || (getWorld()->getValue(eOccupied, step) == 1) ) {continue;} // here we define that the agent cant jump to the Coridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                                 _tempNextPosition = step;
                                 distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
                                 }
                             }
                         }
                     itcurr++; 
                     }
                 getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                 }
            else if ((seedoors == 0) && (seesigns == 0))
                { 
                std::cout << "if ((seedoors == 0) && (seesigns == 0)) " <<std::endl;
                Engine::Point2D<int> radius;
                int agentsAroundHelp = 0;
                //FUNCTION TO TRY TO TALK TO SOMEBODY - SEEMS COMPLICATED
                for (radius._x = getPosition()._x - evacConfig.talkingRadius; radius._x <= getPosition()._x + evacConfig.talkingRadius; radius._x++)
                    {
                    for (radius._y = getPosition()._y - evacConfig.talkingRadius; radius._y <= getPosition()._y + evacConfig.talkingRadius; radius._y++)
                        {
                        if ( (radius._x < 0) || (radius._y < 0) || (radius._x > getWorld()->getBoundaries()._size._width-1) || (radius._y > getWorld()->getBoundaries()._size._height-1) || ((radius._x == getPosition()._x) && (radius._y == getPosition()._y)) || (getWorld()->getValue(eOccupied, radius) == 0) ) {continue;}
                        else 
                            {
                            if ((getWorld()->getValue(eKnowledge, radius) == 1) && (getWorld()->getValue(eRoomOrCoridor, radius) == getWorld()->getValue(eRoomOrCoridor, getPosition())))
                                {
                                int talkOrNot = Engine::GeneralState::statistics().getUniformDistValue(0, 100);
                                if (talkOrNot < evacConfig.talkingProb) // FULLY COPIED FROM IF SEESINS OR SEEDORS > 0
                                    {

					int doorfound = 0; // when door is located, it becomes one and is recorded as the temp goal of the agent
					int i = 1;
					Engine::Point2D<int> radius, step;
					while ((doorfound == 0) && (this->_knowledge == 0))
					    {
					    std::cout << "!!!!!!!!!!!!!!!!!!!!!! i EQUAL SECOND" << i <<std::endl;
					    for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
						{
						for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
						    {
						    if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, radius) != getWorld()->getValue(eFloor, currentPos))){continue;}
						    else if (getWorld()->getValue(eDoors, radius) == 1)
						        {
						        if (getWorld()->getValue(eDoorValue, radius) == 1)
						            {
						            if (getWorld()->getValue(eDoorDir, radius) == 4) //L to R
						                {
						                Engine::Point2D<int> check;
						                check._x = radius._x - 1;
						                check._y = radius._y;
						                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)) 
						                    {
						                    this->_currGoal.clear();
						                    this->_currGoal.push_front(radius);
						                    int j;
						                    for (j = radius._y - evacConfig.maxDoorWidth; j <= radius._y + evacConfig.maxDoorWidth; j++)
						                        {
						                        Engine::Point2D<int> point;
						                        point._x = radius._x;
						                        point._y = j;
						                        if ((j!= radius._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
						                        }
						                    doorfound += 1;
						                    this->_knowledge = 1;
						                    std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
						                    }
						                }
						            else if (getWorld()->getValue(eDoorDir, radius) == 1) //D to U
						                {
						                Engine::Point2D<int> check;
						                check._x = radius._x;
						                check._y = radius._y + 1;
						                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check))
						                    {
						                    this->_currGoal.clear();
						                    this->_currGoal.push_front(radius);
						                    int j;
						                    for (j = radius._x - evacConfig.maxDoorWidth; j <= radius._x + evacConfig.maxDoorWidth; j++)
						                        {
						                        Engine::Point2D<int> point;
						                        point._x = j;
						                        point._y = radius._y;
						                        if ((j!= radius._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
						                        }
						                    doorfound += 1;
						                    this->_knowledge = 1;
						                    std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
						                    }
						                }
						            else if (getWorld()->getValue(eDoorDir, radius) == 2) // R to L
						                {
						                Engine::Point2D<int> check;
						                check._x = radius._x + 1;
						                check._y = radius._y;
						                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check))
						                    {
						                    this->_currGoal.clear();
						                    this->_currGoal.push_front(radius);
						                    int j;
						                    for (j = radius._y - evacConfig.maxDoorWidth; j <= radius._y + evacConfig.maxDoorWidth; j++)
						                        {
						                        Engine::Point2D<int> point;
						                        point._x = radius._x;
						                        point._y = j;
						                        if ((j!= radius._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
						                        }
						                    doorfound += 1;
						                    this->_knowledge = 1;
						                    std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
						                    }
						                }
						            else if (getWorld()->getValue(eDoorDir, radius) == 3) // U TO D
						                {
						                Engine::Point2D<int> check;
						                check._x = radius._x;
						                check._y = radius._y - 1;
						                if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check))
						                    {
						                    this->_currGoal.clear();
						                    this->_currGoal.push_front(radius);
						                    int j;
						                    for (j = radius._x - evacConfig.maxDoorWidth; j <= radius._x + evacConfig.maxDoorWidth; j++)
						                        {
						                        Engine::Point2D<int> point;
						                        point._x = j;
						                        point._y = radius._y;
						                        if ((j!= radius._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
						                        }
						                    doorfound += 1;
						                    this->_knowledge = 1;
						                    std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
						                    }
						                }
						            }
						        }
						    }
						}
					     i++;
					     }
					 i = 1; 
					 currentPos = getPosition();
					 _tempNextPosition = currentPos;


/////////////////////////////HERE WE TAKE OUT SO THAT WHEN KNOWLEDGE CHANGES FROM 0 to 1 the agent only moves the next step 


					 float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
				       

					 // ITERATOR OVER CURRGOAL
					 currGoalsList::const_iterator itcurr=_currGoal.begin();
				    
					 while(itcurr!=_currGoal.end())
					     {
					     const Engine::Point2D<int> & ext = *itcurr;

					     for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
						 {
						 for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
						     {
						     if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
						     else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
						         {
						         if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) &&  (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) || (getWorld()->getValue(eOccupied, step) == 1) ) {continue;} // here we define that the agent cant jump to the Coridor. 0 is door and everything else rooms         
						// WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
						         _tempNextPosition = step;
						         distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
						         }
						     }
						 }
					     itcurr++; 
					     }
					 getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                                         agentsAroundHelp = agentsAroundHelp + 1;
                                    }
                                else {continue;}
                                }
                            // get agent ID, check his knowledge  and then try to talk to him !!!!!!!!
                            }
                        }
                    }
                if (agentsAroundHelp == 0)
                    {
                    std::cout << "SEEDORS OR SEESIGNS EQUAL 0 THIRD" <<std::endl;
                    _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    // HOW TO FORBID THEM TO GO THROUGH WALLS?
                    while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || (getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) || (getWorld()->getValue(eOccupied, _tempNextPosition) == 1))
                        {
                        _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                        _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                        }
                    getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                    }

                }
            }

        else if (_knowledge == 1)
            {
                 std::cout<<" Else if knowledge = 1 error"<<std::endl;
                 Engine::Point2D<int> step, currentPos;
                 currentPos = getPosition();
                 _tempNextPosition = currentPos;
                 float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
                 // ITERATOR OVER CURRGOAL
                 currGoalsList::const_iterator itcurr=_currGoal.begin();
            
	         while(itcurr!=_currGoal.end())
	             {
		     const Engine::Point2D<int> & ext = *itcurr;

                     for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                         {
                         for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                             {
                             if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                             else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
                                 {
                                 if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) &&  (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) || (getWorld()->getValue(eOccupied, step) == 1) ) {continue;} // here we define that the agent cant jump to the Coridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                                 _tempNextPosition = step;
                                 distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
                                 }
                             }
                         }
                     itcurr++; 
                     }
                 getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);

            }
        } // END OF FIRST GENERAL CONDITION

    else if ((getWorld()->getValue(eRoomOrCoridor, getPosition())==0) && (getWorld()->getValue(eStairs, getPosition())!=2) ) // IN A REGULAR DOOR NOT LEADING TO STAIRS
        {
           std::cout << "In one of the doors not to stairs" << std::endl;
            _knowledge = 0;
            int seesigns = 0;
            int seedoors = 0;
            Engine::Point2D<int> index, currentPos, compare; // compare - DOOR EXIT DIRECTION NEXT POINT
            if (getWorld()->getValue(eDoorDir, getPosition()) == 4) 
                {
                compare._x = getPosition()._x + 1;
                compare._y = getPosition()._y;
                }
            else if (getWorld()->getValue(eDoorDir, getPosition()) == 1) 
                {
                compare._x = getPosition()._x;
                compare._y = getPosition()._y - 1;
                }      
            else if (getWorld()->getValue(eDoorDir, getPosition()) == 2) 
                {
                compare._x = getPosition()._x - 1;
                compare._y = getPosition()._y;
                }      
            else if (getWorld()->getValue(eDoorDir, getPosition()) == 3) 
                {
                compare._x = getPosition()._x;
                compare._y = getPosition()._y + 1; 
                }                

            currentPos = getPosition();
            // HOW TO DEAL WITH CORNERED VIEW BLOCKED ?!?!?!?!? IT IS STILL IN RADIUS BUT IN REALITY NOT SEEN
            for (index._x = currentPos._x - _vision; index._x <= currentPos._x + _vision; index._x++) 
                {
                for (index._y = currentPos._y - _vision; index._y <= currentPos._y + _vision; index._y++)
                    {
                    if ( (index._x<0) || (index._x > getWorld()->getBoundaries()._size._width-1 ) || (index._y<0) || (index._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, index) != getWorld()->getValue(eFloor, compare))){continue;}
                    else if ( (getWorld()->getValue(eSigns, index) == 1) && (getWorld()->getValue(eRoomOrCoridor, index) == getWorld()->getValue(eRoomOrCoridor, compare)) ) {seesigns += 1;}
                    else if (getWorld()->getValue(eDoors, index) == 1) 
                        {
                        if (getWorld()->getValue(eDoorValue, index) == 1)
                            {
                            if (getWorld()->getValue(eDoorDir, index) == 4) //L to R
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x - 1;
                                check._y = index._y;
                                if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            else if (getWorld()->getValue(eDoorDir, index) == 1) //D to U
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x;
                                check._y = index._y + 1;
                                if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            else if (getWorld()->getValue(eDoorDir, index) == 2) // R to L
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x + 1;
                                check._y = index._y;
                                if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            else if (getWorld()->getValue(eDoorDir, index) == 3) // U TO D
                                {
                                Engine::Point2D<int> check;
                                check._x = index._x;
                                check._y = index._y - 1;
                                if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check)) {seedoors +=1;}
                                }
                            }
                        }
                    }
                }

            if (((seedoors > 0) || (seesigns > 0)) && (this->_knowledge == 0)) 
                {    
                std::cout << "SEEDORS OR SEESIGNS MORE THAN 0 two" <<std::endl;
                int doorfound = 0; // when door is located, it becomes one and is recorded as the temp goal of the agent
                int i = 1;
                Engine::Point2D<int> radius, step;
                while ((doorfound == 0) && (this->_knowledge == 0))
                    {
                    std::cout << "!!!!!!!!!!!!!!!!!!!!!! i EQUAL FOURTH" << i <<std::endl;
                    for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
                        {
                        for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
                            {
                            if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, radius) != getWorld()->getValue(eFloor, compare))){continue;} // currentPos was compare
                            else if (getWorld()->getValue(eDoors, radius) == 1)
                                {
                                if (getWorld()->getValue(eDoorValue, radius) == 1)
                                    {
                                    if (getWorld()->getValue(eDoorDir, radius) == 4) //L to R
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x - 1;
                                        check._y = radius._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check)) 
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._y - evacConfig.maxDoorWidth; j <= radius._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = radius._x;
                                                point._y = j;
                                                if ((j!= radius._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, radius) == 1) //D to U
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x;
                                        check._y = radius._y + 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check))
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._x - evacConfig.maxDoorWidth; j <= radius._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = radius._y;
                                                if ((j!= radius._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, radius) == 2) // R to L
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x + 1;
                                        check._y = radius._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check))
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._y - evacConfig.maxDoorWidth; j <= radius._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = radius._x;
                                                point._y = j;
                                                if ((j!= radius._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir,radius) == 3) // U TO D
                                        {
                                        Engine::Point2D<int> check;
                                        check._x = radius._x;
                                        check._y = radius._y - 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, compare) == getWorld()->getValue(eRoomOrCoridor, check))
                                            {
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(radius);
                                            int j;
                                            for (j = radius._x - evacConfig.maxDoorWidth; j <= radius._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = radius._y;
                                                if ((j!= radius._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, index) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, index) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            doorfound += 1;
                                            this->_knowledge = 1;
                                            std::cout << "DOORFOUND 1 CURRGOAL SHOULD BE UPDATED" <<std::endl;                                             
                                            }
                                        }
                                    }
                                }
                            }
                        }
                     i++;
                     }
                 i = 1; 
                 currentPos = getPosition();
                 _tempNextPosition = currentPos;
                 float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
               

                 // ITERATOR OVER CURRGOAL
                 currGoalsList::const_iterator itcurr=_currGoal.begin();
            
	         while(itcurr!=_currGoal.end())
	             {
		     const Engine::Point2D<int> & ext = *itcurr;

                     for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                         {
                         for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                             {
                             if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, compare))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                             else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
                                 {
                                 if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, compare)) && (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) || (getWorld()->getValue(eOccupied, step) == 1) ) {continue;} // here we define that the agent cant jump to the Coridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                                 _tempNextPosition = step;
                                 distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
                                 }
                             }
                         }
                     itcurr++; 
                     }
                 getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                 }
            else if ((seedoors == 0) && (seesigns == 0)) // NOTE!! AGENTS WONT TALK IN THE DOOR, ONLY AFTER PASSING IT
                {
                Engine::Point2D<int> compare;                //DOOR EXIT DIRECTION NEXT POINT
                if (getWorld()->getValue(eDoorDir, getPosition()) == 4) 
                    {
                    compare._x = getPosition()._x + 1;
                    compare._y = getPosition()._y;
                    }
                else if (getWorld()->getValue(eDoorDir, getPosition()) == 1) 
                    {
                    compare._x = getPosition()._x;
                    compare._y = getPosition()._y - 1;
                    }      
                else if (getWorld()->getValue(eDoorDir, getPosition()) == 2) 
                    {
                    compare._x = getPosition()._x - 1;
                    compare._y = getPosition()._y;
                    }      
                else if (getWorld()->getValue(eDoorDir, getPosition()) == 3) 
                    {
                    compare._x = getPosition()._x;
                    compare._y = getPosition()._y + 1; 
                    }                

                _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    // HOW TO FORBID THEM TO GO THROUGH WALLS?
                while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || ( getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != getWorld()->getValue(eRoomOrCoridor, compare) ) || (getWorld()->getValue(eOccupied, _tempNextPosition) == 1))
                    {
                    _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _speed), _speed);
                    }
                getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                }  
        }


    else if ((getWorld()->getValue(eRoomOrCoridor, getPosition())==0) && (getWorld()->getValue(eStairs, getPosition())==2) ) // IN A DOOR LEADING TO STAIRS
        {
        std::cout << "In one of thedoors leading to stairs" << std::endl;
        _knowledge = 0;
        Engine::Point2D<int> radius, compare, currentPos, around, step;
        int turnDoorFound = 0;
        int i = 1;
        currentPos = getPosition();
// WE NEED TO DO SOMETHING WITH SPEEDS, SUCH AS EXAMPLE:
        if (_speed == 3) {_speed = 2;}
        else if (_speed == 2) {_speed = 1;}
// STILL NEED TO THINK ABOUT SPEED CONVERSION
        if (getWorld()->getValue(eDoorDir, getPosition()) == 4) 
            {
            compare._x = getPosition()._x + 1;
            compare._y = getPosition()._y;
            }
        else if (getWorld()->getValue(eDoorDir, getPosition()) == 1) 
            {
            compare._x = getPosition()._x;
            compare._y = getPosition()._y - 1;
            }      
        else if (getWorld()->getValue(eDoorDir, getPosition()) == 2) 
            {
            compare._x = getPosition()._x - 1;
            compare._y = getPosition()._y;
            }      
        else if (getWorld()->getValue(eDoorDir, getPosition()) == 3) 
            {
            compare._x = getPosition()._x;
            compare._y = getPosition()._y + 1; 
            }                
        while (turnDoorFound == 0)
            {
            for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
                {
                for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
                    {
                    if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, radius) != getWorld()->getValue(eFloor, compare))){continue;}
                    else if ((getWorld()->getValue(eStairs, radius) == 3))
                        {
                        this->_currGoal.clear();
                        this->_currGoal.push_front(radius);
                        for (around._x = radius._x - evacConfig.maxDoorWidth; around._x <= radius._x + evacConfig.maxDoorWidth; around._x++)
                            {
                            for (around._y = radius._y - evacConfig.maxDoorWidth; around._y <= radius._y + evacConfig.maxDoorWidth; around._y++)
                                {
                                if ( (getWorld()->getValue(eStairs, around) == 3) && ((around._x != radius._x) || (around._y != radius._y)) ) {this->_currGoal.push_front(around);}
                                }
                            }
                        turnDoorFound = turnDoorFound +1;
                        }                  
                    }
                }
            i++;
            }
        

        i = 1;
        _tempNextPosition = currentPos;
        float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
            

         // ITERATOR OVER CURRGOAL
        currGoalsList::const_iterator itcurr=_currGoal.begin();
         
        while(itcurr!=_currGoal.end())
            {
	    const Engine::Point2D<int> & ext = *itcurr;

            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
                        {
                        if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eStairs, step) != 1 ) && (getWorld()->getValue(eStairs, step) != 3 ) ) || (getWorld()->getValue(eOccupied, step) == 1)) {continue;} // here we define that the agent cant jump to the Coridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                        _tempNextPosition = step;
                        distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
                        }
                    }
                }
                itcurr++; 
            }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
        } 


    else if ((getWorld()->getValue(eRoomOrCoridor, getPosition())!=0) && (getWorld()->getValue(eStairs, getPosition())==1) ) // ON THE STAIRS
        {
         std::cout << "on the stairs" << std::endl;
        Engine::Point2D<int> radius, compare, currentPos, around, step;
        currentPos = getPosition();
        _tempNextPosition = currentPos;
        float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
            
         // ITERATOR OVER CURRGOAL
        currGoalsList::const_iterator itcurr=_currGoal.begin();
         
        while(itcurr!=_currGoal.end())
            {
	    const Engine::Point2D<int> & ext = *itcurr;
            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
                        {
                        if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eStairs, step) != 1 ) && (getWorld()->getValue(eStairs, step) != 3 ) && (getWorld()->getValue(eStairs, step) != 4) ) || (getWorld()->getValue(eOccupied, step) == 1)) {continue;} // here we define that the agent cant jump to the Coridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                        _tempNextPosition = step;
                        distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
                        }
                    }
                }
                itcurr++; 
            }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
        } 


    else if ((getWorld()->getValue(eRoomOrCoridor, getPosition())!=0) && (getWorld()->getValue(eStairs, getPosition())==3) ) // ON THE STAIRS TURN POINT
        {
        std::cout << "on stairs turn point" << std::endl;
        Engine::Point2D<int> radius, currentPos, around, step;
        int jumpDoorFound = 0;
        int i = 1;
        currentPos = getPosition();
// WE NEED TO DO SOMETHING WITH SPEEDS, SUCH AS EXAMPLE:
        if (_speed == 3) {_speed = 2;}
        else if (_speed == 2) {_speed = 1;}
// STILL NEED TO THINK ABOUT SPEED CONVERSION
                
        while (jumpDoorFound == 0)
            {
            for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
                {
                for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
                    {
                    if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, radius) != getWorld()->getValue(eFloor, currentPos))){continue;}
                    else if ((getWorld()->getValue(eStairs, radius) == 4))
                        {
                        this->_currGoal.clear();
                        this->_currGoal.push_front(radius);
                        for (around._x = radius._x - evacConfig.maxDoorWidth; around._x <= radius._x + evacConfig.maxDoorWidth; around._x++)
                            {
                            for (around._y = radius._y - evacConfig.maxDoorWidth; around._y <= radius._y + evacConfig.maxDoorWidth; around._y++)
                                {
                                if ( (getWorld()->getValue(eStairs, around) == 4) && ((around._x != radius._x) || (around._y != radius._y)) ) {this->_currGoal.push_front(around);}
                                }
                            }
                        jumpDoorFound = jumpDoorFound +1;
                        }                  
                    }
                }
            i++;
            }
        
        i = 1;
        _tempNextPosition = currentPos;
        float distance = sqrt(pow((_currGoal.front()._x - currentPos._x),2) + pow((_currGoal.front()._y - currentPos._y),2));
            

         // ITERATOR OVER CURRGOAL
        currGoalsList::const_iterator itcurr=_currGoal.begin();
         
        while(itcurr!=_currGoal.end())
            {
	    const Engine::Point2D<int> & ext = *itcurr;

            for (step._x = currentPos._x - _speed; step._x <= currentPos._x + _speed; step._x++)
                {
                for (step._y = currentPos._y - _speed; step._y <= currentPos._y + _speed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if (sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2)) < distance )
                        {
                        if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eStairs, step) != 1 ) && (getWorld()->getValue(eStairs, step) != 4 ) ) || (getWorld()->getValue(eOccupied, step) == 1)) {continue;} // here we define that the agent cant jump to the Coridor. 1 is room and 2 is door         
                        // WE ALSO NEED TO DEFINE SOMEHOW THAT THE AGENTS DO NOT GO THROUGH CORNERS OR TO OTHER ROOMS
                        _tempNextPosition = step;
                        distance = sqrt(pow((ext._x - step._x),2) + pow((ext._y - step._y),2));
                        }
                    }
                }
            itcurr++; 
            }
            getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
        }     

    else if ((getWorld()->getValue(eRoomOrCoridor, getPosition())!=0) && (getWorld()->getValue(eStairs, getPosition())==4) ) // ON THE STAIRS JUMP POINT
        {
        std::cout << "On the jump point" << std::endl;
        EvacConfig::JumpCellsList::const_iterator itjump=evacConfig.jumpCellsfrom.begin();
        EvacConfig::JumpCellsList::const_iterator itjumpto=evacConfig.jumpCellsto.begin();
        while(itjump!=evacConfig.jumpCellsfrom.end())
            {
	    const Engine::Point2D<int> & ext = *itjump;
            const Engine::Point2D<int> & ext2 = *itjumpto;
            if ( (getPosition()._x == ext._x) && (getPosition()._y == ext._y) )
                {
                if (getWorld()->getValue(eOccupied, ext2)==0) 
                    {
                    _tempNextPosition._x = ext2._x;
                    _tempNextPosition._y = ext2._y;
                    }
                else
                    {
                    _tempNextPosition._x = getPosition()._x;
                    _tempNextPosition._y = getPosition()._y;
                    }
                getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);
                break;
                }
            itjump++;
            itjumpto++;
            }
        }  
} // END OF SETTEMPNEXTPOSITION





void EvacAgent::updateState()
{
std::cout<<"UPDATE STATE BEGINS"<<std::endl;  

       const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
       if (_exited == true)
        {
        return;
        }
    else if (getWorld()->getValue(eExits, getPosition()) == 1)
        {
        _exited = true;
        getWorld()->removeAgent(this);
	Engine::World* world  = getWorld();
	EvacWorld & evacWorld = (EvacWorld&) *world;
	auto al = evacWorld.returnAllList();
        evacWorld.removeRemovedAgent(this);
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
        _knowledge = 0; // KNOWLEDGE CAN BE LEFT IF TOO PANICKED ?!?!?!?!!??!?!
        _notMoved = 0;
        }

    std::cout<<"UpdateState continued correctly until settempnext"<<std::endl;  
    SetTempNextPosition();

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
	registerIntAttribute("currGoal.begin()._x");
	registerIntAttribute("currGoal.begin()._y");
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
        serializeAttribute("currGoal.begin()._x", _currGoal.front()._x);   
        serializeAttribute("currGoal.begin()._y", _currGoal.front()._y);     
        std::cout<<"Serialize correctly"<<std::endl;  
}
	
std::string EvacAgent::agentCharac()
{
	std::ostringstream charac;
	charac << getId() <<" is a "<<this->_gender<<" of "<<this->_age<<" yo with v:"<<this->_vision<<" and s:"<<this->_speed<<" and position"<<this->getPosition();
	return charac.str();
    
}


} // namespace Evacuation


// rabndom number seed 

