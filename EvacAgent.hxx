
#ifndef __EvacAgent_hxx__
#define __EvacAgent_hxx__

#include <Agent.hxx>
#include <Action.hxx>

#include <string>

namespace Evacuation
{

    class EvacConfig;
    // TEMPORARILY DISABLE ENUMS
    /*enum Gender
      {
      Male;
      Female;
      };
      enum Age
      {
      Child;
      Adult;
      Elderly;
      };*/

    class EvacAgent : public Engine::Agent
    {
	int _resources; 

	private:
	int _speed;
	Engine::Point2D<int> _exit; 
	int _floor; // which floor the agent is located
	int _time; // time it takes to evacuate
	int panictime; // time the agent was in panic above **SELECT THRESHOLD**
	bool isOnStairs;
	bool exited;
	//Evacuation::Gender gender;
	//Evacuation::Age age;
	int _age;
	char _gender;

	int _vision;
	int _knowledge;
	int _evacTime;
	float _evacDist;
	int panicked; // what high panic leads to ??? FIGURE OUT !!!!!!!! It leads to HARDCORE FIGHT
	int notMoved;// how many timesteps the agent has not moved at all
	Engine::Point2D<int> currGoal; // current goal , maybe door or exit
	Engine::Point2D<int> tempNextPosition; // temporary next position, before competition for free cell has determined who moves. 
	void NextPosition();
	void SetTempNextPosition();



	public:
	

	// todo remove environment from here
	EvacAgent( const std::string & id, double speed, int floor,  char gender, int age, int vision ); //this will be the constructer we will use in createAgent;
	//
	//void setExit( const Engine::Point2D<int> & exit ); // not sure what this is
	virtual ~EvacAgent();


	//ADDING LINE TO TRY TO FIX AN ERROR
	typedef std::list<Engine::Point2D<int> > ExitsList;

	//std::list<Engine::Point2D<int> > operator[](int index){return std::list<Engine::Point2D<int> >[index];}

	//void selectActions();
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


