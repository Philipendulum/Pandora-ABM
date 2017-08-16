
#ifndef __EvacAgent_hxx__
#define __EvacAgent_hxx__

#include <Agent.hxx>
#include <Action.hxx>

#include <string>

namespace Evacuation
{


    class EvacConfig;
    class EvacAgent : public Engine::Agent
    {
	int _resources; 

	private:
	int _speed;
        int _stairsSpeed;
	Engine::Point2D<int> _exit; 
	int _floor; // which floor the agent is located
        int _time;
	int panictime; // time the agent was in panic above **SELECT THRESHOLD**
	bool _isOnStairs;
	int _age;
	char _gender;
        float _mass;

	int _vision;

	public:
        bool _exited;
        bool _trampled;
        int _getuptime;
        int _startDelay;
        bool _justEvaded;
	int returnSpeed(){return _speed;}
	int returnStairsSpeed(){return _stairsSpeed;}
        int _currentSpeed;
        int returnMass(){return _mass;}
	int _panicked; // what high panic leads to ??? FIGURE OUT !!!!!!!! It leads to HARDCORE FIGHT
	int _notMoved;// how many timesteps the agent has not moved, it goes to 0 when panic increases by 1 
        int _notMovedTotal; // TOTAL NOT MOVED FOR SIMULATION. is never reset to 0
	typedef std::list<Engine::Point2D<int> > currGoalsList; 
	currGoalsList _currGoal; // current goal , maybe door or exit
	Engine::Point2D<int> _tempNextPosition; // temporary next position, before competition for free cell has determined who moves. 
	void SetTempNextPosition();
        void CheckTrampled();
        void TryToGetUp();
        void Delayed();
        void DarknessActions();
        void UpdateeTempCells();
	int _knowledge;
	int _evacTime;
	float _evacDist;
        int _directionDarkness;
        bool _touchedWall;

          

	EvacAgent( const std::string & id, int speed, float mass, int stairsSpeed, int floor,  char gender, int age, int vision, bool isOnStairs, bool exited, bool trampled, int getuptime, int startDelay, bool justEvaded, int panicked, int directionDarkness, bool touchedWall, int evacDist, int evacTime, int notMoved, int notMovedTotal, int knowledge); //this will be the constructer we will use in createAgent

	virtual ~EvacAgent();

	typedef std::list<Engine::Point2D<int> > ExitsList;       

        void selectActions();
	void updateState();
	void registerAttributes();
	void serialize();


	std::string agentCharac();


	////////////////////////////////////////////////
	// This code has been automatically generated //
	/////// Please do not modify it ////////////////
	////////////////////////////////////////////////
	EvacAgent( void * );
	void * fillPackage();
	void sendVectorAttributes(int);
	void receiveVectorAttributes(int);
	////////////////////////////////////////////////
	//////// End of generated code /////////////////
	////////////////////////////////////////////////

    };

} // namespace Evacuation

#endif // __EvacAgent_hxx__


