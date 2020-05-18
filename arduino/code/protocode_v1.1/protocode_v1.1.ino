#include <SPI.h>
#include <Wire.h>

#include "SparkFunCCS811.h"
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define CCS811_ADDR 0x5B //Default I2C Address
#define DHTTYPE DHT11   // DHT 11

CCS811 mySensor(CCS811_ADDR);

DHT dht(DHTPIN, DHTTYPE);
char* output_buffer = "";

void setup()
{
//  // set up the LCD's number of columns and rows:
//  lcd.begin(16, 2);
//
//  lcd.print("BOOTING...");
  Wire.begin(); //Inialize I2C Harware
  Serial.begin(9600);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3c for 128x64
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
  // Clear the display buffer
  display.clearDisplay();
  displaySTR("BOOTING...\n");
  dht.begin();
  
  //It is recommended to check return status on .begin(), but it is not
  //required.
  CCS811Core::status returnCode = mySensor.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println(".begin() returned with an error.");
    while (1); //Hang if there was a problem.
  }
  //lcd.print("SUCCESS!");
 Serial.println("temp,humidity,heat_index,");
}

void loop()
{
   output_buffer = "";
   // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      //Serial.println(F("Failed to read from DHT sensor!"));
      //return;
     Serial.print(",,,");
    }
    else{
      // Compute heat index in Fahrenheit (the default)
      //float hif = dht.computeHeatIndex(f, h);
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);

//      //------ DISPLAY 1  (option 1) ----------
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print(" RH | TMP | HI ");
//      lcd.setCursor(0, 1);
//      
//      lcd.print((int)round(h));
//      lcd.print("% |");
//      lcd.print((int)round(t));
//      lcd.print(" C |");
//      lcd.print((int)round(hic));
//      lcd.print(" C");

      Serial.print((int)round(h));
      Serial.print(",");
      Serial.print((int)round(t));
      Serial.print(",");
      Serial.print((int)round(hic));
      Serial.print(",");


//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print("RH: ");
//      lcd.print((int)round(h));
//      lcd.print("%");
//
//      lcd.setCursor(0, 1);
//      
//      lcd.print("TEMP/HI: ");
//      lcd.print((int)round(t));
//      lcd.print("/");
//      lcd.print((int)round(hic));
//      lcd.print(" C");
    }
  //Check to see if data is ready with .dataAvailable()
  if (mySensor.dataAvailable())
  {

    mySensor.readAlgorithmResults();
//    //------ DISPLAY 2 (option 2) ----------
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("CO2: ");
//    //Returns calculated CO2 reading
//    lcd.print(mySensor.getCO2());
//    lcd.print(" ppm");
//    lcd.setCursor(0, 1);
//    lcd.print("tVOC: ");
//    //Returns calculated TVOC reading
//    lcd.print(mySensor.getTVOC());
//    lcd.print(" ug/m^3");

    //Returns calculated CO2 reading
    Serial.print(mySensor.getCO2());
    Serial.print(",");
    Serial.print(mySensor.getTVOC());
  }
  else
  {
    Serial.print(",");
  }
   Serial.println("");

  sprintf(output_buffer,"Temp: %i\nRH: %i\nCO2: %i\nVOC: %i",(int)round(t),(int)round(h),(int)round(mySensor.getCO2()),(int)round(mySensor.getTVOC()));
  displaySTR(output_buffer);
   
  delay(2000-250); //Don't spam the I2C bus
}


void displaySTR(String text) {
  display.clearDisplay();
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(text);

  display.display();
  delay(2000);
}
