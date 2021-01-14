#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MFRC522.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//definition des pin utilisées sur la carte

#define DHTPIN 42 
#define DHTTYPE DHT11 
#define sAudioPin 12
#define greenled 4
#define redled 3
#define relaySET 7
#define relayUNSET 6
#define SS_PIN 9
#define RST_PIN 8
#define OLED_RESET 4


//bitmap affichés sur l'écran

const unsigned char cloud_Bitmap [] PROGMEM = {
	// 'cloud, 22x16px
	0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0xc0, 0xc0, 0x1f, 0xe1, 0xe0, 0x3f, 0xe7, 0xe0, 0x3e, 
	0x0f, 0xf0, 0x01, 0xf7, 0xf8, 0x03, 0xfb, 0xf0, 0x07, 0xfe, 0x00, 0x07, 0xff, 0x80, 0x0f, 0xff, 
	0x80, 0x1f, 0xff, 0xc0, 0x1f, 0xff, 0xe0, 0x1f, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00
};

const unsigned char twitter_logo [] PROGMEM = {
	// 'twitter, 23x23px
	0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0xfc, 0x3e, 0xff, 0xf8, 0x02, 0x9f, 0xf0, 0x02, 0x8f, 
	0xf0, 0x02, 0x83, 0xe0, 0x06, 0x80, 0xe0, 0x06, 0xc0, 0x00, 0x06, 0x80, 0x00, 0x06, 0x80, 0x00, 
	0x0e, 0xc0, 0x00, 0x0e, 0xe0, 0x00, 0x0e, 0xe0, 0x00, 0x1e, 0xe0, 0x00, 0x1e, 0xf0, 0x00, 0x3e, 
	0xf8, 0x00, 0x7e, 0xfc, 0x00, 0xfe, 0xf0, 0x01, 0xfe, 0xc0, 0x07, 0xfe, 0xf0, 0x1f, 0xfe, 0xff, 
	0xff, 0xfe, 0xff, 0xff, 0xfe
};

const unsigned char rainy_Bitmap [] PROGMEM = {
	// 'rainy, 22x16px
	0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x01, 0xf8, 0x00, 0x03, 0xfc, 0x00, 0x03, 0xff, 0x80, 0x07, 
	0xff, 0xc0, 0x0f, 0xff, 0xe0, 0x0f, 0xff, 0xe0, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x02, 0xad, 
	0x00, 0x02, 0xaa, 0x00, 0x01, 0xaa, 0x00, 0x01, 0x5a, 0x00, 0x01, 0x10, 0x00, 0x01, 0x10, 0x00
};

