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
        eStairs,
    
        //chemotaxi-trails
        eChemoTaxiTrails,

        //signs for evacuation
        eSigns,

        //doors locations, like temporary exit-goals
        eDoors,

        // room or corridor (room == 1, corridor == 0, door == 2)
        eRoomOrCoridor,
	
        // floor number for initial distribution
        eFloor,

	// number of agents in each cell
	eNumAgents

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
        void UpdateTempValues();

 

public:
	EvacWorld( EvacConfig * config, Engine::Scheduler * scheduler = 0);
	virtual ~EvacWorld();
// FROM ABOVE THESE 3 LINES HERE

        typedef std::list<EvacAgent*> AL;
        AL al;
        AL returnAllList() const{return al;}
        AL removeRemovedAgent (EvacAgent* agent) 
            {al.remove(agent);
             return al;
            }

        ExitsList returnList() const{return _exits;}
};

} // namespace Evacuation

#endif // __EvacWorld_hxx

