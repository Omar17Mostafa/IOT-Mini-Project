#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wificlient;
PubSubClient client(wificlient);

#define ssid "Omar 17"
#define pass "111112223"
#define LED_GREEN 14
#define LED_RED 23
#define BTN_PIN 0

long long IR_TIME ;
char message[100];

void callback(char topic[], byte*payload, unsigned int length)
{
   Serial.println("topic: ");
   Serial.println(topic);

   Serial.println("message: ");
   for(int i=0; i<length ;i++)
   {
   Serial.print((char)payload[i]);
   message[i]=(char)payload[i];
   }
   message[length]='\0';
   Serial.println();
  if(strcmp(message,"on")){
digitalWrite(LED_GREEN,LOW);
Serial.println("led ON");
  }
else if(strcmp(message,"off")){
digitalWrite(LED_GREEN,HIGH);
Serial.println("led OFF");

}


}

void setup() {
pinMode(LED_GREEN,OUTPUT);  
pinMode(LED_RED,OUTPUT);  
pinMode(BTN_PIN,INPUT);
WiFi.begin(ssid,pass);
Serial.begin(9600);
while(WiFi.status()!=WL_CONNECTED){
  Serial.println("Connecting to WiFi");
  Serial.println(WiFi.status());
  delay(500);
}
 Serial.println("Connected to WiFi");
 Serial.println(WiFi.localIP());

client.setServer("broker.emqx.io",1883);
client.setCallback(callback);
while(!client.connect("omarmmomarom")){
  Serial.println("Connecting to broker");
  delay(500);
}
 Serial.println("Connected to broker");

 while(!client.subscribe("/OPERA/omar/led")){
  Serial.println("Subscribing to /OPERA/omar/led ");
  delay(500);
 }
Serial.println("Subscribed to /OPERA/omar/led ");
IR_TIME=millis();
}


void loop() {
  client.loop();
  if(millis()-IR_TIME>=250){
if(digitalRead(BTN_PIN)==LOW){
  client.publish("/sectorb5/omar/led","on");
    Serial.println("IR_Sensor on");
    digitalWrite(LED_RED,HIGH);
  }
else if(digitalRead(BTN_PIN)==HIGH){
  client.publish("/sectorb5/omar/led","off");
    Serial.println("IR_Sensor off");
    digitalWrite(LED_RED,LOW);
  }
  IR_TIME=millis();
}

}

