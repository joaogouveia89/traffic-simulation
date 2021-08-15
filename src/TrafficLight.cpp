#include "TrafficLight.h"
#include <iostream>

TrafficLight::TrafficLight(){
    std::srand(Helper::GenerateSeedForRand());
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
        _phaseQueue.PopBack();
        generateCurrentCycleTime();
        std::this_thread::sleep_for(std::chrono::seconds(_currentCycleTimeSec));
        InvertLight();
        TrafficLightPhase newPhase = _currentPhase; // to not lose the reference of current phase when calling move semantics
        _phaseQueue.Send(std::move(newPhase));
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

void TrafficLight::waitForGreen(){
    while(true){ 
        if(_phaseQueue.Receive() == TrafficLightPhase::green) return;
    }
}

template <class T>
void MessageQueue<T>::Send(T&& phase){
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.emplace_back(std::move(phase));
    _condition.notify_one();
}

template <class T>
T MessageQueue<T>::Receive(){
     std::unique_lock<std::mutex> uLock(_mutex);
    _condition.wait(uLock, [this] { return !_queue.empty(); }); 
    T item = std::move(_queue.front());
    _queue.pop_front();
    return item;
}

template <class T>
void MessageQueue<T>::PopBack(){
    std::unique_lock<std::mutex> uLock(_mutex);
    while(!_queue.empty()) _queue.pop_back();
}