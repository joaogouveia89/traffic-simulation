#include "TrafficLight.h"

TrafficLight::TrafficLight(){
    std::srand(Helper::GenerateSeedForRand());
}

void TrafficLight::waitForGreen(){

}

void TrafficLight::simulate(){
    std::thread lightSimulation(&::TrafficLight::cycleThroughPhases, this);
    _threads.emplace_back(std::move(lightSimulation));
}

TrafficLightPhase TrafficLight::getCurrentPhase() const{
    return _currentPhase;
}

void TrafficLight::cycleThroughPhases(){
    while(true){
        generateCurrentCycleTime();
        std::this_thread::sleep_for(std::chrono::seconds(_currentCycleTimeSec));
        InvertLight();
        // TODO send update method to message queue(FP4)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void TrafficLight::generateCurrentCycleTime(){
    _currentCycleTimeSec = std::rand() % 10;
    if(_currentCycleTimeSec > CYCLE_TIME_TOP_LIMIT){
        _currentCycleTimeSec = CYCLE_TIME_TOP_LIMIT;
    }else if(_currentCycleTimeSec < CYCLE_TIME_FLOOR_LIMIT){
        _currentCycleTimeSec = CYCLE_TIME_FLOOR_LIMIT;
    }
}

void TrafficLight::InvertLight(){
    if(_currentPhase == TrafficLightPhase::red){
        _currentPhase = TrafficLightPhase::green;
        return;
    }
    _currentPhase = TrafficLightPhase::red;
}