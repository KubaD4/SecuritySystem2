//#ifndef SOUND_DETECTION_H_
//#define SOUND_DETECTION_H_
//
//#include <ti/devices/msp432p4xx/inc/msp.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//
//// Constants for sound detection
//#define SOUND_THRESHOLD 20000  // Threshold for sound detection
//#define SAMPLES_ABOVE_THRESHOLD 5  // Number of consecutive samples needed
//#define SAMPLING_PERIOD_MS 100  // Time between samples in milliseconds
//
//// Function prototypes
//void initSoundDetection(void);
//void enableSoundDetection(void);
//void disableSoundDetection(void);
//
//// Interrupt handlers
//void ADC14_IRQHandler(void);
//void TA2_N_IRQHandler(void);
//
//// Note: Technically, we don't need to declare the interrupt handlers in the header
//// as they are system-level functions already declared in the startup file.
//// However, including them here can help with documentation.
//
//#endif /* SOUND_DETECTION_H_ */
