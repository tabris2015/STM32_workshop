#ifndef LED_H
#define LED_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "math.h"

typedef enum ledState_t
{
  LED_OFF = 0,
  LED_ON
}ledState;

class Led 
{
    private:
    GPIO_TypeDef * _port;
    uint16_t _pin;
    ledState _state;
    bool _reversed;

    public:
    Led(GPIO_TypeDef * port, uint16_t pin, bool reversed);
		Led(GPIO_TypeDef * port, uint16_t pin);
    ledState getState();
		void toggle();
    void write(ledState value);
    void operator=(ledState value);
		void operator=(int value);
		void operator=(Led led);
    ledState operator!() const;
};
#endif // LED_H
