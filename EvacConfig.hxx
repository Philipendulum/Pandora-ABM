#ifndef __EvacConfig_hxx__
#define __EvacConfig_hxx__

#include <Config.hxx>
#include <Size.hxx>
#include <Point2D.hxx>
#include <map>
#include <list>

namespace Evacuation
{


class EvacConfig : public Engine::Config
{

        typedef std::list<Engine::Point2D<int> > SignList;
        typedef std::list<Engine::Point2D<int> > ExitConfigList;
        typedef std::list<Engine::Point2D<int> > DoorList;
        typedef std::list<int> DoorValueDirList;       
        typedef std::list<Engine::Point2D<int> > TurnList;
        typedef std::list<Engine::Point2D<int> > JumpCellsList;
        typedef struct door{
        Engine::Point2D<int> location; 
        char _DIR; // if valuable direction of the passable path (left to right is R, right to left L, from up to down is D, from down to UP U)
        char _VALUE; // indication if the door is actually valuable
        }door;
        typedef struct jumping{
        Engine::Point2D<int> jfrom;
        Engine::Point2D<int> jto;
        }jumping;


	int _numAgents;
        int initialPanicked;
        int evacuationThreshold;
        int childPercentage;
        int elderlyPercentage;
        int malePercentage;
        int adultSpeed;
        int elderlySpeed;
        int childSpeed;
        int adultStairsSpeed;
        int elderlyStairsSpeed;
        int childStairsSpeed;
        int floorNumber;
        int jumpNumber;
        int turnNumber;
        int doorNumber;
        int exitNumber;
        int signNumber;
        int notMovedPanicTreshold;
        int talkingProb;
        int talkingRadius;
        int maxDoorWidth;
        int systemRasters;
        int visionAdult;
        int visionElderly;
        int visionChild;
        float elderlyTrampling;
        float childTrampling;
        float manTrampling;
        float womanTrampling;
        float delayMin;
        float delayMax;
        float elderlyWeightMean;
        float elderlyWeightStDev;
        float childWeightMean;
        float childWeightStDev;
        float manWeightMean;
        float manWeightStDev;
        float womanWeightMean;
        float womanWeightStDev; 
        int getuptime;
        int chemoDecayRate;        
        // exit list from config
        ExitConfigList exitconfiglist;
        	
        // Sign List
        SignList signList;

        // Door List
        DoorList doorList;

        DoorValueDirList doorValueList, doorDirList;

        // Turn List (for stairs)
        TurnList turnList;
        
        // Jumping Cell Lists (to and from)
        JumpCellsList jumpCellsfrom, jumpCellsto;


public:
	EvacConfig( const std::string & xmlFile );
	virtual ~EvacConfig();
        jumping jumps;
        door doors;
	void loadParams();
        int returnChildPerc() const{return childPercentage;}
        int returnMalePerc() const{return malePercentage;}
        int returnElderlyPerc() const{return elderlyPercentage;}
        int returnFloorNumber() const{return floorNumber;}
        int returnPanicTresh() const{return notMovedPanicTreshold;}
        int returnInitialPanicked() const{return initialPanicked;}
        
	
	friend class EvacWorld;
	friend class EvacAgent;
};

} // namespace Evacuation

#endif 

