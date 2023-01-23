#ifndef TIMER_HPP
#define TIMER_HPP
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

class Timer{
    /*
    
    Timer* timer = new Timer;
       .// some stuff
    delete timer;

    */
    std::chrono::_V2::system_clock::time_point start,stop;
public:
    Timer(){
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer(){
        stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = stop - start;
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << "\n"; 
    }
};

#endif // TIMER_HPP