const unsigned char sun_Bitmap [] PROGMEM = {
	// 'sun, 22x16px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x18, 0x80, 0x00, 
	0x81, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x07, 0x7e, 0xe0, 0x00, 0x3e, 0x00, 0x00, 0x99, 
	0x00, 0x01, 0x81, 0x80, 0x00, 0x18, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char snow_Bitmap [] PROGMEM = {
		// 'snow, 16x16px
	0x01, 0x80, 0x0f, 0xf0, 0x2b, 0xdc, 0x39, 0x98, 0x79, 0xbe, 0x4d, 0xa2, 0x63, 0xc6, 0xff, 0xff, 
	0xff, 0xff, 0x63, 0xc6, 0x45, 0xb2, 0x7d, 0x9e, 0x19, 0x9c, 0x3b, 0xd4, 0x0f, 0xf0, 0x01, 0x80
};

const unsigned char humidity_Bitmap [] PROGMEM = {
	// 'humidity, 23x22px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x3c, 0x0c, 0x20, 0x00, 0x0e, 
	0x30, 0x1c, 0x1e, 0x70, 0x00, 0x1f, 0x78, 0x7c, 0x3e, 0xf8, 0x00, 0x3e, 0xfc, 0x1c, 0x7d, 0xfc, 
	0x00, 0x7d, 0xfe, 0x3c, 0x7b, 0xff, 0x00, 0x3b, 0xe3, 0x00, 0x3b, 0xdc, 0x80, 0x07, 0xd5, 0x80, 
	0x07, 0xdf, 0x00, 0x03, 0xe3, 0xc0, 0x03, 0xff, 0xe0, 0x01, 0xf5, 0xe0, 0x00, 0x6c, 0xc0, 0x00, 
	0x00, 0x00
};

const unsigned char wifi_Bitmap [] PROGMEM = {
	// 'wifi_icon, 20x14px
	0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x0f, 0xfe, 0x00, 0x3f, 0xff, 0x00, 0x7e, 0x0f, 0x80, 0x78, 
	0xe3, 0x80, 0x33, 0xf9, 0x80, 0x07, 0xfc, 0x00, 0x0f, 0xfc, 0x00, 0x06, 0x1c, 0x00, 0x00, 0xc0, 
	0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00
};



#define SCREEN_WIDTH 128 // largeur écran OLED, en pixels
#define SCREEN_HEIGHT 64 // hauteur écran OLED, en pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// declarations des fonctions utilisées

/**************** 
 * @param fail: nombre de badges non identifiés badgés
 * @param time_first_fail : nombre de micros-secondes écoulées depuis le début du programme lorsque la première erreur est détectée
 * 
 * reconnaissance d'une intrusion : elle compte le nombre d'erreurs faites en un certain temps : tolérance erreur definie dans les variables fail_securite et time_securite
 * lorsque une erreur est détectée, atmega envoie un message à l'esp8266 via serial 3 qui relaie le message dans le chanel twitter du mqtt  
 * ******************/
void check_intrusion(int * fail,long * time_first_fail);

/**************** 
 * @param humidity: dernier taux d'humidité detecté par dht11
 * @param temp : dernière température détectée par dht11
 * @param connected : en fonction de la valeur (0 ou 1), l'écran affiche un logo wifi ou pas
 * 
 * cette fonction définit l'écran d'entrée de l'écran
 * ******************/
void home(float humidity,float temp,int connected);


/**************** 
 * @param connected: un pointeur qui permet de changer la valeur de connected (0 ou 1)
 * 
 * cette fonction recoit des messages de l'esp et effectue certaines actions en fonction des messages
 * ******************/
void SerialEvent3(int * connected);

/**************** 
 * @param uid_read: un tableau qui stocke l'uid lu sur la carte
 * @return : 1 : carte reconnue / 0 : carte non reconnue
 * cette fonction envoie à l'esp l'uid lu qui le transfert à node red qui effectue une requète à la bdd qui renvoie la réponse à l'esp puis à l'atmega 	: elle permet de checker si l'uid lu est stocké en bdd
 * ******************/
int bdd_recognized(int uid_read[]);

/****************
 * cette fonction ouvre la serrure à solénoïde 
 * **************/
void openLock();

/**************** 
 * @param state: 1 ou 0 en fonction de si la carte est connue ou pas 
 * 
 *la fonction déclenche un son de réussite ou d'échec en fonction de la réponse de la bdd
 * ******************/
void sound(int state);

//definition des variables 
DHT dht11(DHTPIN,DHTTYPE);
MFRC522 mfrc522(SS_PIN,RST_PIN);
int uid_read[4];
long time_securite=60000; 
int fail_securite=3;
long time_first_fail=0;
int fail=0;	
int connected=0;
long lastMillis=0;
String instring;


void setup() {

	Serial.begin(9600); //vers la console
	Serial3.begin(9600); //vers l'esp
	SPI.begin();
	mfrc522.PCD_Init();  
  	dht11.begin();

	//initialisation des pin de led en output
	pinMode(greenled,OUTPUT);
	pinMode(redled,OUTPUT);
	pinMode(relaySET,OUTPUT);
	pinMode(relayUNSET,OUTPUT);


	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
		Serial.println("ATMEGA SSD1306 allocation failed");
		for(;;); // Don't proceed, loop forever
	}
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
}

