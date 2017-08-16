
#include "EvacAgent.hxx"

#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>
#include <cmath>

#include <EvacWorld.hxx>
#include <EvacConfig.hxx>

namespace Evacuation
{


    EvacAgent::EvacAgent( const std::string & id, int speed, float mass, int stairsSpeed,  int floor,  char gender, int age, int vision, bool isOnStairs, bool exited, bool trampled, int getuptime, int startDelay, bool justEvaded, int panicked, int directionDarkness, bool touchedWall, int evacDist, int evacTime, int notMoved, int notMovedTotal, int knowledge) : Agent(id)
    {
	_floor = floor; 
        _vision = vision;
	_age = age;
	_gender = gender;
        _mass = mass;
	_speed = speed;
        _currentSpeed = speed;
        _stairsSpeed = stairsSpeed;
        _isOnStairs = isOnStairs;
        _exited = exited;
        _trampled = trampled;
        _justEvaded = justEvaded;
        _getuptime = getuptime; 
        _startDelay = startDelay;
        _panicked = panicked;
        _directionDarkness = directionDarkness;
        _touchedWall = touchedWall;
        _evacDist = evacDist;
        _evacTime = evacTime;
        _notMoved = notMoved;
        _notMovedTotal = notMovedTotal;
        _knowledge = knowledge;
	//it maybe usefull here to throw errors if, for ex: age<0, gender != {"F","M"} ....
    }


EvacAgent::~EvacAgent()
{
}

void EvacAgent::DarknessActions()
{
    _knowledge = 0;
    Engine::Point2D<int> index, currentPos, check;
    currentPos = getPosition();
    if (_justEvaded == true)
        {
        int nearwall = 0;
        for (index._x = currentPos._x - 2; index._x <= currentPos._x + 2; index._x++) 
            {
	    for (index._y = currentPos._y - 2; index._y <= currentPos._y + 2; index._y++)
	        {
                if (getWorld()->getValue(eObstacles, index) == 1) {nearwall++;}
                }
            }
        if (_directionDarkness == 1)
            {
            if (nearwall==2)
                {
                _tempNextPosition._x = currentPos._x;
                _tempNextPosition._y = currentPos._y - 1;
                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                }
            else 
                {
                Engine::Point2D<int> test, test2;
                test._x = currentPos._x-2;
                test._y = currentPos._y;
                test2._x = currentPos._x+2;
                test2._y = currentPos._y;
                if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 0 )
                    {
                    _tempNextPosition._x = currentPos._x-1;
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 0 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    _tempNextPosition._x = currentPos._x+1;
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
                    if (flip == 0) {_tempNextPosition._x = currentPos._x+1;}
                    else if (flip == 1) {_tempNextPosition._x = currentPos._x-1;}                    
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                }
            }
        else if (_directionDarkness == 2)
            {
            if (nearwall==2)
                {
                _tempNextPosition._x = currentPos._x-1;
                _tempNextPosition._y = currentPos._y;
                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                }
            else 
                {
                Engine::Point2D<int> test, test2;
                test._x = currentPos._x;
                test._y = currentPos._y-2;
                test2._x = currentPos._x;
                test2._y = currentPos._y+2;
                if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 0 )
                    {
                    _tempNextPosition._x = currentPos._x;
                    _tempNextPosition._y = currentPos._y-1;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 0 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    _tempNextPosition._x = currentPos._x;
                    _tempNextPosition._y = currentPos._y+1;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
                    if (flip == 0) {_tempNextPosition._x = currentPos._y+1;}
                    else if (flip == 1) {_tempNextPosition._x = currentPos._y-1;}                    
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                }            
            }
        else if (_directionDarkness == 3)
            {
            if (nearwall==2)
                {
                _tempNextPosition._x = currentPos._x;
                _tempNextPosition._y = currentPos._y + 1;
                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}

                }
            else 
                {
                Engine::Point2D<int> test, test2;
                test._x = currentPos._x-2;
                test._y = currentPos._y;
                test2._x = currentPos._x+2;
                test2._y = currentPos._y;
                if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 0 )
                    {
                    _tempNextPosition._x = currentPos._x-1;
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 0 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    _tempNextPosition._x = currentPos._x+1;
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
                    if (flip == 0) {_tempNextPosition._x = currentPos._x+1;}
                    else if (flip == 1) {_tempNextPosition._x = currentPos._x-1;}                    
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                }         
            }
        else if (_directionDarkness == 4)
            {
            if (nearwall==2)
                {
                _tempNextPosition._x = currentPos._x+1;
                _tempNextPosition._y = currentPos._y;
                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                }
            else 
                {
                Engine::Point2D<int> test, test2;
                test._x = currentPos._x;
                test._y = currentPos._y-2;
                test2._x = currentPos._x;
                test2._y = currentPos._y+2;
                if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 0 )
                    {
                    _tempNextPosition._x = currentPos._x;
                    _tempNextPosition._y = currentPos._y-1;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 0 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    _tempNextPosition._x = currentPos._x;
                    _tempNextPosition._y = currentPos._y+1;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                else if (getWorld()->getValue(eObstacles, test) == 1 && getWorld()->getValue(eObstacles, test2) == 1)
                    {
                    int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
                    if (flip == 0) {_tempNextPosition._x = currentPos._y+1;}
                    else if (flip == 1) {_tempNextPosition._x = currentPos._y-1;}                    
                    _tempNextPosition._y = currentPos._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = false;}
                    else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                    }
                }                
            }
        }
    else if (_justEvaded == false)
        {
            currentPos = getPosition();
	    int nearwall = 0;
	    int doorfound = 0;
	    int xcor = 0;
	    int ycor = 0;
	    for (index._x = currentPos._x - 1; index._x <= currentPos._x + 1; index._x++) 
		{
		for (index._y = currentPos._y - 1; index._y <= currentPos._y + 1; index._y++)
		    {
		    if (getWorld()->getValue(eObstacles, index) == 1) 
		        {
		        nearwall++;
		        xcor = xcor + currentPos._x - index._x;
		        ycor = ycor + currentPos._y - index._y;
		        }
		    else if ((getWorld()->getValue(eDoors, index) == 1)) 
		        {
		        // A LOT OF CODE CHECKING IF ITS THE EXIT DOOR REALLY.
		                    if (getWorld()->getValue(eDoorDir, index) == 4) //L to R
		                        {
		                        Engine::Point2D<int> check;
		                        check._x = index._x - 1;
		                        check._y = index._y;
		                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && (getWorld()->getValue(eDoorValue, index) == 1)) 
		                            {
		                            doorfound++;
		                            _tempNextPosition = index;
		                            _directionDarkness = 0;
		                            }
		                        else
		                            {
		                            nearwall++;
		                            xcor = xcor + currentPos._x - index._x;
		                            ycor = ycor + currentPos._y - index._y;                                    
		                            }
		                        }
		                    else if (getWorld()->getValue(eDoorDir, index) == 1) //D to U
		                        {
		                        Engine::Point2D<int> check;
		                        check._x = index._x;
		                        check._y = index._y + 1;
		                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && (getWorld()->getValue(eDoorValue, index) == 1))
		                            {
		                            doorfound++;
		                            _tempNextPosition = index;
		                            _directionDarkness = 0;
		                            }
		                        else
		                            {
		                            nearwall++;
		                            xcor = xcor + currentPos._x - index._x;
		                            ycor = ycor + currentPos._y - index._y;                                    
		                            }
		                        }
		                    else if (getWorld()->getValue(eDoorDir, index) == 2) // R to L
		                        {
		                        Engine::Point2D<int> check;
		                        check._x = index._x + 1;
		                        check._y = index._y;
		                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && (getWorld()->getValue(eDoorValue, index) == 1) ) 
		                            {
		                            doorfound++;
		                            _tempNextPosition = index;
		                            _directionDarkness = 0;
		                            }
		                        else
		                            {
		                            nearwall++;
		                            xcor = xcor + currentPos._x - index._x;
		                            ycor = ycor + currentPos._y - index._y;                                    
		                            }

		                        }
		                    else if (getWorld()->getValue(eDoorDir, index) == 3) // U TO D
		                        {
		                        Engine::Point2D<int> check;
		                        check._x = index._x;
		                        check._y = index._y - 1;
		                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check)  && (getWorld()->getValue(eDoorValue, index) == 1) )
		                            {
		                            doorfound++;
		                            _tempNextPosition = index;
		                            _directionDarkness = 0;
		                            }
		                        else
		                            {
		                            nearwall++;
		                            xcor = xcor + currentPos._x - index._x;
		                            ycor = ycor + currentPos._y - index._y;                                    
		                            }
		                        }                
		        }
		    }
		}
            if (nearwall > 0)
                {
                if (_touchedWall == false) 
                    {
                    _touchedWall = true;
                    _directionDarkness = 0;
                    }
                }
	// direction: 1 is UP , 2 L , 3 D 4 R
	    if (nearwall == 3 && doorfound == 0)
		{
		if (_directionDarkness == 0)
		    {
		    if (xcor > 0 && ycor == 0)
		        {
		        // direction either 1 or 3
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor < 0 && ycor == 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == 0 && ycor > 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == 0 && ycor < 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    }
		else if (_directionDarkness == 1)
		    {
		    _tempNextPosition._x = currentPos._x;
		    _tempNextPosition._y = currentPos._y - 1;
		    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) 
		        {
		        if (xcor > 0){_tempNextPosition._x = _tempNextPosition._x +1;}
		        else if (xcor < 0){_tempNextPosition._x = _tempNextPosition._x-1;}
		        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0){ _justEvaded = true;}
		        else if (getWorld()->getValue(eOccupied, _tempNextPosition) != 0) {_tempNextPosition = getPosition();}
		        }
		    }
		else if (_directionDarkness == 2)
		    {
		    _tempNextPosition._x = currentPos._x - 1;
		    _tempNextPosition._y = currentPos._y;
		    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) 
		        {
		        if (ycor > 0){_tempNextPosition._y = _tempNextPosition._y +1;}
		        else if (ycor < 0){_tempNextPosition._y = _tempNextPosition._y-1;}
		        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0){ _justEvaded = true;}
		        else if (getWorld()->getValue(eOccupied, _tempNextPosition) != 0) {_tempNextPosition = getPosition();}
		        }
		    }
		else if (_directionDarkness == 3)
		    {
		    _tempNextPosition._x = currentPos._x;
		    _tempNextPosition._y = currentPos._y + 1;
		    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) 
		        {
		        if (xcor > 0){_tempNextPosition._x = _tempNextPosition._x +1;}
		        else if (xcor < 0){_tempNextPosition._x = _tempNextPosition._x-1;}
		        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0){ _justEvaded = true;}
		        else if (getWorld()->getValue(eOccupied, _tempNextPosition) != 0) {_tempNextPosition = getPosition();}
		        }
		    }
		else if (_directionDarkness == 4)
		    {
		    _tempNextPosition._x = currentPos._x + 1;
		    _tempNextPosition._y = currentPos._y;
		    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) 
		        {
		        if (ycor > 0){_tempNextPosition._y = _tempNextPosition._y+1;}
		        else if (ycor < 0){_tempNextPosition._y = _tempNextPosition._y-1;}
		        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0){ _justEvaded = true;}
		        else if (getWorld()->getValue(eOccupied, _tempNextPosition) != 0) {_tempNextPosition = getPosition();}
		        }
		    }
		}

	    else if (nearwall == 2 && doorfound == 0)
		{		    
		if (_directionDarkness == 0) 
		    {
		    if (xcor == 2 && ycor == 1)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == 1 && ycor == 2)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == 2 && ycor == -1)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == 1 && ycor == -2)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == -1 && ycor == 2)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == -1 && ycor == -2)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == -2 && ycor == 1)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor == -2 && ycor == -1)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    }
		    
		else if (_directionDarkness == 1)
		    {
		        if (xcor > 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y-1;
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}                               
                                }
		            }
		        else if (xcor < 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y-1;  
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x-1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}
                                }                      
		            }
		        else if (xcor > 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y-1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 2;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 2;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		        else if (xcor < 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y-1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 4;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x-1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 4;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		    }
		else if (_directionDarkness == 2)
		    {
		        if (xcor > 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y; 
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}                               
                                }
		            }
		        else if (xcor < 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y-1; 
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 1;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x-1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 1;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		        else if (xcor > 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y;
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;} 
                                }    
		            }
		        else if (xcor < 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y+1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 3;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x-1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 3;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		    }
		else if (_directionDarkness == 3)
		    {
		        if (xcor > 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y+1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 2;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 2;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		        else if (xcor < 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y+1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 4;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x-1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 4;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		        else if (xcor > 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y+1; 
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}                               
                                }
		            }
		        else if (xcor < 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y+1;
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x-1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}                               
                                }
		            }
		    }
		else if (_directionDarkness == 4)
		    {
		        if (xcor > 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y-1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 1;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 1;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		        else if (xcor < 0 && ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y; // from here
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y+1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}                               
                                } 
		            }
		        else if (xcor > 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y+1;
		            if(getWorld()->getValue (eOccupied, _tempNextPosition) == 0){_directionDarkness = 3;}
                            else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
                                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 0) {_directionDarkness = 3;}
                                else if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1) {_tempNextPosition = currentPos;}
                                }
		            }
		        else if (xcor < 0 && ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y;
                            if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1)
                                {
		                _tempNextPosition._x = currentPos._x+1;
		                _tempNextPosition._y = currentPos._y-1;
                                if (getWorld()->getValue (eOccupied, _tempNextPosition) == 1){_tempNextPosition = currentPos;}                               
                                }

		            }
		    }

		
		}  

	    else if (nearwall == 5 && doorfound == 0)
		{        
		if (_directionDarkness == 0)
		    {
		    if (xcor > 0 && ycor > 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 4;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor > 0 && ycor < 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 4;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor < 0 && ycor > 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 3;}
                        _tempNextPosition = currentPos;
		        }
		    else if (xcor < 0 && ycor < 0)
		        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 2;}
		        else {_directionDarkness = 1;}
                        _tempNextPosition = currentPos;
		        }
		    }
		else if (_directionDarkness == 1)
		    {
		        if (xcor > 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y;
		                _directionDarkness = 4;
		                
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._y = currentPos._y +1;
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		        else if (xcor < 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y;
		                _directionDarkness = 2;
		                
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._y = currentPos._y+1;
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		    }
		else if (_directionDarkness == 2) 
		    {
		        if (ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y + 1;
		                _directionDarkness = 3;
		                
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._x = currentPos._x +1; 
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		        else if (ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y - 1;
		                _directionDarkness = 1;
		              
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._x = currentPos._x +1; 
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		    }
		else if (_directionDarkness == 3)
		    {
		        if (xcor > 0)
		            {
		            _tempNextPosition._x = currentPos._x+1;
		            _tempNextPosition._y = currentPos._y;
		                _directionDarkness = 4;
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._y = currentPos._y-1;
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		        else if (xcor < 0)
		            {
		            _tempNextPosition._x = currentPos._x-1;
		            _tempNextPosition._y = currentPos._y;
		                _directionDarkness = 2;
		               
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._y = currentPos._y +1;
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		    }
		else if (_directionDarkness == 4)
		    {
		        if (ycor > 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y + 1;
		                _directionDarkness = 3;
		             
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._x = currentPos._x-1;
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		        else if (ycor < 0)
		            {
		            _tempNextPosition._x = currentPos._x;
		            _tempNextPosition._y = currentPos._y - 1; 
		                _directionDarkness = 1;
		               
		            if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1)
		                {
		                _tempNextPosition._x = currentPos._x-1;
		                if (getWorld()->getValue(eOccupied, _tempNextPosition) == 0) {_justEvaded = true;}
		                else if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
		                }
		            }
		    }
		  
		}
            else if (nearwall == 1 && doorfound == 0)
                {
                if (_directionDarkness==0)
                    {
                    if (xcor > 0 && ycor >0)
                        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 2;}
                        _tempNextPosition = currentPos;                    
                        }
                    
                    else if (xcor > 0 && ycor < 0)
                        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 3;}
		        else {_directionDarkness = 2;}   
                        _tempNextPosition = currentPos;                         
                        }
                    else if (xcor < 0 && ycor > 0)
                        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 1;}
		        else {_directionDarkness = 4;} 
                        _tempNextPosition = currentPos;                    
                        }
                    else if (xcor < 0 && ycor < 0)
                        {
		        int flip = Engine::GeneralState::statistics().getUniformDistValue(0,1);
		        if (flip == 0) {_directionDarkness = 4;}
		        else {_directionDarkness = 3;}   
                        _tempNextPosition = currentPos;                
                        }
                    }
                else if (_directionDarkness!=0)
                    {
                    if (_directionDarkness==1)
                        {
                        _tempNextPosition._x = currentPos._x;
                        _tempNextPosition._y = currentPos._y - 1;
                        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}   
                        }
                    else if (_directionDarkness==2)
                        {
                        _tempNextPosition._x = currentPos._x - 1;
                        _tempNextPosition._y = currentPos._y;
                        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
                        }
                    else if (_directionDarkness==3)
                        {
                        _tempNextPosition._x = currentPos._x;
                        _tempNextPosition._y = currentPos._y + 1;
                        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();}
                        }
                    else if (_directionDarkness==4)
                        {
                        _tempNextPosition._x = currentPos._x - 1;
                        _tempNextPosition._y = currentPos._y;
                        if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1) {_tempNextPosition = getPosition();} 
                        }
                    }
                }
	    else if (nearwall == 0 && doorfound == 0 && _directionDarkness==0) // just select direction firstly
		{              
                _directionDarkness = Engine::GeneralState::statistics().getUniformDistValue(1 , 4);        
		}
            else if (nearwall == 0 && doorfound == 0 && _directionDarkness!=0)
                {
                if (_directionDarkness==1)
                    {
                    _tempNextPosition._x = getPosition()._x;
                    _tempNextPosition._y = getPosition()._y - 1;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1){_tempNextPosition = getPosition();}
                    }
                else if (_directionDarkness==2)
                    {
                    _tempNextPosition._x = getPosition()._x - 1;
                    _tempNextPosition._y = getPosition()._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1){_tempNextPosition = getPosition();}
                    }
                else if (_directionDarkness==3)
                    {
                    _tempNextPosition._x = getPosition()._x;
                    _tempNextPosition._y = getPosition()._y + 1;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1){_tempNextPosition = getPosition();}
                    }
                else if (_directionDarkness==4)
                    {
                    _tempNextPosition._x = getPosition()._x + 1;
                    _tempNextPosition._y = getPosition()._y;
                    if (getWorld()->getValue(eOccupied, _tempNextPosition) == 1){_tempNextPosition = getPosition();}
                    }
                }
        }
}


