#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "tugasakhirdft-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "aXVPp1w2I4p7YXouf6Zoqw4yl0baV23h7Kz8etKp"
#define WIFI_SSID "Redot4"
#define WIFI_PASSWORD "herry1204"

int led = D1;
int pump = D2;
int wave = D5;
int trigger_pin1 = D4;
int trigger_pin2 = D6;
int echo_pin1 = D7;
int echo_pin2 = D8;
int led_stat;
int pump_stat;
int wave_stat;
int time1;
int time2;
int airnutrisi;
int airtendon;

String values,sensor_data;

void setup() {
  
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(wave, OUTPUT);
  pinMode(trigger_pin1, OUTPUT);
  pinMode(echo_pin1, INPUT);
  pinMode(trigger_pin2, OUTPUT);
  pinMode(echo_pin2, INPUT);
  delay(100);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop() {
  bool Sr =false;
 
  while(Serial.available()){
        sensor_data=Serial.readString(); 
        Sr=true;    
  }
  
  delay(1000);

  if(Sr==true){  
    
  values=sensor_data;
  
  int firstCommaIndex = values.indexOf(',');
  int secondCommaIndex = values.indexOf(',', firstCommaIndex + 1);
  
  String pH_value = values.substring(0, firstCommaIndex);
  String ppm_value = values.substring(firstCommaIndex + 1, secondCommaIndex);

  Firebase.setString("pH_value", pH_value);
  delay(1000);
  Firebase.setString("ppm_value", ppm_value);
  delay(1000);
  
  if (Firebase.failed()) {  
      return;
      }
  }

  air_nutrisi();
  air_tendon();
  led_relay();
  pump_relay();
  wave_relay();   
}

void led_relay(){
  led_stat = Firebase.getInt("led_status");

  if(led_stat == 1){
    digitalWrite(led, HIGH);
    return;
    delay(100);
  }
  else{
    digitalWrite(led, LOW);
    return;
  }
}

void pump_relay(){
  pump_stat = Firebase.getInt("pump_status");

  if(pump_stat == 1){
    digitalWrite(pump, HIGH);
    return;
    delay(100);
  }
  else{
    digitalWrite(pump, LOW);
    return;
  }
}

void wave_relay(){
  wave_stat = Firebase.getInt("wave_status");

  if(wave_stat == 1){
    digitalWrite(wave, HIGH);
    return;
    delay(100);
  }
  else{
    digitalWrite(wave, LOW);
    return;
  }
}

void air_nutrisi(){
  digitalWrite(trigger_pin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin1, LOW);

  time1 = pulseIn(echo_pin1, HIGH);

  airnutrisi = (time1*0.034/2);
  
  Firebase.setInt("air_nutrisi", airnutrisi);
  delay(1000);
}

void air_tendon(){
  digitalWrite(trigger_pin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin2, LOW);

  time2 = pulseIn(echo_pin2, HIGH);

  airtendon = (time2*0.034/2);

  Firebase.setInt("air_tendon", airtendon);
  delay(1000);
}
