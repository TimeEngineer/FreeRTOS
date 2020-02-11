#include <DHTesp.h>
#include <WiFi.h>

const char* ssid = "ssid";
const char* password =  "password";

char* TOKEN = "BBFF-wpqeBGanloDPNDLbtZZcIyYFLDcuGI"; // Put here your TOKEN
char* DEVICE_LABEL_1 = "dht11_temp"; // Your Device label
char* DEVICE_LABEL_2 = "dht11_humi"; // Your Device label

/* Put here your variable's labels*/
char const * VARIABLE_LABEL_1 = "temperature";
char const * VARIABLE_LABEL_2 = "humidity";

/* HTTP Settings */
char const * HTTPSERVER = "things.ubidots.com";
const int HTTPPORT = 80;
char const * USER_AGENT = "ESP8266";
char const * VERSION = "1.0";

WiFiClient clientUbi;

QueueHandle_t Q1;
QueueHandle_t Q2;
/** Initialize DHT sensor */
DHTesp dht;
/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Pin number for DHT11 data pin */
int dhtPin = 19;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  // Connect Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  if (clientUbi.connect(HTTPSERVER, HTTPPORT)) {
    Serial.println("connected to Ubidots cloud");
  } else {
    Serial.println("could not connect to Ubidots cloud");
  }
  
  delay(1000);
  // Initialize temperature sensor
  Q1 = xQueueCreate(1, 8);
  Q2 = xQueueCreate(1, 8);
  xTaskCreate(task1, "Task1", 10000, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 10000, NULL, 2, NULL);
  xTaskCreate(task3, "Task3", 10000, NULL, 2, NULL);
  dht.setup(dhtPin, DHTesp::DHT11);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void task1( void * parameter ){
  for(;;){
    // Reading temperature and humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    TempAndHumidity lastValues = dht.getTempAndHumidity();
    delay(2000);
    xQueueSend(Q1, &lastValues, portMAX_DELAY);
    xQueueSend(Q2, &lastValues, portMAX_DELAY);
  }
}

void task2( void * parameter ){
  for(;;){
    TempAndHumidity lastValues;
    xQueueReceive(Q1, &lastValues, portMAX_DELAY);
    // Affichage port serie
    Serial.println("Temperature: " + String(lastValues.temperature,0));
    Serial.println("Humidity: " + String(lastValues.humidity,0));
  }
}

void SendToUbidots(char* payload, const char* device_label) {

  int contentLength = strlen(payload);

  /* Connecting the client */
  clientUbi.connect(HTTPSERVER, HTTPPORT);

  if (clientUbi.connected()) {
    /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
    clientUbi.print(F("POST /api/v1.6/devices/"));
    clientUbi.print(device_label);
    clientUbi.print(F(" HTTP/1.1\r\n"));
    clientUbi.print(F("Host: "));
    clientUbi.print(HTTPSERVER);
    clientUbi.print(F("\r\n"));
    clientUbi.print(F("User-Agent: "));
    clientUbi.print(USER_AGENT);
    clientUbi.print(F("/"));
    clientUbi.print(VERSION);
    clientUbi.print(F("\r\n"));
    clientUbi.print(F("X-Auth-Token: "));
    clientUbi.print(TOKEN);
    clientUbi.print(F("\r\n"));
    clientUbi.print(F("Content-Type: application/json\r\n"));
    clientUbi.print(F("Content-Length: "));
    clientUbi.print(contentLength);
    clientUbi.print(F("\r\n\r\n"));
    clientUbi.print(payload);
    clientUbi.print(F("\r\n"));

    //Serial.println(F("Making request to Ubidots:\n"));
    //Serial.print("POST /api/v1.6/devices/");
    //Serial.print(device_label);
    //Serial.print(" HTTP/1.1\r\n");
    //Serial.print("Host: ");
    //Serial.print(HTTPSERVER);
    //Serial.print("\r\n");
    //Serial.print("User-Agent: ");
    //Serial.print(USER_AGENT);
    //Serial.print("/");
    //Serial.print(VERSION);
    //Serial.print("\r\n");
    //Serial.print("X-Auth-Token: ");
    //Serial.print(TOKEN);
    //Serial.print("\r\n");
    //Serial.print("Content-Type: application/json\r\n");
    //Serial.print("Content-Length: ");
    //Serial.print(contentLength);
    //Serial.print("\r\n\r\n");
    //Serial.print(payload);
    //Serial.print("\r\n");
  } else {
    Serial.println("Connection Failed ubidots - Try Again");
  }

  /* Reach timeout when the server is unavailable */
  int timeout = 0;
  while (!clientUbi.available() && timeout < 5000) {
    timeout++;
    delay(1);
    if (timeout >= 5000) {
      Serial.println(F("Error, max timeout reached"));
      break;
    }
  }

  /* Reads the response from the server */
  Serial.println(F("\nUbidots' Server response:\n"));
  while (clientUbi.available()) {
    char c = clientUbi.read();
    Serial.print(c); // Uncomment this line to visualize the response on the Serial Monitor
  }

  /* Disconnecting the client */
  clientUbi.stop();
}

void task3( void * parameter ){
  for(;;){
    TempAndHumidity lastValues;
    xQueueReceive(Q2, &lastValues, portMAX_DELAY);
    char payload1[200];
    char payload2[200];
    // Envoie à Ubidots
    sprintf(payload1, "{");
    sprintf(payload1, "%s\"%s\":{\"value\":%s}", payload1, VARIABLE_LABEL_1, String(lastValues.temperature,0));
    sprintf(payload1, "%s}", payload1);
    SendToUbidots(payload1, DEVICE_LABEL_1);
    delay(5000);
    
    sprintf(payload2, "{");
    sprintf(payload2, "%s\"%s\":{\"value\":%s}", payload2, VARIABLE_LABEL_2, String(lastValues.humidity,0));
    sprintf(payload2, "%s}", payload2);
    SendToUbidots(payload2, DEVICE_LABEL_2);
    delay(5000);
  }
}
