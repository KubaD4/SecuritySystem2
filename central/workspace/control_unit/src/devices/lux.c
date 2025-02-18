#include "../../include/lux.h"

/* Variable for storing lux value returned from OPT3001 */
float lux;

void _lightSensorInit()
{
    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    /* Initialize OPT3001 digital ambient light sensor */
    OPT3001_init();

    __delay_cycles(100000);

};
