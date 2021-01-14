#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

//INFORMATIONS DE CONNEXION

  //connexion wifi
  const char ssid [] = "Tel DD";
  const char password [] = "Jesaispas";

  //connexion au broker mqtt
  const char* brokerUser = "corentin.latimier17@gmail.com";
  const char* brokerPass = "0be2523e";
  const char* broker ="mqtt.dioty.co";

    //definition des topics mqtt
    const char* mqqtTopicTemp="/corentin.latimier17@gmail.com/temp";
    const char* mqqtTopicHumidity="/corentin.latimier17@gmail.com/humidity";
    const char* mqqtTopicalive ="/corentin.latimier17@gmail.com/alive";
    const char* mqttTopicRFID="/corentin.latimier17@gmail.com/RFID";
    const char* mqttTopicIn="/corentin.latimier17@gmail.com/IN";
    const char* mqttTopictwitter="/corentin.latimier17@gmail.com/twitter";

WiFiClient wificlient;
MQTTClient client;


//définition des variables 
long last_time=0;


/**************** 
 * @param topic: le topic sur lequel on recoit les messages 
 * @param payload : contenu du message
 * 
 *transfert le message recu depuis mqtt à atmega
 * ******************/
void messageReceived(String &topic, String &payload) {
  Serial.println(payload); //on envoie le message à l'atmega
}


/**************** 
 *transfert les datas recues de l'atmega vers le channel mqtt
 * ******************/
String inString="";
void SerialEvent() {
  while (Serial.available()) {

    char inChar = Serial.read();
    inString = inString+inChar;
    
    if (inChar == ']') { // ] = caractère de fin de commande
      if (inString.indexOf("Humidity#")>0) {
        client.publish(mqqtTopicHumidity,inString);
      }
      else if (inString.indexOf("Temp#")>0) {
        client.publish(mqqtTopicTemp,inString);
      }
      else if(inString.indexOf("RFID#")>0){
        client.publish(mqttTopicRFID,inString);
      }
      else if(inString.indexOf("twitter#")>0){
        client.publish(mqttTopictwitter,inString);
      }
      else 
      {
        Serial.println("ESP: Problème commande inconnue arrive depuis l'ESP"); // s'affiche sur le monitor de debug
      }
      
      inString = "";//reset de la string jusqu'à la prochaine commande
    }
  }
}


/****************  
 *vérifie la connexion de l'esp à internet et se connecte à internet si la connexion est rompue
 * ******************/
void connect(){

  while(WiFi.status()!=WL_CONNECTED){
    delay(100); 
  }

  Serial.println();
  Serial.print("ESP Connexion reussie à ");
  Serial.print(ssid);
}



/****************  
 *vérifie la connexion de l'esp au broker mqtt et se reconnecte si la connexion est perdue 
 * ******************/
void reconnect(){
  while(!client.connected()){
    Serial.print("ESP: Connecting to: ");
    Serial.println(broker);
    if(client.connect("CORENTINL",brokerUser,brokerPass)){
        Serial.print("ESP Connected to: ");
        Serial.println(broker);
        client.subscribe(mqttTopicIn);
    }
    else{
      Serial.print("Trying to connect to : ");
      Serial.println(broker);
      delay(6000);
    }
  }
}


void setup(){
  Serial.begin(9600); //communication vers atmega
  //mode de connexion
  WiFi.mode(WIFI_STA); //le esp est en mode station
  WiFi.begin(ssid,password);
  connect();
  client.begin(broker,1883,wificlient);
  client.onMessage(messageReceived);
  reconnect();
}

void loop(){

  client.loop();
  
  if (WiFi.status()!=WL_CONNECTED) {
    Serial.println("ESP Connexion perdue / Tentative de reconnexion");
    connect();
  }

  if(!client.connected()){
    reconnect();
  }
  
  SerialEvent();
}




