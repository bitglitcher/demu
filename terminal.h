#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <atomic>
#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <cstdio>
#include <future>
#include "arch/D16i/core/core.h"

#define buffer_lenght 255

#ifdef WIN32
// no getchar_unlocked on Windows so call _getchar_nolock
inline int getchar_unlocked() { return _getchar_nolock(); }
#endif

class terminal
{
    public:
    void read();
    void write();
    terminal(device_port*, bool&, std::mutex&, unsigned int&);
    device_port *some;
    bool kill;   
    std::mutex *terminal_mutex;
    unsigned int *update_rate;
};
