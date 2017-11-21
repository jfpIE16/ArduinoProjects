#include <Adafruit_Fingerprint.h>   //Librería para sensor de huella
#include <SoftwareSerial.h>       //Comunicación serial por software
#include <SD.h>                    //Librería para uso de la microSD
#include <SPI.h>                   //Controlador del bus de la microSD
#include <Wire.h>                   //Controlar bus I2C
#include <DS3231.h>
#include "U8glib.h"
//Variables y métodos para interactuar con el sensor
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);                    //Pantalla
DS3231 Clock;                                                  //Reloj 
SoftwareSerial mySerial(2, 3);                                //Rx, Tx
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);  //Biometrico
int getFingerprintIDs();                                        
//Slave select para MicroSD
int CS_PIN = 10;
//Manejo de archivos
File file;
String nombrearchivo = "";
char tempF[16] = "";
int i = 2025;
bool Century = false;
char line[30];
bool h12;
bool PM;


void setup() 
{
  Wire.begin();
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  while(!Serial);
  Serial.begin(115200);
  Serial.println("Prueba 3, biometrico, creación de ficheros, reloj y pantalla");
  finger.begin(57600);
  initializeSD();
  if(finger.verifyPassword())
  {
    Serial.println("Se encontro lector de huellas");
  }else
  {
    Serial.println("No se encontro el lector de huellas");
  }
  Serial.println("Ingrese su huella");

  //Ajustar pantalla
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

}

void drawOK(void)
{
  u8g.setFont(u8g_font_7x13);
  u8g.drawStr(20,20, "OK!");
}

void drawNOK(void)
{
  u8g.setFont(u8g_font_7x13);
  u8g.drawStr(13,20, "Denied!");
}

void draw(void)
{
  u8g.setFont(u8g_font_7x13);
  u8g.drawStr(10,20, "Ingrese huella...");
}

  
void loop() 
{
  Clock.setClockMode(false);
  int id = getFingerprintIDs();
  if(id != -1 and id != -2)
  {
    digitalWrite(5, HIGH);
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
    nombrearchivo = String(i)+".txt";
    nombrearchivo.trim();
    nombrearchivo.toCharArray(tempF, sizeof(tempF));
    file = SD.open(tempF, FILE_WRITE);
    if(file)
    {
      file.println("ID:Hora:Fecha");
      file.close();
    }else
    {
      Serial.println("Error con el archivo  ");
    }
    i++;
    Serial.println("Ponga su dedo en el lector");
      u8g.firstPage();  
      do {
        drawOK();
      } while( u8g.nextPage() );
      delay(1000);
  }else if(id == -2)
  {
    digitalWrite(6, HIGH);
    u8g.firstPage();  
      do {
        drawNOK();
      } while( u8g.nextPage() );
      delay(1000);
  }
      u8g.firstPage();  
      do {
        draw();
      } while( u8g.nextPage() );
  delay(300);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}


int getFingerprintIDs()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
  {
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  {
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    return -2;
  }
  
  return finger.fingerID;
}


