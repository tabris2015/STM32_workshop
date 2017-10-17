#include "led.h"
Led::Led(GPIO_TypeDef * port, uint16_t pin, bool reversed=false):
_port(port), 
_pin(pin), 
_reversed(reversed)
{
    _state = LED_OFF;
    // led off
    HAL_GPIO_WritePin(_port, _pin, _reversed ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
Led::Led(GPIO_TypeDef * port, uint16_t pin):
_port(port), 
_pin(pin), 
_reversed(false)
{
    _state = LED_OFF;
    // led off
    HAL_GPIO_WritePin(_port, _pin, _reversed ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
ledState Led::getState(void)
{
    return _state;
}

void Led::toggle()
{
	HAL_GPIO_TogglePin(_port, _pin);
}

void Led::write(ledState value)
{
    if(value == LED_ON)
    {
        _port->BSRR = _reversed ? (uint32_t)_pin << 16 : _pin;
    }
    else
    {
        _port->BSRR = _reversed ? _pin: (uint32_t)_pin << 16;
    }
	_state = value;
}

void Led::operator=(ledState value)
{
    write(value);    
}
void Led::operator=(int value)
{
    write((ledState) value);    
}


void Led::operator=(Led led)
{
	write(led.getState());
}
ledState Led::operator!() const
{
    return (_state == LED_ON) ? LED_OFF : LED_ON;
}
