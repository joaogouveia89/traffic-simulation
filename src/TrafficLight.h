#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H
#include "TrafficObject.h"

enum class TrafficLightPhase{
    red,
    green
};

class TrafficLight: public TrafficObject
{
private:
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase { TrafficLightPhase::red }; /* arbitrary choose */

public:
    TrafficLight();
    ~TrafficLight();


    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();
};

#endif