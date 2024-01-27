#include "Motor.h"

class Fan : public Motor
{
    private:
        uint8_t pin;
        bool state;
    public:
        Fan(uint8_t pin, bool state);
        ~Fan();
        void open() override;
        void close() override;
        void setupMotor() override;
        bool getState() override;

};