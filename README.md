# Assignment-from-Lazy-Idli

# Vehicle Control Architecture and FreeRTOS Tasks

## Project Overview

This repository contains solutions to two problem statements related to a vehicle control architecture and FreeRTOS tasks. The first problem involves writing an Interrupt Service Routine (ISR) for sampling digital inputs in a vehicle's Electronic Control Unit (ECU). The second problem demonstrates the use of FreeRTOS by creating tasks and queues for data processing.

## Problem Statement 1: ISR for Sampling Digital Inputs

### Description

In a vehicle control architecture, there are multiple Electronic Control Units (ECUs) connected together through a CAN protocol. Each ECU is programmed to take care of specific functionality. MCB is one such ECU in the architecture that performs the following tasks:

- Measure 8-bit digital inputs from multiple switches/sensors in a vehicle.
- Perform defined actions based on the status of digital inputs.
- Communicate with other ECUs through the CAN protocol.

As part of this assignment, the task is to write an Interrupt Service Routine (ISR) in C for sampling the digital inputs. The ISR should perform the following functionality:

- Read the real-time status of digital input pins from a global variable `g_ReadDIpinSts`. Here, each bit in `g_ReadDIpinSts` represents the status of one digital input pin.
- Update the digital input status to a global variable `g_AppDIpinSts`, maintaining the bit position of digital inputs.
- The condition for status update in `g_AppDIpinSts` is: "The pin state of a particular digital input has to be consistent for 10 consecutive ISR calls". Here, ISR is triggered every 100 milliseconds.



## Problem Statement 2:  FreeRTOS Tasks and Queues

### Description
This task involves using FreeRTOS to create two tasks and a queue. Task1 sends data to the queue every 500ms, and Task2 processes the data based on specific conditions.

### Task Prototypes and Handles
The following tasks and their corresponding task handles are considered:
- `void ExampleTask1(void *pV);` - `TaskHandle_t TaskHandle_1;`
- `void ExampleTask2(void *pV);` - `TaskHandle_t TaskHandle_2;`

### Queue and Its Properties
The following queue and its properties are considered:
- **QueueHandle:** `QueueHandle_t Queue1;`
- **Size of Queue:** 5
- **Data Type:** `Data_t`

### Definition of Data_t
```c
typedef struct {
    uint8_t dataID;
    int32_t DataValue;
} Data_t;
```

 ### What is to be done:

- Create Tasks ExampleTask1 and ExampleTask2 as per above prototypes and handles. Priority will be the same and at your descrition.
- Create Queue Queue1 with the above properties mentioned.
- ExampleTask1 sends data to Queue1 at a rate of once every 500ms. The delay between each send needs to be exact. The members of the structure are populated by global variables G_DataID; and G_DataValue; Assume that these variables are updated elsewhere.
- ExampleTask2 takes data from Queue2 whenever data is available, and applies the logic to the data gathered.