void EvacAgent::Delayed()
{
    //std::cout << "DELAY" <<std::endl;
    _tempNextPosition = getPosition();
    _startDelay--;

}

void EvacAgent::TryToGetUp()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
    Engine::Point2D<int> index, currentPos;
    currentPos = getPosition();
    int neighbourcount = 0;
    int panickedcount = 0;
    if(_getuptime > 0)
        {
	    for (index._x = currentPos._x - 1; index._x <= currentPos._x + 1; index._x++) 
		{
		for (index._y = currentPos._y - 1; index._y <= currentPos._y + 1; index._y++)
		   {
		   if ((getWorld()->getValue(ePanic, index) == 0) && (getWorld()->getValue(eOccupied, index) == 1)) 
		       {
		       neighbourcount++;
		       }
		   else if ((getWorld()->getValue(ePanic, index) == 1) && (getWorld()->getValue(eOccupied, index) == 1))
		       {
		       if ((index._x == currentPos._x) && (index._y = currentPos._y)) {
                           neighbourcount = neighbourcount+ 0;
                           panickedcount = panickedcount + 0;
		           break;
		           } 
		       else if (!((index._x == currentPos._x) && (index._y == currentPos._y)))
		           {
                           panickedcount++;
		           neighbourcount++;
		           break;
		           }
		       } 
		   }
		}

        if (neighbourcount < 8 && panickedcount == 0)
            {
            getWorld()->setValue(eObstacles, getPosition(), 0);
            _trampled = false;
            _getuptime = evacConfig.getuptime;
            }
        else
            {
            _getuptime = _getuptime-1;
            }
        }
    else if(_getuptime == 0)
        {
        getWorld()->setValue(eDead, getPosition(), 1);
        getWorld()->removeAgent(this);
	Engine::World* world  = getWorld();
	EvacWorld & evacWorld = (EvacWorld&) *world;
	auto al = evacWorld.returnAllList();
        evacWorld.removeRemovedAgent(this);
        }
}


