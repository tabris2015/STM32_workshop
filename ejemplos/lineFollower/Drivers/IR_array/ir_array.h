#ifndef IR_ARRAY_H
#define IR_ARRAY_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
// defines
#define MAX_NORM 3000   // max value for normalization
#define MIN_NORM 0      // the same
#define MAX_ADC_VALUE 4095      // max value from ADC, with 12 bits
#define MIN_ADC_VALUE 0         // same
#define CAL_SAMPLES 100
#define DEFAULT_SENSORS 8


/*
IR_array class
this class helps to calibrate and normalize the sensor values and then 
compute the line position through weighted average.
*/
class IR_array
{
public:
    /* the constructor takes the number of sensors and a pointer to the 
    ADC values, in this case, we are using DMA */
    IR_array(uint8_t nSensors, uint32_t * dataBuffer, float * weights);
    /* This method returns the line position in mm, 0 means that the line
    in the center of the array, +/- 30.0 means that is in the limits */
    void calibrate();
    void readSensors();
    uint8_t getBynary();
    uint32_t * getNormalized();
    float getLinePosition();
    /* executes a calibration routine and stores calibration values */
private:
    // methods
    uint32_t _map(uint32_t x, uint32_t inMin, uint32_t inMax, uint32_t outMin, uint32_t outMax);
    void _normalize();  // normalize
    // data
    uint32_t * _dataBuffer;                 // pointer to ADC values buffer
    
    uint32_t _maxVals[DEFAULT_SENSORS];             // max values from  rosutine
    uint32_t _minVals[DEFAULT_SENSORS];             // same
    uint32_t _normalizedValues[DEFAULT_SENSORS];
    float * _sensorWeights;
    uint8_t _nSensors;                      // number of sensors

    float _linePosition;                    // value of line position 
    
    
};
#endif //IR_ARRAY_H
