// Global variables to be used for reading and updating digital input pin statuses
volatile int g_ReadDIpinSts;  // Represents real-time status of digital input pins
volatile int g_AppDIpinSts;   // Represents the application status of digital input pins

// Array to track the consistent count of each digital input pin
static int pin_consistent_count[8] = {0};

// Function prototype
int ISR_DIsampling(void) {
    // Loop through each bit in the global read status variable
    for (int i = 0; i < 8; i++) {
        // Extract the current status of the i-th pin
        int current_pin_status = (g_ReadDIpinSts >> i) & 1;
        
        // Extract the last known status of the i-th pin from the application status variable
        int last_known_status = (g_AppDIpinSts >> i) & 1;
        
        // Check if the current pin status matches the last known status
        if (current_pin_status == last_known_status) {
            // Increment the consistent count for this pin
            if (pin_consistent_count[i] < 10) {
                pin_consistent_count[i]++;
            }
        } else {
            // Reset the consistent count for this pin
            pin_consistent_count[i] = 0;
        }
        
        // If the consistent count reaches 10, update the application status for this pin
        if (pin_consistent_count[i] >= 10) {
            if (current_pin_status) {
                g_AppDIpinSts |= (1 << i);  // Set the bit if the pin is high
            } else {
                g_AppDIpinSts &= ~(1 << i); // Clear the bit if the pin is low
            }
            // Reset the consistent count after updating the status
            pin_consistent_count[i] = 0;
        }
    }                               
    return 0; // Return 0 to indicate successful execution
}
