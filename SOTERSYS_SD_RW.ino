#include <Adafruit_Fingerprint.h>   //Librería para sensor de huella
#include <SoftwareSerial.h>       //Comunicación serial por software
#include <SD.h>                    //Librería para uso de la microSD
#include <SPI.h>                   //Controlador del bus de la microSD
#include <Wire.h>                   //Controlar bus I2C
#include <DS3231.h>
#include "U8glib.h"
//Variables y métodos para interactuar con el sensor
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);                    //Pantalla
DS3231 Clock();                                                //Reloj 
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

void setup() 
{
  Wire.begin();
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  while(!Serial);
  Serial.begin(115200);
  Serial.println("Prueba 2, biometrico y creación de ficheros");
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

}

void loop() 
{
  int id = getFingerprintIDs();
  if(id != -1)
  {
    digitalWrite(5, HIGH);
    Serial.print("Se encontro ID #");
    Serial.print(finger.fingerID);
    Serial.println();
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
  }
  delay(200);
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
    digitalWrite(6, HIGH);
    return -1;
  }
  
  return finger.fingerID;
}


