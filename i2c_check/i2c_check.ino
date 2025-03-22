#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address (0x27 or 0x3F are common addresses)
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 LCD

void setup() {
    lcd.init();            // Initialize the LCD
    lcd.backlight();       // Turn on the backlight

    lcd.setCursor(0, 0);   // Set cursor to first column, first row
    lcd.print("Hello, World!"); // Print text on LCD

    lcd.setCursor(0, 1);   // Set cursor to first column, second row
    lcd.print("Arduino & I2C");
}

void loop() {
    // No loop code needed for static text display
}
