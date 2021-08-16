#include "TrafficLightPhaseTime.h"

TrafficLightPhaseTime::TrafficLightPhaseTime(int min, int max)
    :_distribution(min, max), _min(min), _max(max){}

int TrafficLightPhaseTime::Get(){
    return _distribution(_generator);
}