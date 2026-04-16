#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void)
{
    end_time_ = 0.0;
}


Timer::~Timer()
{
}


void Timer::Start(double delay)
{

    double current_time = glfwGetTime();
    end_time_ = current_time + delay;
}


bool Timer::Finished(void) const
{

    double current_time = glfwGetTime();

    //For debug
    //std::cout << current_time << " " << end_time_ << std::endl;

    return (current_time >= end_time_);
}

double Timer::Remaining() const {
    double current_time = glfwGetTime();
    double remaining = end_time_ - current_time;
    return remaining > 0.0 ? remaining : 0.0;
}

} // namespace game
