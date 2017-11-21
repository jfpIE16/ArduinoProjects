#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;

byte Year;
byte Month;
byte Date;
byte DoW;
byte Hour;
byte Minute;
byte Second;

bool Century=false;
bool h12;
bool PM;

void setup() {
  // Start the serial port
  Serial.begin(57600);
  Wire.begin();
  Clock.setClockMode(false);  // set to 24h
  Clock.setYear(17);
  Clock.setMonth(11);
  Clock.setDate(9);
  Clock.setDoW(4);
  Clock.setHour(11);
  Clock.setMinute(10);
  Clock.setSecond(0);
}

void loop() {

    for (int i=0; i<5; i++){
        delay(1000);
        Serial.print(Clock.getYear(), DEC);
        Serial.print("-");
        Serial.print(Clock.getMonth(Century), DEC);
        Serial.print("-");
        Serial.print(Clock.getDate(), DEC);
        Serial.print(" ");
        Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
        Serial.print(":");
        Serial.print(Clock.getMinute(), DEC);
        Serial.print(":");
        Serial.println(Clock.getSecond(), DEC);
    }

  
  delay(1000);
}
