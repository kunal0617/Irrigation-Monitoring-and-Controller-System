#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int pinValue1;

char auth[] = "auth_token";
char ssid[] = "Redmi";
char pass[] = "1234567890@";

BlynkTimer timer;

String myString;
char rdata;

int firstVal, secondVal, thirdVal;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sensorvalue1);
  timer.setInterval(1000L, sensorvalue2);
  timer.setInterval(1000L, sensorvalue3);
}

void loop()
{
  if(Serial.available()==0)
  {
    Blynk.run();
    timer.run();
  }
  
  if(Serial.available() > 0)
  {
    rdata = Serial.read();
    myString = myString+rdata;
    
    //Serial.println(myString)
    
    if(rdata=='\n')
    {
      String l = getValue(myString, ',', 0);
      String m = getValue(myString, ',', 1);
      String n = getValue(myString, ',', 2);
      
      firstVal = l.toInt();
      secondVal = m.toInt();
      thirdVal = n.toInt();
      
      myString = "";
    }
  }
}

void sensorvalue1()
{
  int sdata = firstVal;
  Blynk.virtualWrite(V2, sdata);
}

void sensorvalue1()
{
  int sdata = secondVal;
  Blynk.virtualWrite(V3, sdata);
}

void sensorvalue1()
{
  int sdata = thirdVal;
  Blynk.virtualWrite(V4, sdata);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  
  for(int i=0; i<=maxIndex && found<=index; i++)
  {
    if(data.charAt(i) == separator || i==maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i==maxIndex) ? i+1: i;
    }
  }
  
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

BLYNK_WRITE(V10)
{
  //assigning value from pin V10 to variable
  pinValue1 = param.asInt();
  
  Serial.print(pinValue1);
}