void loop() {

	float humidity=dht11.readHumidity(); //lecture de l'humidité
	float temp=dht11.readTemperature();  //lecture de la température
	digitalWrite(greenled,LOW); //on eteint les leds
	digitalWrite(redled,LOW);

	if(isnan(humidity) || isnan(temp))  // si problème de lecture du dht11
	{ 
		Serial.println();
		Serial.print("ATMEGA ERROR : DHT11 not readable");
		return;
	}
	else
	{
		home(humidity,temp,connected); //affichage de l'écran d'accueil
		
		if(millis()-lastMillis>10000)  //envoie de données de température et d'humidité à l'esp toutes les 10 secondes
		{ 
			Serial3.print("[Humidity#");
			Serial3.print(humidity);
			Serial3.print("]");
			Serial3.print("[Temp#");
			Serial3.print(temp);
			Serial3.print("]");
			lastMillis=millis();
		}  
	}
	if (mfrc522.PICC_IsNewCardPresent()) { //on  teste si la carte est détectée
		if (mfrc522.PICC_ReadCardSerial()) { //on lit sur la carte

			display.clearDisplay(); //supprime les carctères potentiels sur l'écran
			display.setCursor(45,0);
			display.print("Tag UID");

			display.setCursor(25,20);
			for(int i=0;i<mfrc522.uid.size;i++) //on affiche l'uid sur la carte + on stocke l'uid lu
			{
				uid_read[i]=mfrc522.uid.uidByte[i];
				display.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
				display.print(mfrc522.uid.uidByte[i],HEX);
			}

			mfrc522.PICC_HaltA();
			Serial.println();
			Serial.print("ATMEGA L'UID de la carte détectée est : "); ///on affiche l'uid lu sur la console
			Serial.print(uid_read[0],HEX);
			Serial.print(uid_read[1],HEX);
			Serial.print(uid_read[2],HEX);
			Serial.println(uid_read[3],HEX);
    
			if(bdd_recognized(uid_read)){ //si la carte est reconnue
				fail=0; //on repasse le compteur d'echecs à 0
			}
			else
			{
				check_intrusion(&fail,&time_first_fail); // si erreur alors on appelle check_intrusion qui ré-incrémente le nombre d'erreurs et regarde si on atteint l'état intrusion
			}
		}

		display.display(); //l'écran est ré-actualisé
		delay(3000);
		display.clearDisplay(); 
	} 
	
	SerialEvent3(&connected);// on check si on recoit des messages de l'esp à chaque loop
}

void check_intrusion(int * fail,long * time_first_fail){

	long crnt_time=micros();

	if(*fail==0){
		*fail=*fail+1;
		*time_first_fail=crnt_time;
	}
	else
	{
		if((crnt_time-*time_first_fail)>time_securite || *fail>fail_securite){ ///si la premiere erreur date de plus d'une minute alors on repasse le compteur d'erreur à 0
			*fail=0;
		}
		else
		{
			if(*fail==fail_securite  && (crnt_time-*time_first_fail)<time_securite ){ 	//si on est à plus de 3 erreurs en moins d'une minutes-> notif à l'utilisateur
				Serial3.print("[twitter#"); //envoi d'un message à l'esp qui publie sur le channel twitter
				Serial3.print("Tentative d'entrée non autorisée sur Serrure RFID");
				Serial3.print("]");
				Serial.println();
				display.clearDisplay();
				display.drawBitmap(0,24,twitter_logo,23,23,WHITE);
				display.setCursor(27,29);
				display.print("Tweet securite");
				display.setCursor(29,38);
				display.print("envoye");
				display.display();
				Serial.print("ATMEGA Test notification blocage");
				*fail=*fail+1;
			}
			if(*fail<fail_securite &&  (crnt_time-*time_first_fail)<time_securite){ //si on est à moins d'une minute et moins de 3 erreurs, alors on continue à incrémenter
				*fail=*fail+1;
			}
		}
	}
}


