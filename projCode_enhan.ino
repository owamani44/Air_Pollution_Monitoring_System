#include <SoftwareSerial.h>
#include <math.h>

// Pin Definitions
#define MQ7_PIN A0
#define MQ135_PIN A1
#define BUZZER_PIN 8
#define LED_PIN 9

SoftwareSerial gprsSerial(10, 11); // RX, TX for SIM800L

//ThingSpeak Settings 
String apiKey = "9HH5UC4ZTODQ0QD9";
String server = "api.thingspeak.com";
String field1 = "Carbonmonoxide  readings";
String field2 = "AQ and CO2 Readings";

String apn = "internet";     

//Sensor Calibration Constants
float MQ7_R0 = 10.0;  
float MQ135_R0 = 5.0;    
float RL = 10.0;         

// Datasheet log-log slope values
float MQ7_a = -0.77, MQ7_b = 1.699;
float MQ135_a = -0.263, MQ135_b = 0.42;

//Function Prototypes
int readSensorAverage(int pin, int samples = 10);
float getPPM(int analogVal, float R0, float a, float b);
void triggerAlert();
void sendToThingSpeak(float mq7, float mq135);
bool isGSMReady();
void runGSMDiagnostics();
void ShowSerialData();

void setup() {
  Serial.begin(9600);
  gprsSerial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Air Pollution Monitoring System Starting...");
  delay(2000);

  runGSMDiagnostics();  // Initial AT check + setup
}

void loop() {
  //Sensor Readings
  int mq7Raw = readSensorAverage(MQ7_PIN);
  int mq135Raw = readSensorAverage(MQ135_PIN);

  float mq7PPM = getPPM(mq7Raw, MQ7_R0, MQ7_a, MQ7_b);
  float mq135PPM = getPPM(mq135Raw, MQ135_R0, MQ135_a, MQ135_b);

  Serial.print("MQ-7 (CO): ");
  Serial.print(mq7PPM);
  Serial.println(" ppm");

  Serial.print("MQ-135 (Air Quality): ");
  Serial.print(mq135PPM);
  Serial.println(" ppm");

  // Alert System
  if (mq7PPM > 40 || mq135PPM > 40) {
    triggerAlert();
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  //Send to ThingSpeak 
  if (isGSMReady()) {
    sendToThingSpeak(mq7PPM, mq135PPM);
  } else {
    Serial.println("GSM not ready. Skipping upload.");
  }

  delay(20000); // 20 seconds interval
}

// Function Definitions

int readSensorAverage(int pin, int samples) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delay(50);
  }
  return total / samples;
}

float getPPM(int analogVal, float R0, float a, float b) {
  float voltage = (analogVal / 1023.0) * 5.0;
  float Rs = (5.0 - voltage) * RL / voltage;
  float ratio = Rs / R0;
  return pow(10, (log10(ratio) - b) / a);
}

void triggerAlert() {
  Serial.println("ALERT: Pollutant threshold exceeded!");
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
}

void sendToThingSpeak(float mq7, float mq135) {
  String data = "GET /update?api_key=" + apiKey + "&field1=" + String(mq7) + "&field2=" + String(mq135);

  Serial.println("Sending data to ThingSpeak...");

  gprsSerial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");
  delay(5000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSEND");
  delay(1000);
  ShowSerialData();

  gprsSerial.println(data);
  delay(100);
  gprsSerial.write(26); // Ctrl+Z
  delay(5000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
  ShowSerialData();
}

bool isGSMReady() {
  Serial.println("Checking GSM readiness...");

  gprsSerial.println("AT+CPIN?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CREG?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CSQ");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+COPS?");
  delay(1000);
  ShowSerialData();

  return true;
}

void runGSMDiagnostics() {
  Serial.println("Running GSM startup diagnostics...");

  gprsSerial.println("AT");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CPIN?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CSQ");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CREG?");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+COPS?");
  delay(2000);
  ShowSerialData();

  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
  ShowSerialData();

  gprsSerial.println("AT+CSTT=\"" + apn + "\",\"\",\"\""); // Set your APN
  delay(2000);
  ShowSerialData();

  gprsSerial.println("AT+CIICR");
  delay(3000);
  ShowSerialData();

  gprsSerial.println("AT+CIFSR");
  delay(2000);
  ShowSerialData();
}

void ShowSerialData() {
  while (gprsSerial.available()) {
    Serial.write(gprsSerial.read());
  }
}