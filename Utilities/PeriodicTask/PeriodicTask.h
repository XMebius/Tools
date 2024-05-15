/*
 * @file PeriodicTask.h
 * @brief 
 */

#ifndef TOOLS_PERIODICTASK_H
#define TOOLS_PERIODICTASK_H

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <cmath>
#include <sys/timerfd.h>
#include <unistd.h>
#include <functional>
#include "Timer.h"

class PeriodicTaskManager;

/*
 * Base class for periodic tasks
 */
class PeriodicTask {
public:
    PeriodicTask(std::string name, float period, PeriodicTaskManager *manager);

    virtual ~PeriodicTask();

    virtual void init() = 0;

    virtual void run() = 0;

    virtual void cleanup() = 0;

    void start();

    void stop();

    std::string getName() { return _taskName; }

private:
    void loopFunction();

    float _taskPeriod;
    std::atomic<bool> _running{false};
    std::thread _taskThread;
    std::string _taskName;

    float _lastRuntime = 0;     // time taken to run the task
    float _lastPeriodTime = 0;  // time taken to run the task and sleep
    float _maxPeriod = 0;       // maximum time taken to run the task
    float _maxRuntime = 0;      // maximum time taken to run the task and sleep
};

class PeriodicTaskManager {
public:
    PeriodicTaskManager() = default;    // do not do anything in the constructor
    ~PeriodicTaskManager();

    void addTask(PeriodicTask *task);

    void startAll();

    void stopAll();

private:
    std::vector<PeriodicTask *> _tasks;
};

class PeriodicFunction : public PeriodicTask {
public:
    PeriodicFunction(
            std::string name, float period, PeriodicTaskManager *manager, std::function<void()> function)
            : PeriodicTask(name, period, manager), _function(function) {}
    ~PeriodicFunction(){}

    void run() {
        _function();
    }

    void init() {
        printf("init %s \n", getName().c_str());
    }

    void cleanup() {}

private:
//    void (*_function)() = nullptr;
//    PeriodicTask *_task = nullptr;
    std::function<void()> _function;
};



#endif //TOOLS_PERIODICTASK_H
