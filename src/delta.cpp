#include "delta.hpp"
#include <cmath>

double physics_fps = 0.0;
uint frame_count = 0;
bool is_physics_tick = false;
double delta = 0.0;
double fps = 0.0;

TimePoint previous_time = Clock::now();
TimePoint previous_fps_time = Clock::now();

double physics_delta = 0.0;

void calculate_delta() {
    frame_count ++;
    TimePoint current_time = Clock::now();
    Duration delta_time = current_time - previous_time;
    Duration fps_delta_time = current_time - previous_fps_time;
    if (fps_delta_time.count() > 1.0) {
        fps = (double)frame_count / fps_delta_time.count();
        
        frame_count  = 0;
        previous_fps_time = current_time;
    }
    previous_time = current_time;

    delta = delta_time.count();

    
    is_physics_tick = false;
    if (physics_delta * physics_fps > 1.0) {
        physics_delta = std::fmod(physics_delta, 1.0);
        is_physics_tick = true;
    }
    physics_delta += delta; 
}