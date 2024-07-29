#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Definition of Data_t
typedef struct {
    uint8_t dataID;
    int32_t DataValue;
} Data_t;

// Global Variables
extern uint8_t G_DataID;
extern int32_t G_DataValue;

// Queue Handle
QueueHandle_t Queue1;

// Task Handles
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;

// Task Prototypes
void ExampleTask1(void *pV);
void ExampleTask2(void *pV);

int main(void) {
    // Create the queue
    Queue1 = xQueueCreate(5, sizeof(Data_t));
    
    if (Queue1 != NULL) {
        // Create tasks
        xTaskCreate(ExampleTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &TaskHandle_1);
        xTaskCreate(ExampleTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &TaskHandle_2);
        
        // Start the scheduler
        vTaskStartScheduler();
    } else {
        // Queue was not created and must not be used.
        printf("Failed to create the queue.\n");
    }
    
    // Main should never reach here as the scheduler is running
    for (;;) {}
    return 0;
}

void ExampleTask1(void *pV) {
    Data_t data;
    
    for (;;) {
        // Populate data from global variables
        data.dataID = G_DataID;
        data.DataValue = G_DataValue;
        
        // Send data to the queue
        xQueueSend(Queue1, &data, portMAX_DELAY);
        
        // Print statement
        printf("Task1 sent data: dataID = %d, DataValue = %d\n", data.dataID, data.DataValue);
        
        // Delay for 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void ExampleTask2(void *pV) {
    Data_t data;
    UBaseType_t initialPriority = uxTaskPriorityGet(NULL);
    UBaseType_t currentPriority = initialPriority;
    
    for (;;) {
        // Wait for data to be available in the queue
        if (xQueueReceive(Queue1, &data, portMAX_DELAY) == pdPASS) {
            // Print statement
            printf("Task2 received data: dataID = %d, DataValue = %d\n", data.dataID, data.DataValue);
            
            // Process data according to the specified logic
            if (data.dataID == 0) {
                // Delete ExampleTask2
                vTaskDelete(NULL);
            } else if (data.dataID == 1) {
                // Allow processing of DataValue member
                if (data.DataValue == 0) {
                    // Increase the priority of ExampleTask2 by 2
                    vTaskPrioritySet(NULL, initialPriority + 2);
                    currentPriority = initialPriority + 2;
                } else if (data.DataValue == 1) {
                    // Decrease the priority of ExampleTask2 if previously increased
                    if (currentPriority > initialPriority) {
                        vTaskPrioritySet(NULL, initialPriority);
                        currentPriority = initialPriority;
                    }
                } else if (data.DataValue == 2) {
                    // Delete ExampleTask2
                    vTaskDelete(NULL);
                }
            }
        }
    }
}
