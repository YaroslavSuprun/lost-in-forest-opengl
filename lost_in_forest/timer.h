#ifndef TIMER_H_
#define TIMER_H_

namespace game {

    // A class implementing a simple timer
    class Timer {

        public:
            // Constructor and destructor
            Timer();
            ~Timer();

            // Start the timer NOW and end it in "delay" seconds from now
            // The delay can include a fractional part, e.g, 1.5 seconds
            void Start(double delay); 

            // Check if timer has finished
            bool Finished(void) const;

            double Remaining() const;

        private:
            double end_time_;

    }; // class Timer

} // namespace game

#endif // TIMER_H_
