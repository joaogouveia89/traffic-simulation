#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H
#include "TrafficObject.h"
#include "Helper.h"
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <chrono>
#include <random>

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
    static constexpr int CYCLE_TIME_TOP_LIMIT = 6; //no need to define this in runtime, put the constexpr to define in compile time
    static constexpr int CYCLE_TIME_FLOOR_LIMIT = 4; //no need to define this in runtime, put the constexpr to define in compile time

public:
    TrafficLight();
    ~TrafficLight();

    void generateCurrentCycleTime();
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase() const;
};

#endif