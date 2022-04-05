#include <SoftwareSerial.h>
#include <dht.h>
#define DHT11_PIN 8
#define RX 10
#define TX 11

dht abcd;
String AP = "Harikp";       // CHANGE ME
String PASS = "harikp@2701"; // CHANGE ME
String API = "4D0GPWWMV8BQ1VZ4";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field ="Field 1";

  
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor1 = 1;
int valSensor2=1;
SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
void loop() {
 valSensor1 = getSensorData();
 Serial.println(valSensor1);
 Serial.println(String(valSensor1));
 String getData = "GET /update?api_key="+ API +"&"+ field  +"="+String(valSensor1);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 valSensor2 = _getSensorData_();
 Serial.println(valSensor2);
 Serial.println(String(valSensor2));
 String _getData_ = "GET /update?api_key="+ API +"&"+ field  +"="+String(valSensor2);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 
}
int getSensorData(){

  
  int chk = abcd.read11(DHT11_PIN);
  int chk1=abcd.temperature;
  Serial.println(chk1);
  Serial.println(abcd.temperature);
  return abcd.temperature;
  
  }
 

int _getSensorData_(){
  int chk = abcd.read11(DHT11_PIN);
  Serial.println(abcd.humidity);
  return abcd.humidity;
}  

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("success");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = true;
 }
