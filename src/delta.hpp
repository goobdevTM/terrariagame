#ifndef DELTA_HPP
#define DELTA_HPP

#include <chrono>

extern double physics_fps;
extern uint frame_count;
extern bool is_physics_tick;
extern double delta;
extern double fps;

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration = std::chrono::duration<float>;

extern TimePoint previous_time; 
extern TimePoint previous_fps_time; 

extern double physics_delta;

void calculate_delta();

#endif //DELTA_HPP