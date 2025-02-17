#include "../../include/tmp.h"

void _temperatureSensorInit()
{
    /* Temperature Sensor initialization */
    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();
    /* Initialize TMP006 temperature sensor */
    TMP006_init();
    __delay_cycles(100000);

}
