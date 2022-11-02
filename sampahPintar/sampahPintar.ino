//inisialisasi komunikasi serial dengan nodoMCU
#include <SoftwareSerial.h>
SoftwareSerial s(5,6); // (Rx, Tx)

#include "HX711.h"
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A0;
const int LOADCELL_SCK_PIN = A1;
float calibration_factor = 4000; //nilai kalibrasi awal

//LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display //kalau bukan 0x27 0x3F

HX711 scale;

float berat =0;

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  Serial.println("Menunggu...");
  delay(5000);
  Serial.println("Selesai Menunggu...");
  scale.set_scale();
  scale.tare(); // auto zero / mengenolkan pembacaan berat
  Serial.println("Selesai tare...");
  
  long zero_factor = scale.read_average(); //membaca nilai output sensor saat tidak ada beban
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);

  //LCD
  lcd.init();
  lcd.init();
  lcd.backlight();
  Serial.println("Setup Ready");
}

void loop() {
//  //jika ada komunikaasi dari nodemcu
//  if(s.available()>0)
//{
//}

  //koding kalibrasi
   scale.set_scale(calibration_factor); //sesuaikan hasil pembacaan dengan nilai kalibrasi

  Serial.print("Berat: ");
  Serial.print(scale.get_units(), 1);
  Serial.print(" gram");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
 
  

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
      else if(temp == '-' || temp == 's')
      calibration_factor += 100;
      else if(temp == '-' || temp == 'd')
      calibration_factor += 1000;
      else if(temp == '-' || temp == 'x')
      calibration_factor -= 100;
      else if(temp == '-' || temp == 'c')
      calibration_factor -= 1000;
  }

//main coding


  //if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
    
//  } else {
//    Serial.println("HX711 tidak di temukan / PIN Salah");
//  }

  delay(5000);


  
//LCD
    lcd.setCursor(0,0);
    lcd.print("Sampah Pintar");
    lcd.setCursor(0,1);
    lcd.print("Berat: ");
    lcd.setCursor(7,1);
    lcd.print(scale.get_units(),1); 

  //megnirimd ata berat secara serial ke nodeMCU
  s.write(scale.get_units());
}
