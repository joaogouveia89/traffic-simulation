#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H
#include "TrafficObject.h"
#include "TrafficLightPhaseTime.h"
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <chrono>
#include <random>
#include <sstream>

enum class TrafficLightPhase{
    red,
    green
};

template <class T>
class MessageQueue
{
public:
    void Send(T&& phase);
    T Receive();
    void PopBack();
private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

class TrafficLight: public TrafficObject
{
private:
    void cycleThroughPhases();
    void InvertLight();
    TrafficLightPhase _currentPhase { TrafficLightPhase::red }; /* arbitrary choose */
    int _currentCycleTimeSec;
    MessageQueue<TrafficLightPhase> _phaseQueue;    
    TrafficLightPhaseTime phaseTime { 4000, 6000 };
public:
    void generateCurrentCycleTime();
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase() const;
    std::string ToString() const;
};

#endif