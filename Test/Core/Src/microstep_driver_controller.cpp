#include "microstep_driver_controller.hpp"

#include <algorithm>

MicrostepDriverController::MicrostepDriverController(
    const Specification& specification,
    TIM_HandleTypeDef& timer,
    const GPIO_Pin_Pair& pulse_pin_pair,
    const GPIO_Pin_Pair& direction_pin_pair):
    _specs(specification),
    _timer_ref(timer),
    _pulse_pin_pair(pulse_pin_pair),
    _dir_pin_pair(direction_pin_pair),
	_step_delay(UINT64_MAX)
{}

void MicrostepDriverController::set_speed(uint16_t rpm) {
    _step_delay = 60ULL * 1000ULL * 1000ULL / static_cast<uint64_t>(_specs.steps_per_revolution) / static_cast<uint64_t>(rpm);
}

void MicrostepDriverController::set_direction(Direction dir) {
    HAL_GPIO_WritePin(_dir_pin_pair.GPIO, _dir_pin_pair.pin, dir == RIGHT ? GPIO_PIN_SET : GPIO_PIN_RESET);
    u_delay(_specs.min_direction_advance);
}

void MicrostepDriverController::send_pulse(uint16_t count) {
    uint64_t high_state_width = std::max(_specs.min_high_level_width, _step_delay / 2ULL);
    uint64_t low_state_width = std::max(_specs.min_high_level_width, _step_delay - high_state_width);

    while(count--) {
        HAL_GPIO_WritePin(_pulse_pin_pair.GPIO, _pulse_pin_pair.pin, GPIO_PIN_SET);
        u_delay(high_state_width);
        HAL_GPIO_WritePin(_pulse_pin_pair.GPIO, _pulse_pin_pair.pin, GPIO_PIN_RESET);
        u_delay(low_state_width);
    }
}

void MicrostepDriverController::u_delay(uint64_t us) {
    __HAL_TIM_SET_COUNTER(&_timer_ref, 0);  
	while (__HAL_TIM_GET_COUNTER(&_timer_ref) < us);
}
