#include <Adafruit_Fingerprint.h> //Librería para sensor de huella
#include <SoftwareSerial.h>     //Comunicación serial por Software

int getFingerprintIDs();

SoftwareSerial mySerial(2, 3);  //Rx, Tx
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  while(!Serial);
  Serial.begin(9600);
  Serial.println("Prueba 1, recolección de huellas");
  finger.begin(57600);
  if(finger.verifyPassword())
  {
    Serial.println("Biometrico encontrado!");
  }else
  {
    Serial.println("No se encontro el sensor...");
    while(1);
  }
  Serial.println("Ponga su dedo en el lector");    

}

void loop() 
{
  getFingerprintIDs();
  delay(500);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
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
  digitalWrite(5, HIGH);
  Serial.print("Se encontro ID #");
  Serial.print(finger.fingerID);
  Serial.print(" con una coincidencia de ");
  Serial.print(finger.confidence);
  Serial.println();
  Serial.println("Ponga su dedo en el lector");
  return finger.fingerID;
}

