#include "TrafficLight.h"
#include <iostream>

void TrafficLight::simulate(){
    std::thread lightSimulation(&::TrafficLight::cycleThroughPhases, this);
    _threads.emplace_back(std::move(lightSimulation));
}

TrafficLightPhase TrafficLight::getCurrentPhase() const{
    return _currentPhase;
}

void TrafficLight::cycleThroughPhases(){
    while(true){
        if(!HasElapsedCycleTime()){
            continue;
        }
        _phaseQueue.PopBack();
        //randomly chose of current cycle period
        InvertLight();
        _phaseQueue.Send(std::move(_currentPhase));
        _currentCycleTimeMili = phaseTime.Get();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

bool TrafficLight::HasElapsedCycleTime(){
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    int difference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastMeasuredTime).count();
    if(difference >= _currentCycleTimeMili){
        lastMeasuredTime = currentTime;
        return true;
    }
    return false;
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