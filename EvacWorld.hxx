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

        // temporary count of agents wanting to move to this cell. 
        eTempCells,

        //occupied cells at current step in time
        eOccupied,
        
        // stairs
        eStairs, // 0 for rooms and doors, 1 - stairs, 2 - stair door, 3 - for the stairs turning , 4 for position of jumping from stairs of one floor to another
    
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

        // room or corridor (any room > 0 door == 0) // SHOULD BE CHANGED TO 0 DOOR and so on...
        eRoomOrCoridor,
	
        // floor number for initial distribution
        eFloor,

	// number of agents in each cell
	//eNumAgents,
         
        // shows whether agent here has knowledge of the next exit or no 
        eKnowledge

	// eDeaths, - Does not seem required now.

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

 

public:
	EvacWorld( EvacConfig * config, Engine::Scheduler * scheduler = 0);
	virtual ~EvacWorld();
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

