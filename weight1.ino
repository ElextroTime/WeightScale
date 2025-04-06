#include <HX711.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LOADCELL_DT  3   // HX711 Data pin
#define LOADCELL_SCK 2   // HX711 Clock pin
#define TARE_BUTTON  4   // Push button for tare

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

HX711 scale;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float calibration_factor = 108.2; // Adjust this after calibration

void setup() {
    Serial.begin(115200);
    pinMode(TARE_BUTTON, INPUT_PULLUP);

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Weighing...");
    display.display();
    
    // Initialize HX711
    scale.begin(LOADCELL_DT, LOADCELL_SCK);
    scale.set_scale(calibration_factor);  // Set calibration factor
    scale.tare(); // Reset to zero

    Serial.println("Place an object to weigh...");
}

void loop() {
    // Read weight
    float weight = scale.get_units(10);  // Take 10 readings and average
    
    // Tare button function
    if (digitalRead(TARE_BUTTON) == LOW) {
        scale.tare();  // Reset to zero
        Serial.println("Tare done!");
        delay(500);
    }
    
    // Display on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Weight:");
    display.setCursor(0, 40);
    if (weight < 0){
      display.print(0);
    }
    else{
      display.print(weight, 2);
    }
    display.print(" g");
    display.display();
    
    // Print to Serial Monitor
    Serial.print("Weight: ");
    Serial.print(weight, 2);
    Serial.println(" g");
    
    delay(500);
}
