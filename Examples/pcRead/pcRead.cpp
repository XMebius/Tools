/*
 * @file pcRead.cpp
 * @brief 
 */

#include "pcRead.h"
#include <iostream>

pcRead::pcRead() {
    _manager = new PeriodicTaskManager();
    cpuTask = new PeriodicFunction("cpuTask", 1, _manager, std::bind(&pcRead::readCPU, this));

}

pcRead::~pcRead() {
    _manager->stopAll();
    delete _manager;
    delete cpuTask;
}

void pcRead::run() {
    std::cout << "run..." << std::endl;
//    _manager->startAll();
    cpuTask->start();
}

void pcRead::readCPU() {
    std::cout << "pcRead::readCPU()" << std::endl;
}

void pcRead::readRAM() {
    std::cout << "pcRead::readRAM()" << std::endl;
}

void pcRead::readDisk() {
    std::cout << "pcRead::readDisk()" << std::endl;
}