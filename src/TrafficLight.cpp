#include "TrafficLight.h"

TrafficLight::TrafficLight(){
    srand (time(NULL));
}

void TrafficLight::waitForGreen(){

}

void TrafficLight::simulate(){
    
}

TrafficLightPhase TrafficLight::getCurrentPhase() const{
    return _currentPhase;
}

void TrafficLight::cycleThroughPhases(){
    
}

void TrafficLight::generateCurrentCycleTime(){
    _currentCycleTimeSec = rand() % 10;
    if(_currentCycleTimeSec > CYCLE_TIME_TOP_LIMIT){
        _currentCycleTimeSec = CYCLE_TIME_TOP_LIMIT;
    }else if(_currentCycleTimeSec < CYCLE_TIME_FLOOR_LIMIT){
        _currentCycleTimeSec = CYCLE_TIME_FLOOR_LIMIT;
    }
}