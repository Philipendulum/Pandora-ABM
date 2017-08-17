#ifndef __EvacWorld_hxx
#define __EvacWorld_hxx

#include <World.hxx>
#include <GeneralState.hxx>

namespace Evacuation 
{
class EvacAgent;
class EvacConfig;

enum Rasters
{
	// obstacle, 1 if true, 0 if false
	eObstacles,

	// exits, 1 if true, 0 if false
	eExits,

        //exited agents count
        eExitedAgents,

        // temporary count of agents wanting to move to this cell. 
        eTempCells,

        //occupied cells at current step in time
        eOccupied,
        
        // stairs
        eStairs, // 0 for rooms and doors, 1 - stairs, 2 - stair door, 3 - for the stairs turning , 4 for position of jumping from stairs of one floor to another

        // stairs side 1 for left 2 for right
        eStairsSide,
    
        //chemotaxi-trails
        eChemoTaxiTrails,

        //signs for evacuation
        eSigns,

        //doors locations, like temporary exit-goals
        eDoors,

        // sets up if door is valuable for exit strategy or not (1 valuable, 0 not)
        eDoorValue,

        // sets up the door passing direction for exit strategy if door value is 1 (D to U is 1 , R to L is 2, U to D is 3,  L to R is 4 )
        eDoorDir,

        // door number in range of doors
        eDoorNumber,

        // turn number in range of turn points
        eTurnNumber,

        // jump number in range of jump points
        eJumpNumber,

        // room or corridor (any room > 0 door == 0) // SHOULD BE CHANGED TO 0 DOOR and so on...
        eRoomOrCoridor,
	
        // floor number for initial distribution
        eFloor,
         
        // shows whether agent here has knowledge of the next exit or no 
        eKnowledge,
       
        eMass,

        ePanic,

        eDead,

        eDarkness,

};

class EvacWorld: public Engine::World
{

	typedef std::list<Engine::Point2D<int> > ExitsList;
	ExitsList _exits;
	void createAgents();
	void createRasters();
         
        void NextPosition();
	void fillExitList();
	void stepEnvironment();
        void ResolveCompetition();
        void ResetExits();
        void ResetOccupied();
        void ResetKnowledge();
        void UpdateTempValues();
        void ResetPanic();
        void ResetMass();
        void ChemoDecay();
        int DecayStatus;

 

public:
	EvacWorld( EvacConfig * config, Engine::Scheduler * scheduler = 0);
	virtual ~EvacWorld();
        void run();
        typedef std::list<EvacAgent*> AL;
        AL al; // listof all agents
        AL returnAllList() const{return al;}
        AL removeRemovedAgent (EvacAgent* agent) 
            {al.remove(agent);
             return al;
            }

        ExitsList returnList() const{return _exits;}
};

} // namespace Evacuation

#endif // __EvacWorld_hxx

