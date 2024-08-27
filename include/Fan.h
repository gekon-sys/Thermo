#include "motor.h"

class Fan : public Motor
{
    private:
        static uint8_t counter;
        static Fan* instances[];

        const char *name;
        uint8_t pin;
        bool state;
    public:
        Fan(uint8_t pin, const char* name, bool state);
        ~Fan() override;
        bool setupMotor() override;
        void open() override;
        void close() override;
        bool getState() override;
        uint8_t getPin() override;
        static uint8_t getCounter();
        static Fan* getInstance(int index);
        const char* getName() override;

};