#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

/* Implementation of class "WaitingVehicles" */

int WaitingVehicles::getSize()
{
    std::lock_guard<std::mutex> lock(interMtx);
    return _vehicles.size();
}

void WaitingVehicles::pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise)
{
    std::lock_guard<std::mutex> lock1(interMtx);
    _vehicles.push_back(vehicle);
    _promises.push_back(std::move(promise));
}

void WaitingVehicles::permitEntryToFirstInQueue()
{
    // L2.3 : First, get the entries from the front of _promises and _vehicles. 
    // Then, fulfill promise and send signal back that permission to enter has been granted.
    // Finally, remove the front elements from both queues.

    std::lock_guard<std::mutex> lock2(interMtx);
    auto vehicle = _vehicles.begin();
    auto nextPromise = std::move(_promises.begin());
    
    nextPromise->set_value();

    _vehicles.erase(vehicle);
    _promises.erase(nextPromise);
}

/* Implementation of class "Intersection" */

Intersection::Intersection()
{
    _type = ObjectType::objectIntersection; 
    _isBlocked = false;
}

void Intersection::addStreet(std::shared_ptr<Street> street)
{
    _streets.push_back(street);
}

std::vector<std::shared_ptr<Street>> Intersection::queryStreets(std::shared_ptr<Street> incoming)
{
    // store all outgoing streets in a vector ...
    std::vector<std::shared_ptr<Street>> outgoings;
    for (auto it : _streets)
    {
        if (incoming->getID() != it->getID()) // ... except the street making the inquiry
        {
            outgoings.push_back(it);
        }
    }

    return outgoings;
}

// adds a new vehicle to the queue and returns once the vehicle is allowed to enter<
void Intersection::addVehicleToQueue(std::shared_ptr<Vehicle> vehicle)
{
    TrafficObject::_mtxCout.lock();
    std::cout << "Intersection #" << _id << "::addVehicleToQueue: thread id = " << std::this_thread::get_id() << std::endl;
    TrafficObject::_mtxCout.unlock();
    // L2.2 : First, add the new vehicle to the waiting line by creating a promise, a corresponding future and then adding both to _waitingVehicles. 
    // Then, wait until the vehicle has been granted entry. 

    std::promise<void> vehiclePromise;
    std::future<void> vehicleFuture = vehiclePromise.get_future();
    _waitingVehicles.pushBack(vehicle, std::move(vehiclePromise));
    
    vehicleFuture.wait();
    TrafficObject::_mtxCout.lock();
    std::cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " is granted entry." << std::endl;
    std::lock_guard<std::mutex> uLock(_mutex);
    if(!trafficLightIsGreen()){
        _trafficLight.waitForGreen();
    }
    TrafficObject::_mtxCout.unlock();
}

void Intersection::vehicleHasLeft(std::shared_ptr<Vehicle> vehicle)
{
    //std::cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " has left." << std::endl;

    // unblock queue processing
    this->setIsBlocked(false);
}

void Intersection::setIsBlocked(bool isBlocked)
{
    _isBlocked = isBlocked;
    //std::cout << "Intersection #" << _id << " isBlocked=" << isBlocked << std::endl;
}

// virtual function which is executed in a thread
void Intersection::simulate() // using threads + promises/futures + exceptions
{   
    _trafficLight.simulate();
    // launch vehicle queue processing in a thread
    _threads.emplace_back(std::thread(&Intersection::processVehicleQueue, this));
}

void Intersection::processVehicleQueue()
{
    // print id of the current thread
    //std::cout << "Intersection #" << _id << "::processVehicleQueue: thread id = " << std::this_thread::get_id() << std::endl;

    // continuously process the vehicle queue
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // only proceed when at least one vehicle is waiting in the queue
        if (_waitingVehicles.getSize() > 0 && !_isBlocked)
        {
            // set intersection to "blocked" to prevent other vehicles from entering
            this->setIsBlocked(true);

            // permit entry to first vehicle in the queue (FIFO)
            _waitingVehicles.permitEntryToFirstInQueue();
        }
    }
}

bool Intersection::trafficLightIsGreen(){
    return _trafficLight.getCurrentPhase() == TrafficLightPhase::green;
}
