#ifndef TRAFFIC_LIGHT_PHASE_TIME_H 
#define TRAFFIC_LIGHT_PHASE_TIME_H
#include <random>

class TrafficLightPhaseTime
{
private:
    std::mt19937 _generator { std::random_device()() };
	std::uniform_int_distribution<int> _distribution;
    int _min;
    int _max;
public:
    TrafficLightPhaseTime(int min, int max);
    int Get();
    int GetFloorBound() const { return _min; };
    int GetTopBound() const { return _max; };
};

#endif