void EvacAgent::UpdateeTempCells()
{
    if (_exited == false){getWorld()->setValue(eTempCells, _tempNextPosition, getWorld()->getValue(eTempCells, (_tempNextPosition))+1);}    
}

void EvacAgent::CheckTrampled()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
    Engine::Point2D<int> index, currentPos;
    currentPos = getPosition();
    int controlMass = 0;
    int neighbourcount = 0;

    for (index._x = currentPos._x - 1; index._x <= currentPos._x + 1; index._x++) 
        {
        for (index._y = currentPos._y - 1; index._y <= currentPos._y + 1; index._y++)
           {
           if ((getWorld()->getValue(ePanic, index) == 0) && (getWorld()->getValue(eOccupied, index) == 1)) 
               {
               neighbourcount++;
               }
           else if ((getWorld()->getValue(ePanic, index) == 1) && (getWorld()->getValue(eOccupied, index) == 1))
               {
               if ((index._x == currentPos._x) && (index._y = currentPos._y)) {
                   controlMass= controlMass + 0;
                   break;
                   } 
               else if (!((index._x == currentPos._x) && (index._y == currentPos._y)))
                   {
                   controlMass= controlMass + getWorld()->getValue(eMass, index);
                   neighbourcount++;
                   break;
                   }
               } 
           }
        }

    if ( _age == 0 && neighbourcount >= 4)
        {
        if (((float)controlMass/((float)returnMass()*100))> evacConfig.childTrampling) 
            {
            _trampled = true;
            getWorld()->setValue(eObstacles, getPosition(), 1);
            _tempNextPosition = currentPos;
            }
        }
    else if (_age == 1 && neighbourcount >= 4 )
        {
        if (_gender == 'M')
            {
            if (((float)controlMass/((float)returnMass()*100)) > evacConfig.manTrampling) 
                {
                _trampled = true;
                getWorld()->setValue(eObstacles, getPosition(), 1);
                _tempNextPosition = currentPos;
                }
            }
        else if (_gender == 'F')
            {
            if (((float)controlMass/((float)returnMass()*100)) > evacConfig.womanTrampling) 
                {
                _trampled = true;
                getWorld()->setValue(eObstacles, getPosition(), 1);
                _tempNextPosition = currentPos;
                }
            }      
        }
    else if (_age == 2 && neighbourcount>=4)
        {
        if (((float)controlMass/((float)returnMass()*100)) > evacConfig.elderlyTrampling) 
            {
            _trampled = true;
            getWorld()->setValue(eObstacles, getPosition(), 1);
            _tempNextPosition = currentPos;
            }
        }

    controlMass = 0;
}
void EvacAgent::SetTempNextPosition()
{
    //std::cout << "SETTEMPNEXT" <<std::endl;
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();

    if ((getWorld()->getValue(eRoomOrCoridor, getPosition())!=0) && (getWorld()->getValue(eStairs, getPosition())==0) ) // IN ONE OF THE FLOOR ROOMS - speed as _currentSpeed
        {
    	Engine::Point2D<int> index, currentPos;
    	currentPos = getPosition();
    	int neighbourcount = 0;

    	for (index._x = currentPos._x - 1; index._x <= currentPos._x + 1; index._x++) 
            {
            for (index._y = currentPos._y - 1; index._y <= currentPos._y + 1; index._y++)
               {
               if (getWorld()->getValue(eOccupied, index) == 1) {neighbourcount++;}
               }
            }
        if (neighbourcount > 4 && neighbourcount < 7)
            {
            if (returnSpeed() == 3 || returnSpeed() == 2) {_currentSpeed == returnSpeed()-1;}
            }
        else if (neighbourcount > 6){_currentSpeed = 1;}
        else if (neighbourcount < 5){_currentSpeed = returnSpeed();}
      
        if (_knowledge == 0)
            {
            // USE std::cout << "if knowledge == 0" << std::endl; 
            int seesigns = 0;
            int seedoors = 0;
            Engine::Point2D<int> index, currentPos;
            currentPos = getPosition();
            for (index._x = currentPos._x - _vision; index._x <= currentPos._x + _vision; index._x++) 
                {
                for (index._y = currentPos._y - _vision; index._y <= currentPos._y + _vision; index._y++)
                    {
                    if ( (index._x<0) || (index._x > getWorld()->getBoundaries()._size._width-1 ) || (index._y<0) || (index._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, index) != getWorld()->getValue(eFloor, currentPos))){continue;}
                    else if ( (getWorld()->getValue(eSigns, index) == 1) && (getWorld()->getValue(eRoomOrCoridor, index) == getWorld()->getValue(eRoomOrCoridor, currentPos)) ) {seesigns += 1;}
                    else if (getWorld()->getValue(eDoors, index) == 1) 
                        {
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
                float distanceToDoor = std::numeric_limits<int>::max();
                EvacConfig::DoorList::const_iterator itdoor1=evacConfig.doorList.begin();
                while(itdoor1!=evacConfig.doorList.end())
                    {
                                const Engine::Point2D<int> & door = *itdoor1;
                                if (getWorld()->getValue(eDoorValue, door) == 1)
                                    {
                                    if (getWorld()->getValue(eDoorDir, door) == 4) //L to R
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x - 1;
                                        check._y = door._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._y - evacConfig.maxDoorWidth; j <= door._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = door._x;
                                                point._y = j;
                                                if ((j!= door._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, door) == 1) //D to U
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x;
                                        check._y = door._y + 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._x - evacConfig.maxDoorWidth; j <= door._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = door._y;
                                                if ((j!= door._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                         
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, door) == 2) // R to L
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x + 1;
                                        check._y = door._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._y - evacConfig.maxDoorWidth; j <= door._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = door._x;
                                                point._y = j;
                                                if ((j!= door._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, door) == 3) // U TO D
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x;
                                        check._y = door._y - 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._x - evacConfig.maxDoorWidth; j <= door._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = door._y;
                                                if ((j!= door._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                             
                                            }
                                        }
                                    }
                                
                            
                        
                     itdoor1++;
                     }
                 currentPos = getPosition();
                 _tempNextPosition = currentPos;
                 }
            else if ((seedoors == 0) && (seesigns == 0))
                { 
                Engine::Point2D<int> radius;
                int agentsAroundHelp = 0;
                for (radius._x = getPosition()._x - evacConfig.talkingRadius; radius._x <= getPosition()._x + evacConfig.talkingRadius; radius._x++)
                    {
                    for (radius._y = getPosition()._y - evacConfig.talkingRadius; radius._y <= getPosition()._y + evacConfig.talkingRadius; radius._y++)
                        {
                        if ( (radius._x < 0) || (radius._y < 0) || (radius._x > getWorld()->getBoundaries()._size._width-1) || (radius._y > getWorld()->getBoundaries()._size._height-1) || ((radius._x == getPosition()._x) && (radius._y == getPosition()._y)) || (getWorld()->getValue(eOccupied, radius) == 0) || agentsAroundHelp > 0 ) {continue;}
                        else 
                            {
                            if ((getWorld()->getValue(eKnowledge, radius) == 1) && (getWorld()->getValue(eRoomOrCoridor, radius) == getWorld()->getValue(eRoomOrCoridor, getPosition())))
                                {
                                int talkOrNot = Engine::GeneralState::statistics().getUniformDistValue(1, 100);
                                if (talkOrNot <= evacConfig.talkingProb) 
                                    {
                                    agentsAroundHelp = agentsAroundHelp + 1;

                                    float distanceToDoor = std::numeric_limits<int>::max();
                                    EvacConfig::DoorList::const_iterator itdoor1=evacConfig.doorList.begin();
                            while(itdoor1!=evacConfig.doorList.end())
                                {
                                const Engine::Point2D<int> & door = *itdoor1;
                                if (getWorld()->getValue(eDoorValue, door) == 1)
                                    {
                                    if (getWorld()->getValue(eDoorDir, door) == 4) //L to R
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x - 1;
                                        check._y = door._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._y - evacConfig.maxDoorWidth; j <= door._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = door._x;
                                                point._y = j;
                                                if ((j!= door._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                                
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, door) == 1) //D to U
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x;
                                        check._y = door._y + 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._x - evacConfig.maxDoorWidth; j <= door._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = door._y;
                                                if ((j!= door._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, door) == 2) // R to L
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x + 1;
                                        check._y = door._y;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._y - evacConfig.maxDoorWidth; j <= door._y + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = door._x;
                                                point._y = j;
                                                if ((j!= door._y) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                            }
                                        }
                                    else if (getWorld()->getValue(eDoorDir, door) == 3) // U TO D
                                        {
                                        Engine::Point2D<int> check;
					float distance;
					int absx = abs(door._x - currentPos._x);
					int absy = abs(door._y - currentPos._y);
					if (absx > absy){distance = (absx + absy*(sqrt(2)-1));}
					else if (absy > absx){distance = (absy + absx*(sqrt(2)-1));}
					else if (absy == absx){distance = (absx + absy*(sqrt(2)-1));}
                                        check._x = door._x;
                                        check._y = door._y - 1;
                                        if (getWorld()->getValue(eRoomOrCoridor, currentPos) == getWorld()->getValue(eRoomOrCoridor, check) && distance < distanceToDoor) 
                                            {
                                            distanceToDoor = distance;
                                            this->_currGoal.clear();
                                            this->_currGoal.push_front(door);
                                            int j;
                                            for (j = door._x - evacConfig.maxDoorWidth; j <= door._x + evacConfig.maxDoorWidth; j++)
                                                {
                                                Engine::Point2D<int> point;
                                                point._x = j;
                                                point._y = door._y;
                                                if ((j!= door._x) && (getWorld()->getValue(eDoors, point) == 1) && (getWorld()->getValue(eDoorDir, door) == getWorld()->getValue(eDoorDir, point)) && (getWorld()->getValue(eDoorValue, door) == getWorld()->getValue(eDoorValue, point)) ) {this->_currGoal.push_front(point);} 
                                                }
                                            this->_knowledge = 1;
                                            }
                                        }
                                    }                                                     
                     itdoor1++;
                     }
                 currentPos = getPosition();
                 _tempNextPosition = currentPos;

   
                                    }
                                else {continue;}
                                }
                            }
                        }
                    }
                if (agentsAroundHelp == 0)
                    {
////////////////////////////////////////////////////////////////////////////////// CHEMO TAXI TRAILS STUFF  BEGIN

                    Engine::Point2D<int> radius, nbhood, currPos, check;
                    check = getPosition();
                    currPos = getPosition();
                    int taxisum = 0;
                    for (radius._x = currPos._x - _vision; radius._x <= currPos._x + _vision; radius._x++)
                        {
                        for (radius._y = currPos._y - _vision; radius._y <= currPos._y + _vision; radius._y++)
                            {
                            if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || ( (getWorld()->getValue(eFloor, radius) != getWorld()->getValue(eFloor, currPos)) && (getWorld()->getValue(eDoors, radius) == 0) ) || getWorld()->getValue(eRoomOrCoridor, radius) != getWorld()->getValue(eRoomOrCoridor, currPos)){continue;}
                            else if ( (getWorld()->getValue(eChemoTaxiTrails, radius) > 1) && (getWorld()->getValue(eChemoTaxiTrails, radius) > getWorld()->getValue(eChemoTaxiTrails, check)) )
                                {
                                int count = 0;
                                int nbhoodsum = 0;
                                for (nbhood._x = radius._x - 1; nbhood._x <= radius._x + 1; nbhood._x ++)
                                    {
                                    for (nbhood._y = radius._y - 1; nbhood._y <= radius._y + 1; nbhood._y++)
                                        {
                                        if ( (nbhood._x<0) || (nbhood._x > getWorld()->getBoundaries()._size._width-1 ) || (nbhood._y<0) || (nbhood._y > getWorld()->getBoundaries()._size._height-1 ) || ( (getWorld()->getValue(eFloor, nbhood) != getWorld()->getValue(eFloor, currPos)) && (getWorld()->getValue(eDoors, nbhood) == 0) ) || getWorld()->getValue(eRoomOrCoridor, nbhood) != getWorld()->getValue(eRoomOrCoridor, currPos) ){continue;}
                                        else if (getWorld()->getValue(eChemoTaxiTrails, nbhood) >= getWorld()->getValue(eChemoTaxiTrails, radius) )
                                            {
                                            count++;
                                            nbhoodsum += getWorld()->getValue(eChemoTaxiTrails, nbhood);
                                            }
                                        }
                                    }
                                if (count >= 3 && nbhoodsum > taxisum)
                                    {
                                    check = radius;
                                    taxisum = nbhoodsum;
                                    }
                                }
                            }
                        }                    
                    if (check != getPosition())
                        {
                        _tempNextPosition = getPosition();
                        Engine::Point2D<int> step;
                        float distance = sqrt(pow((check._x - currPos._x),2) + pow((check._y - currPos._y),2));
                        for (step._x = currentPos._x - _currentSpeed; step._x <= currentPos._x + _currentSpeed; step._x++)
                            {
                            for (step._y = currentPos._y - _currentSpeed; step._y <= currentPos._y + _currentSpeed; step._y++)
                                {
                                if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos)) || getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, currPos) ){continue;} //PROTECT FROM OUT OF BOUNDARIES
                                else if (sqrt(pow((check._x - step._x),2) + pow((check._y - step._y),2)) < distance )
                                    {
                                    if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) &&  (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) || (getWorld()->getValue(eOccupied, step) == 1) ) {continue;}         
                                    _tempNextPosition = step;
                                    distance = sqrt(pow((check._x - step._x),2) + pow((check._y - step._y),2));
                                    }
                                }
                            }
                        }

                    else if (check == getPosition())
                        {// new bracket - 
/////////////////////////////////////////////////////////////////////////////////////////////HERE WHERE TAXI TRAIL STUFF ENDSSS
// PART WITH THE RANDOM WALK IF CHEMOTAXITRAILS DOESN'T WORK - OLD PART
                      
		        int freespace = 0;
		        Engine::Point2D<int> step2;
		        for (step2._x = getPosition()._x - _currentSpeed; step2._x <= getPosition()._x + _currentSpeed; step2._x++)
		            {
		            for (step2._y = getPosition()._y - _currentSpeed; step2._y <= getPosition()._y + _currentSpeed; step2._y++)
		                {
		                if (getWorld()->getValue(eObstacles, step2) == 1) {continue;}
		                else if ( (getWorld()->getValue(eObstacles, step2) == 0) && (getWorld()->getValue(eOccupied, step2) == 0) && (getWorld()->getValue(eRoomOrCoridor, step2) == getWorld()->getValue(eRoomOrCoridor, getPosition()) ) ) {freespace++;}
		                }
		            }

		        if (freespace == 0) 
		            {
		            _tempNextPosition._x = getPosition()._x;
		            _tempNextPosition._y = getPosition()._y;
		            }
		        else if (freespace > 0) 
		        {
		        _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
		        _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
		        while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || ( getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) || (getWorld()->getValue(eOccupied, _tempNextPosition) == 1))
		            {    
		            _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
		            _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
		            }
                        }
                    } // new bracket

                    }// IF AROUNDHELP == 0

                } //sd & ss == 0
            } //knowledge == 0

        else if (_knowledge == 1)
            {
                 Engine::Point2D<int> step, currentPos;
                 currentPos = getPosition();
                 _tempNextPosition = currentPos;
                 currGoalsList::const_iterator itcurr=_currGoal.begin();
                 int rasterName = getWorld()->getValue(eDoorNumber, _currGoal.front());
                 std::ostringstream rasterStr;
	         rasterStr << "d" <<rasterName;                 
                 float distance = getWorld()->getValue(rasterStr.str(), getPosition());
            
	         while(itcurr!=_currGoal.end())
	             {
		     const Engine::Point2D<int> & ext = *itcurr;
                     int doorNum = getWorld()->getValue(eDoorNumber, ext); 
                     std::ostringstream doorRaster;
                     doorRaster << "d" << doorNum;
                     for (step._x = currentPos._x - _currentSpeed; step._x <= currentPos._x + _currentSpeed; step._x++)
                         {
                         for (step._y = currentPos._y - _currentSpeed; step._y <= currentPos._y + _currentSpeed; step._y++)
                             {
                             if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eFloor, step) != getWorld()->getValue(eFloor, currentPos))){continue;} //PROTECT FROM OUT OF BOUNDARIES
                             else if (getWorld()->getValue(doorRaster.str(), step) <= distance )
                                 {
                                 if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) != getWorld()->getValue(eRoomOrCoridor, getPosition()) ) &&  (getWorld()->getValue(eRoomOrCoridor, step) != 0) ) || (getWorld()->getValue(eOccupied, step) == 1) || ( (getWorld()->getValue(eRoomOrCoridor, step) == 0) && (getWorld()->getValue(eDoorNumber, step) != getWorld()->getValue(eDoorNumber, ext)) ) ) {continue;} // here we define that the agent cant jump between rooms
                                 _tempNextPosition = step;
                                 distance = getWorld()->getValue(doorRaster.str(), step);
                                 }
                             }
                         }
                     itcurr++; 
                     }
            }
        } // END OF FIRST GENERAL CONDITION

