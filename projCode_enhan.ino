#include <SoftwareSerial.h>

// Pin assignments
const int MQ7_PIN = A0;   
const int MQ135_PIN = A1; 
const int BUZZER_PIN = 8;
const int LED_PIN = 9;   

// SIM800L GSM Module connections
SoftwareSerial gprsSerial(10, 11); // RX, TX

// ThingSpeak API settings
String apiKey = "9HH5UC4ZTODQ0QD9";
String server = "api.thingspeak.com";
String field1 = "CO readings";
String field2 = "Air Quality Readings";

// Pollution thresholds
const int MQ7_THRESHOLD = 150;   
const int MQ135_THRESHOLD = 300; 

void setup() {
  Serial.begin(9600);
  gprsSerial.begin(9600);
  delay(1000);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Initializing GSM Module...");
  setupGPRS();
}

void loop() {
  int mq7Value = analogRead(MQ7_PIN);   // CO sensor reading
  int mq135Value = analogRead(MQ135_PIN); // Air quality sensor reading

  Serial.print("MQ-7 (CO) Reading: ");
  Serial.println(mq7Value);
  Serial.print("MQ-135 (Air Quality) Reading: ");
  Serial.println(mq135Value);

  if (mq7Value > MQ7_THRESHOLD || mq135Value > MQ135_THRESHOLD) {
    triggerAlert();
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  sendDataToThingSpeak(mq7Value, mq135Value);
  delay(15000); // Wait for 15 seconds
}

// Function to set up GPRS connection
void setupGPRS() {
  gprsSerial.println("AT"); 
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CPIN?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CREG?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CGATT?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
  ShowSerialData();

  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();

  gprsSerial.println("AT+CSTT=\"internet\",\"\",\"\""); 
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CIICR");
  delay(3000);
  ShowSerialData();

  gprsSerial.println("AT+CIFSR");
  delay(2000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
}

// Function to trigger alerts
void triggerAlert() {
  Serial.println("Pollution detected! Triggering alert...");
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
}

// Function to send data to ThingSpeak
void sendDataToThingSpeak(int mq7Value, int mq135Value) {
  Serial.println("Connecting to ThingSpeak...");

  String data = "GET /update?api_key=" + apiKey + "&field1=" + String(mq7Value) + "&field2=" + String(mq135Value);

  gprsSerial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");
  delay(5000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSEND");
  delay(1000);
  ShowSerialData();

  gprsSerial.println(data);
  Serial.println("Sent to ThingSpeak: " + data);
  delay(1000);

  gprsSerial.write(26); 
  delay(5000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
  ShowSerialData();
}

// Function to display SIM800L for troubleshooting
void ShowSerialData() {
  while (gprsSerial.available()) {
    Serial.write(gprsSerial.read());
  }
}
