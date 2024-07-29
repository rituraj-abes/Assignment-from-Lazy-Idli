#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// Global variables
int G_DataID = 0;
int G_DataValue = 0;

// Definition of Data_t
typedef struct {
    uint8_t dataID;
    int32_t DataValue;
} Data_t;

// Queue handle
QueueHandle_t Queue1;

// Task handles
TaskHandle_t TaskHandle_1 ;
TaskHandle_t TaskHandle_2 ;

int originalPriority = 1;

// Function prototypes
void ExampleTask1(void *pV);
void ExampleTask2(void *pV);

void setup() {
    Serial.begin(9600);

    // Create the queue
    Queue1 = xQueueCreate(5, sizeof(Data_t));
    if (Queue1 == NULL) {
        Serial.println("Queue creation failed");
        while (1); // Halt if queue creation fails
    }

    // Create tasks
    xTaskCreate(ExampleTask1, "Task1", 128, NULL, originalPriority, &TaskHandle_1);
    xTaskCreate(ExampleTask2, "Task2", 128, NULL, originalPriority, &TaskHandle_2);
}

void loop() {
    // The loop is empty because FreeRTOS handles tasks
}

// Task to send data to the queue
void ExampleTask1(void *pV) {
    Data_t data;
    
    while (1) {
        data.dataID = G_DataID;
        data.DataValue = G_DataValue;
        // if (xQueueSend(Queue1, &data, portMAX_DELAY) != pdPASS) {
            // Serial.println("Failed to send data to the queue");
        // }
        
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay of 500ms
    }
}

// Task to process data from the queue
void ExampleTask2(void *pV) {
    Data_t receivedData;
    while (1) {
        if (xQueueReceive(Queue1, &receivedData, portMAX_DELAY) == pdPASS) {
            Serial.print("Received dataID: ");
            Serial.print(receivedData.dataID);
            Serial.print(", DataValue: ");
            Serial.println(receivedData.DataValue);

            if(receivedData.dataID == 0)
                vTaskDelete(TaskHandle_2);
            else if(receivedData.dataID == 1){
                if (receivedData.DataValue == 0)
                    vTaskPrioritySet(NULL, originalPriority + 2); // Increase priority
                else if (receivedData.DataValue == 1)
                    vTaskPrioritySet(NULL, originalPriority); // Restore original priority
                else if (receivedData.DataValue == 2)
                    vTaskDelete(TaskHandle_2); // Delete this task
            }
           
        }
    }
}
