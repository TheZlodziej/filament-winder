#pragma once
#ifndef __MICROSTEP_DRIVER_CONTROLLER__
#define __MICROSTEP_DRIVER_CONTROLLER__

#include <cinttypes>

#include "stm32f7xx_hal.h"

class MicrostepDriverController {
/* enums */
public:
    enum Direction { LEFT = 0, RIGHT = 1 };

/* structs */
public:
    struct GPIO_Pin_Pair {
        GPIO_TypeDef* GPIO = nullptr;
        uint16_t pin = 0;
    };

    struct Specification {
        /*  
            PUL -> pulses that enable rotation
            DIR -> low/high for direction
        */

        /*
            steps_per_revolution = motor_steps * 1/microsteps
            e.g. 200 * 1/(0.5) = 400
        */
        uint32_t steps_per_revolution = 200;

        /*  minimum high level width */
        uint64_t min_high_level_width = 3; // [us]

        /*  time between pulses */
        uint64_t min_low_level_width = 3; // [us]

        /* 
            minimum time before pulse signal
            is changed for direction signal
            to be noticed 
        */
        uint64_t min_direction_advance = 5; // [us]
    };

/* variables */
private:
    Specification _specs;
    TIM_HandleTypeDef& _timer_ref;
    uint64_t _step_delay;

    /* gpio out */
    GPIO_Pin_Pair _pulse_pin_pair;
    GPIO_Pin_Pair _dir_pin_pair;

/* methods */
public:
    void set_speed(uint16_t rpm);
    void set_direction(Direction dir);
    void send_pulse(uint16_t count);

private:
    void u_delay(uint64_t us);

/* constructor */
public:
    MicrostepDriverController(const Specification& specification, TIM_HandleTypeDef& timer, const GPIO_Pin_Pair& pulse_pin_pair, const GPIO_Pin_Pair& direction_pin_pair);
};

#endif // __MICROSTEP_DRIVER_CONTROLLER__
