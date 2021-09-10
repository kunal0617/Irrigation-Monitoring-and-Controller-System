//nodemcu
#include <SoftwareSerial.h>
SoftwareSerial nodemcu(2,3);
long int data;
String cdata;

//temp sensor
#include <dht.h>
#define dht_apin A2
dht DHT;
int temp;
int humid;

//lcd display
#include <LiquidCrystal.h>
int rs=5, en=6, d4=10, d5=9, d6=8, d7=7;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

//motor
#define CW 12 //CW is defined as pin #12
#define CCW 13 //CCW is defined as pin #13
String motor_state;

//rain sensor
const int capteur_D = 11;
const int capteur_A = A0;
int val_analogique;

//soil moisture
int sensorPin = A1;
int digitalPin = 4;
int SensorValue;
int limit = 800;

void setup()
{
  Serial.begin(9600);
  nodemcu.begin(9600);
  pinMode(capteur_D, INPUT);
  pinMode(capteur_A, INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(digitalPin, INPUT);
  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  lcd.begin(16, 2);
  //wait before accessing sensor
  delay(1000);
}

void loop()
{
  Serial.print("NodeMCU: ");
  Serial.println(nodemcu.available());
  Serial.println(nodemcu.parseInt());
  
  if(nodemcu.available() == 0)
  {
    //rain sensor
    Serial.print("RainSensor");
    if(digitalRead(capteur_D) == LOW)
    {
      Serial.print("\t");
      Serial.print("Digital value: wet");
      delay(10);
    }
    else
    {
      Serial.print("\t");
      Serial.print("Digital value: dry");
      delay(10);
    }
    
    val_analogique = analogRead(capteur_A);
    Serial.print("\t");
    Serial.print("Analog value: ");
    Serial.println(val_analogique);
    
    //soil moisture
    Serial.print("Soil Moisture: ");
    Serial.print("\t");
    sensorValue = analogRead(sensorPin);
    
    if(sensorValue < limit)
    {
      Serial.print("\t");
      Serial.print("Digital value: wet");
      delay(10);
    }
    else
    {
      Serial.print("\t");
      Serial.print("Digital value: dry");
      delay(10);
    }
    Serial.print("\t");
    Serial.print("Analog value: ");
    Serial.println(sensorValue);
    
    //temp sensor
    Serial.print("Temp & Humidity sensor: ");
    Serial.print("\t");
    DHT.read11(dht_apin);
    temp = DHT.temperature;
    humid = DHT.humidity;
    Serial.print("Current Humidity = ");
    Serial.print(humid);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(temp);
    Serial.println("C  ");
    
    //nodemcu
    cdata = cdata+sensorValue+","+temp+","+humid;
    Serial.println(cdata);
    nodemcu.println(cdata);
    
    //motor
    if(digitalRead(capteur_D) == HIGH && sensorValue > limit)
    {
      Serial.println("Motor ON");
      lcd.print("Motor");
      lcd.setCursor(0,9);
      lcd.print("ON");
      delay(1000);
      
      //motor runs clockwise
      digitalWrite(CW, HIGH);
      delay(2000);
      //Motor Stops
      digitalWrite(CCW, LOW);
      lcd.clear();
    }
    else
    {
      lcd.print("Motor");
      lcd.setCursor(0,9);
      lcd.print("OFF");
      delay(1000);
      Serial.println("Motor OFF");
      digitalWrite(CW, LOW);
      lcd.clear();
    }
    
    //lcd
    lcd.setCursor(0,0);
    lcd.print("Raining");
    lcd.setCursor(9,0);
    
    if(digitalRead(capteur_D) == HIGH)
    {
      lcd.print("NO");
    }
    else
    {
      lcd.print("YES");
    }
    
    lcd.setCursor(0,1);
    lcd.print("Moisture");
    lcd.setCursor(9, 1);
    lcd.print(sensorValue);
    delay(2000);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp");
    lcd.setCursor(9,0);
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print("Humidity");
    lcd.setCursor(9,1);
    lcd.print(humid);
    delay(2000);
    lcd.clear();
    delay(1000);
    cdata = "";
  }
  
  if(nodemcu.available() > 0)
  {
    data = nodemcu.parseInt();
    delay(100);
    Serial.println(data);
    
    if(data==11)
    {
      //motor
      lcd.print("Motor");
      lcd.setCursor(0,7);
      lcd.print("Forced ON");
      delay(1000);
      lcd.clear();
      digitalWrite(CW, HIGH);
      delay(10000);
      digitalWrite(CCW, LOW);
    }
    
    if(data==10)
    {
      lcd.print("Motor");
      lcd.setCursor(0,7);
      lcd.print("Forced OFF");
      delay(1000);
      lcd.clear();
      digitalWrite(CW, LOW);
      delay(10000);
    }
  }
  
  
}
