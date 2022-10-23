#pragma once
#ifndef __STEPPER_MOTOR__
#define __STEPPER_MOTOR__

#include <cinttypes>

#include "stm32f7xx_hal.h"

/*
    based on https://github.com/arduino-libraries/Stepper
    step sequence for different motors (based on output wires)

    2 wires W1, W2
    W1  W2
    0   1
    1   1
    1   0
    0   0

    4 wires W1, W2, W3, W4
    W1  W2  W3  W4
    1   0   1   0
    0   1   1   0
    0   1   0   1
    1   0   0   1

    5 wires W1, W2, W3, W4, W5
    W1  W2  W3  W4  W5
    0	1	1	0	1
    0	1	0	0	1
    0	1	0	1	1
    0	1	0	1	0
    1	1	0	1	0
    1	0	0	1	0
    1	0	1	1	0
    1	0	1	0	0
    1	0	1	0	1
    0	0	1	0	1
*/

/* zeby dzialalo (podswietlalo sie) nie na halu xd
class GPIO_TypeDef {};
class TIM_HandleTypeDef {};
enum GPIO_PinState{ GPIO_PIN_RESET = 0, GPIO_PIN_SET };
void HAL_GPIO_WritePin(GPIO_TypeDef* _, uint16_t __, GPIO_PinState ___) { return; }
#define __HAL_TIM_SET_COUNTER()
#define __HAL_TIM_GET_COUNTER() 16u
end zeby dzialalo (podswietlalo sie) nie na halu xd */

class StepperMotor {
/* enums */
public:
    struct GPIO_Pin_Pair {
        GPIO_TypeDef* GPIO = nullptr;
        uint16_t pin = 0;
    };
private:
    enum StepperMotor_Direction { LEFT = 0, RIGHT = 1 };

/* variables */
private:
    StepperMotor_Direction _direction;  /* dir of rotation */
    uint64_t _step_delay;               /* delay between steps in us (based on speed) */
    uint64_t _last_step_time;           /* timestamp of last step in us */
    uint16_t _number_of_steps;          /* num of motor steps (including microstepping) */
    uint8_t _gpio_pin_pair_count;       /* num of pins in use */
    uint16_t _curr_step_number;         /* curr motor step */
    TIM_HandleTypeDef& _timer_ref;      /* timer reference */

    /* gpio/pin pairs */
    GPIO_Pin_Pair _gpio_pin_pair_1;
    GPIO_Pin_Pair _gpio_pin_pair_2;
    GPIO_Pin_Pair _gpio_pin_pair_3;
    GPIO_Pin_Pair _gpio_pin_pair_4;
    GPIO_Pin_Pair _gpio_pin_pair_5;

/* constructors */
public:
    StepperMotor() = delete;
    StepperMotor(TIM_HandleTypeDef& timer /* freq = 1 MHz -> T = 1us */, uint16_t steps, const GPIO_Pin_Pair& w1, const GPIO_Pin_Pair& w2);
    StepperMotor(TIM_HandleTypeDef& timer /* freq = 1 MHz -> T = 1us */, uint16_t steps, const GPIO_Pin_Pair& w1, const GPIO_Pin_Pair& w2, const GPIO_Pin_Pair& w3, const GPIO_Pin_Pair& w4);
    StepperMotor(TIM_HandleTypeDef& timer /* freq = 1 MHz -> T = 1us */, uint16_t steps, const GPIO_Pin_Pair& w1, const GPIO_Pin_Pair& w2, const GPIO_Pin_Pair& w3, const GPIO_Pin_Pair& w4, const GPIO_Pin_Pair& w5);

/* methods */
public:
    void StepperMotor_SetSpeed(uint8_t speed /* [rev/min] */);  /* sets speed of rotation */
    void StepperMotor_Step(int32_t step_number);                /* send pulse <step_number> times. if <step_number> < 0, moves in opposite direction */
private:
    void StepperMotor_StepMotor(uint16_t step_sequence_index);  /* sets output pins to correct values according to the type of motor */
};

#endif /* __STEPPER_MOTOR__ */
