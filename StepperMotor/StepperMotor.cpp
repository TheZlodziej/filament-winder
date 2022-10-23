#include "StepperMotor.hpp"

#include <cstdlib>

StepperMotor::StepperMotor(
	TIM_HandleTypeDef& timer,
	uint16_t steps,
	const GPIO_Pin_Pair& w1,
	const GPIO_Pin_Pair& w2):
	/* common */
	_timer_ref(timer),
	_number_of_steps(steps),
	_direction(RIGHT),
	_last_step_time(0ULL),
	_curr_step_number(0),
	_step_delay(UINT64_MAX),

	/* motor type specific */
	_gpio_pin_pair_count(2U),
	_gpio_pin_pair_1(w1),
	_gpio_pin_pair_2(w2)
{}

StepperMotor::StepperMotor(
	TIM_HandleTypeDef& timer,
	uint16_t steps,
	const GPIO_Pin_Pair& w1,
	const GPIO_Pin_Pair& w2,
	const GPIO_Pin_Pair& w3,
	const GPIO_Pin_Pair& w4):
	/* common */
	_timer_ref(timer),
	_number_of_steps(steps),
	_direction(RIGHT),
	_last_step_time(0ULL),
	_curr_step_number(0),
	_step_delay(UINT64_MAX),

	/* motor type specific */
	_gpio_pin_pair_count(4U),
	_gpio_pin_pair_1(w1),
	_gpio_pin_pair_2(w2),
	_gpio_pin_pair_3(w3),
	_gpio_pin_pair_4(w4)
{}

StepperMotor::StepperMotor(
	TIM_HandleTypeDef& timer,
	uint16_t steps,
	const GPIO_Pin_Pair& w1,
	const GPIO_Pin_Pair& w2,
	const GPIO_Pin_Pair& w3,
	const GPIO_Pin_Pair& w4,
	const GPIO_Pin_Pair& w5) :
	/* common */
	_timer_ref(timer),
	_number_of_steps(steps),
	_direction(RIGHT),
	_last_step_time(0ULL),
	_curr_step_number(0),
	_step_delay(UINT64_MAX),

	/* motor type specific */
	_gpio_pin_pair_count(5U),
	_gpio_pin_pair_1(w1),
	_gpio_pin_pair_2(w2),
	_gpio_pin_pair_3(w3),
	_gpio_pin_pair_4(w4),
	_gpio_pin_pair_5(w5)
{}

void StepperMotor::StepperMotor_SetSpeed(uint8_t speed /* [rev/min] */) {
	_step_delay = 60ULL * 1000ULL * 1000ULL / static_cast<uint64_t>(_number_of_steps) / static_cast<uint64_t>(speed);
}

void StepperMotor::StepperMotor_Step(int32_t step_number) {
	__HAL_TIM_SET_COUNTER(&_timer_ref, 0);
	_direction = step_number > 0 ? RIGHT : LEFT;
	int32_t steps_left = abs(step_number);

	while (steps_left > 0) {
		uint64_t now = __HAL_TIM_GET_COUNTER(&_timer_ref);

		if (now - _last_step_time < _step_delay) {
			continue;
		}

		_last_step_time = now;
		_curr_step_number += _direction == RIGHT ? 1 : -1;
		_curr_step_number %= _number_of_steps;
		steps_left--;
		StepperMotor_StepMotor(_curr_step_number);
	}
}

void StepperMotor::StepperMotor_StepMotor(uint16_t step_sequence_index) {
	step_sequence_index %= _gpio_pin_pair_count == 5 ? 10 : 4;

	switch (_gpio_pin_pair_count) {
	case 2:
		switch (step_sequence_index) {
		case 0: /* 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			break;
		case 1: /* 1 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			break;
		case 2: /* 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			break;
		case 3: /* 0 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			break;
		}
		break;

	case 4:
		switch (step_sequence_index) {
		case 0: /* 1 0 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			break;
		case 1: /* 0 1 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			break;
		case 2: /* 0 1 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			break;
		case 3: /* 1 0 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			break;
		}
		break;

	case 5:
		switch (step_sequence_index) {
		case 0: /* 0 1 1 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_SET);
			break;
		case 1: /* 0 1 0 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_SET);
			break;
		case 2: /* 0 1 0 1 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_SET);
			break;
		case 3: /* 0 1 0 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_RESET);
			break;
		case 4: /* 1 1 0 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_RESET);
			break;
		case 5: /* 1 0 0 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_RESET);
			break;
		case 6: /* 1 0 1 1 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_RESET);
			break;
		case 7: /* 1 0 1 0 0 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_RESET);
			break;
		case 8: /* 1 0 1 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_SET);
			break;
		case 9: /* 0 0 1 0 1 */
			HAL_GPIO_WritePin(_gpio_pin_pair_1.GPIO, _gpio_pin_pair_1.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_2.GPIO, _gpio_pin_pair_2.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_3.GPIO, _gpio_pin_pair_3.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpio_pin_pair_4.GPIO, _gpio_pin_pair_4.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpio_pin_pair_5.GPIO, _gpio_pin_pair_5.pin, GPIO_PIN_SET);
			break;
		}
		break;
	}
}
