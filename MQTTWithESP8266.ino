#define MY_WIFI_NAME "YourWiFiName"
#define MY_WIFI_PASS "YourWiFiPass"

#define mqtt_server "test.mosquitto.org"
#define mqtt_port 1883
#define mqtt_user "TEST" // up to you
#define mqtt_password "12345" // up to you 
#define mqtt_name "esp8266"

#define SUBSCRIBE_TOPIC ""
#define PUBLISH_TOPIC ""

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

const char* ssid = MY_WIFI_NAME;
const char* pass = MY_WIFI_PASS;

String buf[10];
int buf_index = -1;

WiFiClient client;
PubSubClient mqtt(client);

void callback(char* topic, byte* payload, int length) 
{
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  
  String msg = "";
  
  for(int i = 0; i < length; i++)
    msg += (char)payload[i];
    
  Serial.println(msg)
}
void reconnect() {
  
  while ( !mqtt.connected() ) 
  {
    Serial.println("Reconnecting...");
    
    if( mqtt.connect(mqtt_name, mqtt_user, mqtt_password) ) 
    {
      Serial.println("\nConnected...");
      mqtt.subscribe(SUBSCRIBE_TOPIC);
    } 
    else 
    {
      Serial.println("\nConnection failed... try again in 5 seconds") ;
      delay(5000);
    }
  }
   
}
void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);

}

void loop() 
{
  // put your main code here, to run repeatedly:
  if( !mqtt.connected() )
    reconnect();

  if( mqtt.connected() ) 
  {
    
    if ( buf_index > -1 ) 
    {
      unsigned int len = buf[0].length;
      char msg[len + 1];
      buf[0].toCharArray(msg, len);
      
      if( mqtt.connected() ) 
      {
        mqtt.publish(PUBLISH_TOPIC, msg);
        Serial.println("Send : " + (String)msg);
        buf_index--;
      }
      
    }
    
    if ( Serial.available() > 0 ) 
    {
      buf_index++;
      buf[buf_index] = Serial.readStringUntil("\n");
    }
    
  }
  
}
