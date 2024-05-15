/*
 * @file PeriodicTask.cpp
 * @brief 
 */

#include "PeriodicTask.h"

PeriodicTask::PeriodicTask(std::string name, float period, PeriodicTaskManager *manager) :
        _taskName(name), _taskPeriod(period){
    manager->addTask(this);
}

PeriodicTask::~PeriodicTask() {
    stop();
}

void PeriodicTask::start() {
    if(!_running){
        printf("[Periodic] Tried to start %s but it's already running\n", _taskName.c_str());
        return;
    }
    init();
    _running = true;
    _taskThread = std::thread(&PeriodicTask::loopFunction, this);
}

void PeriodicTask::stop() {
    if(!_running){
        printf("[Periodic] Tried to stop %s but it's already stopped\n", _taskName.c_str());
        return;
    }
    _running = false;
    printf("[Periodic] Waiting for stopping %s\n", _taskName.c_str());
    _taskThread.join();
    printf("[Periodic] Stopped %s\n", _taskName.c_str());
    cleanup();
}

/*
 * call the run() function every _taskPeriod seconds
 */
void PeriodicTask::loopFunction() {
    auto timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
    int seconds = (int)_taskPeriod;
    int nanoseconds = (int)(1e9 * std::fmod(_taskPeriod, 1.f));
    Timer t;

    itimerspec timerSpec;
    timerSpec.it_interval.tv_sec = seconds;
    timerSpec.it_interval.tv_nsec = nanoseconds;
    timerSpec.it_value.tv_sec = seconds;
    timerSpec.it_value.tv_nsec = nanoseconds;


    timerfd_settime(timerFd, 0, &timerSpec, nullptr);

    unsigned long long missed = 0;

    printf("[PeriodicTask] Start %s (%d s, %d ns)\n", _taskName.c_str(), seconds,
           nanoseconds);

    while(_running){
        _lastPeriodTime = (float)t.getSeconds();
        t.start();
        run();
        _lastRuntime = (float)t.getSeconds();
        int m = read(timerFd, &missed, sizeof(missed));
        (void)m;
        _maxPeriod = std::max(_maxPeriod, _lastPeriodTime);
        _maxRuntime = std::max(_maxRuntime, _lastRuntime);
    }
    printf("[PeriodicTask] %s has stopped!\n", _taskName.c_str());
}

PeriodicTaskManager::~PeriodicTaskManager() {
    for(auto task : _tasks){
        delete task;
    }
}

void PeriodicTaskManager::addTask(PeriodicTask *task) {
    _tasks.push_back(task);
}

void PeriodicTaskManager::startAll() {
    for(auto task : _tasks){
        task->start();
    }
}

void PeriodicTaskManager::stopAll() {
    for(auto task : _tasks){
        task->stop();
    }
}
