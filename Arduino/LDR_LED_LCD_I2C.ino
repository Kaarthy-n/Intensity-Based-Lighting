#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Create an LCD object with the I2C address (0x27 is a common default)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
#define LDR_PIN A0      // LDR connected to analog pin A0
#define LED_PIN 13      // LED connected to digital pin 13

// Threshold value for the LDR
#define LDR_THRESHOLD 512  // Adjust based on your requirements

void setup() {
    // Initialize LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Initialize the LCD
    lcd.init();            // Initialize the LCD
    lcd.backlight();       // Turn on the backlight
    lcd.setCursor(0, 0);   // Set cursor to the first row
    lcd.print("LDR Value:"); // Display static text

    // Initialize Serial Monitor (optional for debugging)
    Serial.begin(9600);
}

void loop() {
    // Read the LDR value
    int ldrValue = analogRead(LDR_PIN);

    // Control LED based on threshold
    if (ldrValue < LDR_THRESHOLD) {
        digitalWrite(LED_PIN, HIGH); // Turn on LED
    } else {
        digitalWrite(LED_PIN, LOW);  // Turn off LED
    }

    // Display the LDR value on the LCD
    lcd.setCursor(0, 1);         // Move to the second row
    lcd.print("Value: ");        // Label
    lcd.print(ldrValue);         // Display ADC value
    lcd.print("   ");            // Clear extra characters if needed

    // Print ADC value to Serial Monitor (optional)
    Serial.println(ldrValue);

    delay(500); // Small delay for stability
}
