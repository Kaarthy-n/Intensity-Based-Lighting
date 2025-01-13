#include <msp430.h>

// Define pins for the LED, ADC input, and 7-segment display
#define LED_PIN BIT0       // LED connected to P1.0
#define LDR_PIN INCH_0     // LDR connected to ADC channel A0
#define SEGMENT_PORT P2OUT // Port 2 for 7-segment display
#define SEGMENT_DIR P2DIR  // Direction register for 7-segment display

// Threshold value for LDR
#define LDR_THRESHOLD 512  // Adjust based on your requirements

// Segment mappings for digits 0-9
const unsigned char segment_map[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Function prototypes
void setupADC();
unsigned int readADC();
void displayNumber(unsigned int value);
void delay(unsigned int ms);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

    // Setup LED
    P1DIR |= LED_PIN;          // Set LED pin as output
    P1OUT &= ~LED_PIN;         // Turn off LED initially

    // Setup ADC
    setupADC();

    // Setup 7-segment display
    SEGMENT_DIR = 0xFF;        // Set entire port as output
    SEGMENT_PORT = 0x00;       // Clear display initially

    while (1) {
        // Read LDR value
        unsigned int adcValue = readADC();

        // Control LED based on threshold
        if (adcValue > LDR_THRESHOLD) {
            P1OUT |= LED_PIN;  // Turn on LED
        } else {
            P1OUT &= ~LED_PIN; // Turn off LED
        }

        // Display ADC value on 7-segment display
        displayNumber(adcValue);

        // Small delay for stability
        delay(500);
    }
}

void setupADC() {
    ADC10CTL1 = LDR_PIN;      // Select input channel
    ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON; // Configure ADC settings
}

unsigned int readADC() {
    ADC10CTL0 |= ENC | ADC10SC; // Enable and start conversion
    while (ADC10CTL1 & ADC10BUSY); // Wait for conversion to complete
    return ADC10MEM;             // Return ADC value
}

void displayNumber(unsigned int value) {
    // Display each digit of the 3-digit number
    unsigned char hundreds = (value / 100) % 10;
    unsigned char tens = (value / 10) % 10;
    unsigned char ones = value % 10;

    // Display hundreds place
    SEGMENT_PORT = segment_map[hundreds];
    delay(5);

    // Display tens place
    SEGMENT_PORT = segment_map[tens];
    delay(5);

    // Display ones place
    SEGMENT_PORT = segment_map[ones];
    delay(5);
}

void delay(unsigned int ms) {
    while (ms--) {
        __delay_cycles(1000); // Assumes 1 MHz clock, adjust if needed
    }
}