//////////////////// LETS CHANGED THE WAY THAT THE AGENTS MOVE WHEN THEY EXIT FROM A DOOR TO SOME ROOM. LET IT BE THAT THEY RANDOMLY WALK SOMEWHEE OUTSIDE THE DOOR IN THE NEXT ROOM, AND ON THE NEXT STEP THEY EXECUTE LOCATE DOORS SIGNS ETC.

    else if ((getWorld()->getValue(eRoomOrCoridor, getPosition())==0) && (getWorld()->getValue(eStairs, getPosition())!=2) && (getWorld()->getValue(eDoors, getPosition()) == 1) ) // IN A REGULAR DOOR NOT LEADING TO STAIRS - Speed will stay as _speed in from the door.
        {
            _knowledge = 0;
            _currentSpeed = returnSpeed();
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
           
                int freespace = 0;
                Engine::Point2D<int> radius, step;
                for (step._x = getPosition()._x - _currentSpeed; step._x <= getPosition()._x + _currentSpeed; step._x++)
                    {
                    for (step._y = getPosition()._y - _currentSpeed; step._y <= getPosition()._y + _currentSpeed; step._y++)
                        {
                        if (getWorld()->getValue(eObstacles, step) == 1) {continue;}
                        else if ( (getWorld()->getValue(eObstacles, step) == 0) && (getWorld()->getValue(eOccupied, step) == 0) && (getWorld()->getValue(eRoomOrCoridor, step) == getWorld()->getValue(eRoomOrCoridor, compare) ) ) {freespace++;}
                        }
                    }

                if (freespace == 0) 
                    {
                    _tempNextPosition._x = getPosition()._x;
                    _tempNextPosition._y = getPosition()._y;
                    }
                else if (freespace > 0) 
                {
                _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
                _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
                while ((_tempNextPosition._x < 0) || (_tempNextPosition._x > getWorld()->getBoundaries()._size._width-1) || (_tempNextPosition._y < 0) || (_tempNextPosition._y > getWorld()->getBoundaries()._size._height-1) || (getWorld()->getValue(eObstacles, _tempNextPosition) == 1) || ( getWorld()->getValue(eRoomOrCoridor, _tempNextPosition) != getWorld()->getValue(eRoomOrCoridor, compare) ) || (getWorld()->getValue(eOccupied, _tempNextPosition) == 1))
                    {    
                    _tempNextPosition._x = getPosition()._x + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
                    _tempNextPosition._y = getPosition()._y + Engine::GeneralState::statistics().getUniformDistValue((-1 * _currentSpeed), _currentSpeed);
                    }
                }  
        }

    else if (getWorld()->getValue(eStairs, getPosition())==2) // IN A DOOR LEADING TO STAIRS // speed will remain stairsSpeed in the door.
        {
        _knowledge = 0;
	_currentSpeed = returnStairsSpeed();
        Engine::Point2D<int> radius, compare, currentPos, around, step;
        int turnDoorFound = 0;
        int i = 1;
        currentPos = getPosition();
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
                    if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eStairs, radius))!=3)
                        {continue;}
                    else if ((getWorld()->getValue(eStairs, radius) == 3))
                        {
                        this->_currGoal.clear();
                        this->_currGoal.push_front(radius);
                        for (around._x = radius._x - evacConfig.maxDoorWidth; around._x <= radius._x + evacConfig.maxDoorWidth; around._x++)
                            {
                            for (around._y = radius._y - evacConfig.maxDoorWidth; around._y <= radius._y + evacConfig.maxDoorWidth; around._y++)
                                {
                                if ( (getWorld()->getValue(eStairs, around) == 3) && ((around._x != radius._x) && (around._y != radius._y)) ) {this->_currGoal.push_front(around);}
                                }
                            }
                        turnDoorFound = turnDoorFound +1;
                        }                  
                    }
                }
            i++;
            }
        

        i = 1;
        _tempNextPosition = getPosition();
        currentPos = getPosition();  
        int rasterName = getWorld()->getValue(eTurnNumber, _currGoal.front());
        std::ostringstream rasterStr;
	rasterStr << "t" << rasterName;
                 
        float distance = getWorld()->getValue(rasterStr.str(), getPosition());
        currGoalsList::const_iterator itcurr=_currGoal.begin();
         
        while(itcurr!=_currGoal.end())
            {
	    const Engine::Point2D<int> & ext = *itcurr;

            int doorNum = getWorld()->getValue(eTurnNumber, ext);
            std::ostringstream doorRaster;
            doorRaster << "t" << doorNum;

            for (step._x = currentPos._x - _currentSpeed; step._x <= currentPos._x + _currentSpeed; step._x++)
                {
                for (step._y = currentPos._y - _currentSpeed; step._y <= currentPos._y + _currentSpeed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eStairs, step) == 0)){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if (getWorld()->getValue(doorRaster.str(), step) <= distance )
                        {
                        if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eStairs, step) != 1 ) && (getWorld()->getValue(eStairs, step) != 3 ) ) || (getWorld()->getValue(eOccupied, step) == 1) || (getWorld()->getValue(eStairsSide, step) != 1) ) {continue;} 
                        _tempNextPosition = step;
                        distance = getWorld()->getValue(doorRaster.str(), step);
                        }
                    }
                }
                itcurr++; 
            }
        } 


    else if (getWorld()->getValue(eStairs, getPosition())==1 ) // ON THE STAIRS - _currenSpeed will be implemented
        {
        Engine::Point2D<int> index, radius, compare, currentPos, around, step;
        currentPos = getPosition();
        _tempNextPosition = currentPos;
    	int neighbourcount = 0;
    	for (index._x = currentPos._x - 1; index._x <= currentPos._x + 1; index._x++) 
            {
            for (index._y = currentPos._y - 1; index._y <= currentPos._y + 1; index._y++)
               {
               if (getWorld()->getValue(eOccupied, index) == 1) {neighbourcount++;}
               }
            }
        if (neighbourcount > 3 && returnStairsSpeed() == 2) {_currentSpeed = returnStairsSpeed()-1;}
        else if (neighbourcount < 4 || returnStairsSpeed() == 1 ){_currentSpeed = returnStairsSpeed();}

        float distance;
        int rasterName;
        if (getWorld()->getValue(eStairsSide, getPosition()) == 1)
            {
	    rasterName = getWorld()->getValue(eTurnNumber, _currGoal.front());
	    std::ostringstream rasterStr;
	    rasterStr << "t" << rasterName;                 
	    distance = getWorld()->getValue(rasterStr.str(), getPosition());
            }
        else if (getWorld()->getValue(eStairsSide, getPosition()) == 2)
            {
	    rasterName = getWorld()->getValue(eJumpNumber, _currGoal.front()); 
	    std::ostringstream rasterStr;
	    rasterStr << "j" << rasterName;                 
	    distance = getWorld()->getValue(rasterStr.str(), getPosition());
            }

        currGoalsList::const_iterator itcurr=_currGoal.begin();
         
        while(itcurr!=_currGoal.end())
            {
	    const Engine::Point2D<int> & ext = *itcurr;
            int doorNum;
            std::ostringstream doorRaster;
            if (getWorld()->getValue(eStairsSide, getPosition()) == 1)
                {
                doorNum = getWorld()->getValue(eTurnNumber, ext);
                doorRaster << "t" << doorNum;
                }

            else if (getWorld()->getValue(eStairsSide, getPosition()) == 2)
                {
                doorNum = getWorld()->getValue(eJumpNumber, ext);
                doorRaster << "j" << doorNum;
                }

            for (step._x = currentPos._x - _currentSpeed; step._x <= currentPos._x + _currentSpeed; step._x++)
                {
                for (step._y = currentPos._y - _currentSpeed; step._y <= currentPos._y + _currentSpeed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eStairs, step) == 0)){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if (getWorld()->getValue(doorRaster.str(), step) <= distance )
                        {
                        if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eStairs, step) != 1 ) && (getWorld()->getValue(eStairs, step) != 3 ) && (getWorld()->getValue(eStairs, step) != 4) ) || (getWorld()->getValue(eOccupied, step) == 1) || ( (getWorld()->getValue(eStairsSide, step) !=0) && (getWorld()->getValue(eStairsSide, step) != getWorld()->getValue(eStairsSide, getPosition())) &&  (getWorld()->getValue(eStairs, step) != 3))  ) {continue;} 
                        _tempNextPosition = step;
                        distance = getWorld()->getValue(doorRaster.str(), step);
                        }
                    }
                }
                itcurr++; 
            }
        } 


    else if ((getWorld()->getValue(eStairs, getPosition())==3) ) // ON THE STAIRS TURN POINT
        {
        Engine::Point2D<int> radius, index, currentPos, around, step;
        int jumpDoorFound = 0;
        int i = 1;
        currentPos = getPosition();

    	int neighbourcount = 0;
    	for (index._x = currentPos._x - 1; index._x <= currentPos._x + 1; index._x++) 
            {
            for (index._y = currentPos._y - 1; index._y <= currentPos._y + 1; index._y++)
               {
               if (getWorld()->getValue(eOccupied, index) == 1) {neighbourcount++;}
               }
            }
        if (neighbourcount > 3 && returnStairsSpeed() == 2) {_currentSpeed = returnStairsSpeed()-1;}
        else if (neighbourcount < 4 || returnStairsSpeed() == 1 ){_currentSpeed = returnStairsSpeed();}
       
        while (jumpDoorFound == 0)
            {
            for (radius._x = currentPos._x - i; radius._x <= currentPos._x + i; radius._x++)
                {
                for (radius._y = currentPos._y - i; radius._y <= currentPos._y + i; radius._y++)
                    {
                    if ( (radius._x<0) || (radius._x > getWorld()->getBoundaries()._size._width-1 ) || (radius._y<0) || (radius._y > getWorld()->getBoundaries()._size._height-1 )){continue;}
                    else if ((getWorld()->getValue(eStairs, radius) == 4))
                        {
                        this->_currGoal.clear();
                        this->_currGoal.push_front(radius);
                        for (around._x = radius._x - evacConfig.maxDoorWidth; around._x <= radius._x + evacConfig.maxDoorWidth; around._x++)
                            {
                            for (around._y = radius._y - evacConfig.maxDoorWidth; around._y <= radius._y + evacConfig.maxDoorWidth; around._y++)
                                {
                                if ( (around._x<0) || (around._x > getWorld()->getBoundaries()._size._width-1 ) || (around._y<0) || (around._y > getWorld()->getBoundaries()._size._height-1 )){continue;}
                                else if ( (getWorld()->getValue(eStairs, around) == 4) && ((around._x != radius._x) && (around._y != radius._y)) ) {this->_currGoal.push_front(around);}
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
          
	int rasterName = getWorld()->getValue(eJumpNumber, _currGoal.front());
	std::ostringstream rasterStr;
	rasterStr << "j" << rasterName;                 
	float distance = getWorld()->getValue(rasterStr.str(), getPosition());

        currGoalsList::const_iterator itcurr=_currGoal.begin();
         
        while(itcurr!=_currGoal.end())
            {
	    const Engine::Point2D<int> & ext = *itcurr;

            int doorNum = getWorld()->getValue(eJumpNumber, ext);
            std::ostringstream doorRaster;
            doorRaster << "j" << doorNum;


            for (step._x = currentPos._x - _currentSpeed; step._x <= currentPos._x + _currentSpeed; step._x++)
                {
                for (step._y = currentPos._y - _currentSpeed; step._y <= currentPos._y + _currentSpeed; step._y++)
                    {
                    if ( (step._x<0) || (step._x > getWorld()->getBoundaries()._size._width-1 ) || (step._y<0) || (step._y > getWorld()->getBoundaries()._size._height-1 ) || (getWorld()->getValue(eStairs, step) == 0 )){continue;} //PROTECT FROM OUT OF BOUNDARIES
                    else if ( getWorld()->getValue(doorRaster.str(), step) <= distance )
                        {
                        if ( (getWorld()->getValue(eObstacles, step) == 1) || ( (getWorld()->getValue(eStairs, step) != 1 ) && (getWorld()->getValue(eStairs, step) != 4 ) ) || (getWorld()->getValue(eOccupied, step) == 1) || (getWorld()->getValue(eStairsSide, step) == 1)) {continue;} 
                        _tempNextPosition = step;
                        distance = getWorld()->getValue(doorRaster.str(), step);
                        }
                    }
                }
            itcurr++; 
            }
        }     

    else if (getWorld()->getValue(eStairs, getPosition())==4) // ON THE STAIRS JUMP POINT
        {
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
                break;
                }
            itjump++;
            itjumpto++;
            }
        }  
} // END OF SETTEMPNEXTPOSITION


void EvacAgent::selectActions()
{
    //std::cout << "SelectActions for agent" << getId() << std::endl;
    if ( (getWorld()->getValue(eDarkness, getPosition()) == 1) && _startDelay == 0 && !_trampled ) {DarknessActions();}
    else if (getWorld()->getValue(eDarkness, getPosition()) == 1 && _startDelay > 0 && !_trampled) {Delayed();}
    else 
        {    
        if(_trampled ){TryToGetUp();}
        if(!_trampled){CheckTrampled();} 
        if(!_trampled && _startDelay <= 0){SetTempNextPosition();}
        else if (!_trampled && _startDelay > 0){Delayed();}
        }    
}


void EvacAgent::updateState()
{

       const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
       if (_exited == true)
           {
            return;
           }
        else if (getWorld()->getValue(eExits, getPosition()) == 1)
            {
        getWorld()->setValue(eExitedAgents, getPosition(),(getWorld()->getValue(eExitedAgents, getPosition())) +1 );
        _exited = true;
        getWorld()->removeAgent(this);
	Engine::World* world  = getWorld();
	EvacWorld & evacWorld = (EvacWorld&) *world;
	auto al = evacWorld.returnAllList();
        evacWorld.removeRemovedAgent(this);
        }
        if (_notMoved == evacConfig.returnPanicTresh()) 
            {
            _panicked +=1;
            _notMoved = 0;
            }

        UpdateeTempCells();
}

void EvacAgent::registerAttributes()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
        registerIntAttribute("panicked");
        registerIntAttribute("currentSpeed");
        registerIntAttribute("age");
        registerFloatAttribute("mass");
        registerIntAttribute("trampled");       
        registerIntAttribute("knowledge");
        registerIntAttribute("startDelay");
        registerIntAttribute("directionDarkness");
	registerIntAttribute("notMoved");
	registerIntAttribute("notMovedTotal");
        registerFloatAttribute("evacDist"); 
	registerIntAttribute("evacTime"); 
	registerIntAttribute("tempNextPosition_x"); 
	registerIntAttribute("tempNextPosition_y");
	registerIntAttribute("currGoal.begin()._x");
	registerIntAttribute("currGoal.begin()._y");
        //USE DEF std::cout<<"RegisterAttributes correctly"<<std::endl;          
}

void EvacAgent::serialize()
{
    const EvacConfig & evacConfig = (const EvacConfig &)getWorld()->getConfig();
        serializeAttribute("panicked", _panicked);
        serializeAttribute("currentSpeed", _currentSpeed);
        serializeAttribute("age", _age);
        serializeAttribute("mass", _mass);
        serializeAttribute("trampled", _trampled);
        serializeAttribute("knowledge", _knowledge);
        serializeAttribute("startDelay", _startDelay);
        serializeAttribute("directionDarkness", _directionDarkness);
        serializeAttribute("notMoved", _notMoved);
        serializeAttribute("notMovedTotal", _notMovedTotal);
        serializeAttribute("evacTime", _evacTime);
        serializeAttribute("evacDist", _evacDist);
        serializeAttribute("tempNextPosition_x", _tempNextPosition._x);      
        serializeAttribute("tempNextPosition_y", _tempNextPosition._y);
        serializeAttribute("currGoal.begin()._x", _currGoal.front()._x);   
        serializeAttribute("currGoal.begin()._y", _currGoal.front()._y);     
        //USE DEF std::cout<<"Serialize correctly"<<std::endl;  
}
	
std::string EvacAgent::agentCharac()
{
	std::ostringstream charac;
	charac << getId() <<" is a "<<this->_gender<<" of "<<this->_age<<" yo with v:"<<this->_vision<<" and s:"<<this->_speed<<" and position"<<this->getPosition();
	return charac.str();
    
}


} // namespace Evacuation


// rabndom number seed 

