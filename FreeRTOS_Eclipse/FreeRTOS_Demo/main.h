/*
 * main.h
 *
 *  Created on: Jul 14, 2025
 *      Author: LightStore
 */

#ifndef SOURCE_MAIN_H_
#define SOURCE_MAIN_H_

typedef enum
{
 OFF,LOW,MEDIUM,HIGH,HEATER_MAX_LEVEL,ERROR
}heater_level;

EventGroupHandle_t xButtonEventGroup;

xSemaphoreHandle xADC0Mutex;
xSemaphoreHandle xUART0Mutex;

TaskHandle_t xRuntimeMeasurementsTaskHandle;
TaskHandle_t xCheckPassengerTempTaskHandle;
TaskHandle_t xCheckDriverTempTaskHandle;
TaskHandle_t xDisplayPassengerStatusTaskHandle;
TaskHandle_t xDisplayDriverStatusTaskHandle;
TaskHandle_t xPassengerHeaterSetLevelTaskHandle;
TaskHandle_t xDriverHeaterSetLevelTaskHandle;
TaskHandle_t xControlPassengerHeaterTaskHandle;
TaskHandle_t xControlDriverHeaterTaskHandle;




void vCheckSeatTemp(void * pvParameters);
void vHeaterSetLevel(void * pvParameters);
void vControlHeater(void * pvParameters);
void vDisplayStatus(void * pvParameters);
void vRunTimeMeasurementsTask(void *pvParameters);
void ExternalButton_Handler(void);
void BuiltInButtons_Handler(void);


#endif /* SOURCE_MAIN_H_ */
