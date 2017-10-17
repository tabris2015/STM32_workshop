#include "ir_array.h"

IR_array::IR_array(uint8_t nSensors, uint32_t * dataBuffer, float * weights):
_nSensors(nSensors), 
_dataBuffer(dataBuffer), 
_sensorWeights(weights)
{
    //_sensorWeights[8] = {28.0, 20.0, 12.0, 4.0, -4.0, -12.0, -20.0,-28.0};
}
// private methods
void IR_array::_normalize()
{
    for(int i = 0; i < _nSensors; i++)
    {
        _normalizedValues[i] = _map(_dataBuffer[i], _minVals[i], _maxVals[i], MIN_NORM, MAX_NORM);
    }
}

uint32_t IR_array::_map(uint32_t x, uint32_t inMin, uint32_t inMax, uint32_t outMin, uint32_t outMax)
{
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
// public methods
void IR_array::calibrate()
{
    for(int r = 0; r < CAL_SAMPLES; r++)
    {
        // find max and min
        for(int i = 0; i < _nSensors; i++)
        {
            _maxVals[i] = _dataBuffer[i] > _maxVals[i] ? _dataBuffer[i] : _maxVals[i];
            _minVals[i] = _dataBuffer[i] < _minVals[i] ? _dataBuffer[i] : _minVals[i];
        }
        // 30 ms delay 
				HAL_Delay(20);
    }    
}

void IR_array::readSensors()
{

}
uint8_t IR_array::getBynary()
{
    _normalize();
    uint8_t binVals = 0;
    for(int i = 0; i < _nSensors; i++)
    {
        binVals |= _normalizedValues[i] > 1500 ? 1 << i: 0;
    }

    return binVals;
}
uint32_t * IR_array::getNormalized()
{
    return _normalizedValues;
}


float IR_array::getLinePosition()
{
    float accum = 0;
    uint32_t norm = 0; // zero division!
    // first normalize values
    _normalize();
    // then make the avg 
    for(int i = 0; i < _nSensors; i++)
    {
        accum += _sensorWeights[i] * (float)_normalizedValues[i];
        norm += _normalizedValues[i];
    }
    
    if(norm > 0)
    {
        _linePosition = accum / (float)norm;
    }
    return _linePosition;
}
