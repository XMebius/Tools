/*
 * @file pcRead.h
 * @brief 
 */

#ifndef TOOLS_PCREAD_H
#define TOOLS_PCREAD_H

#include "PeriodicTask.h"

struct cpuInfo {
    float user;
    float nice;
    float system;
    float idle;
    float iowait;
    float irq;
    float softirq;
    float steal;
    float guest;
    float guest_nice;
};

struct ramInfo {
    float total;
    float free;
    float available;
    float buffers;
    float cached;
    float swap_total;
    float swap_free;
};

struct diskInfo {
    float read;
    float write;
};

class pcRead {
public:
    pcRead();
    ~pcRead();

    void run();
    void readCPU();
    void readRAM();
    void readDisk();

    PeriodicFunction* cpuTask;
    PeriodicFunction* ramTask;
    PeriodicFunction* diskTask;

private:
    PeriodicTaskManager* _manager;
    cpuInfo _cpu;
    ramInfo _ram;
    diskInfo _disk;
};


#endif //TOOLS_PCREAD_H