void home(float humidity,float temp,int connected){

	display.clearDisplay();

	if(connected==1){
		display.drawBitmap(0,0,wifi_Bitmap,20,14,WHITE);
	}

	if(temp<0){
		display.drawBitmap(25,0,snow_Bitmap,16,16,WHITE);
		display.setCursor(55,5);
	}
	else if(temp>=0 && temp<15){
		if(humidity>=75){
			display.drawBitmap(25,0,rainy_Bitmap,22,16,WHITE);
			display.setCursor(55,5);
		}
		else{
			display.drawBitmap(30,0,cloud_Bitmap,22,16,WHITE);
			display.setCursor(55,5);
		}
	}
	else
	{
		display.drawBitmap(25,0,sun_Bitmap,22,16,WHITE);
		display.setCursor(50,5);
	}
	
	display.print(temp);
	display.print(" C");

	display.drawBitmap(20,42,humidity_Bitmap,22,23,WHITE);
	display.setCursor(50,50);
	display.print(humidity);
	display.print(" %");

	display.setCursor(8,22);
	display.print("Attente d'un badge");
	display.display();
}

void SerialEvent3(int * connected) {

	while (Serial3.available()) {  //on construit une chaine de caractères avec ce qu'on recoit de l'esp
		char inChar = Serial3.read();
		instring += inChar;
		Serial.print(inChar);
	}

	/// Methode à appliquer en fonction des messages recues depuis l'ESP

	if(instring.indexOf("ESP Connexion reussie")>0){
		*connected=1;
	}
	if(instring.indexOf("ESP Connexion perdue")>0){
		*connected=0;
	}
	if(instring.indexOf("Ouverture")>0){
		sound(1);
		Serial.print("Ouverture de la porte");
		digitalWrite(greenled,HIGH);
		openLock();
	}
	

	instring=""	; //on réinitialise la string 
}

int bdd_recognized(int uid_read[]){
    int result=0;

    Serial3.print("[RFID#");
    for(int i=0;i<4;i++){
        Serial3.print(uid_read[i],HEX);
    }
    Serial3.print("]");

	delay(3000); ///pour etre sur que le message puisse etre transféré à l'atmega 
	display.setCursor(0,40);
	char inChar = Serial3.read();

    instring += inChar;
    if(inChar=='O' ){
		sound(1);
		digitalWrite(greenled,HIGH);
		result=1;
		openLock();

    }
    if(inChar!='O'){
		sound(0);
    }
	
	while (Serial3.available()) {    
		char inChar = Serial3.read();
    
		instring += inChar;
    
		Serial.print(inChar);
		display.print(inChar);
	}
  
	instring="";
	return result;
}

void openLock(){
	digitalWrite(relaySET, HIGH);
  	delay(100);
  	digitalWrite(relaySET, LOW);
	delay(1000);
  	digitalWrite(relayUNSET,HIGH);
  	delay(100);
  	digitalWrite(relayUNSET, LOW);
}

void sound(int state){

	if(state==1){
		for(int nLoop = 0;nLoop < 2;nLoop ++)
        {
			tone(sAudioPin,880);
			delay(50);
			tone(sAudioPin,988);
			delay(50);
			tone(sAudioPin,523);
			delay(50);
			tone(sAudioPin,988);
			delay(50);
			tone(sAudioPin,523);
			delay(50);
			tone(sAudioPin,587);
			delay(50);
			tone(sAudioPin,523);
			delay(50);
			tone(sAudioPin,587);
			delay(50);
			tone(sAudioPin,659);
			delay(50);
			tone(sAudioPin,587);
			delay(50);
			tone(sAudioPin,659);
			delay(50);
			tone(sAudioPin,659);
			delay(50);
        }
        noTone(sAudioPin);
    }
    else
    {
		digitalWrite(redled,HIGH);
        tone(sAudioPin,392);
        delay(250);
        tone(sAudioPin,262);
        delay(500);
        noTone(sAudioPin);
    }
}