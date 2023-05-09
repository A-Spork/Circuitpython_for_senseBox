#include <senseBoxIO.h>
#include <WiFi101.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>

char ssid[] = "SSID";
char pass[] = "Password";
int status = WL_IDLE_STATUS;

  const long intervalInterval = 10000;
  long time_startInterval = 0;
  long time_actualInterval = 0;

SCD30 airSensor;
const char SENSOR_IDRID[] PROGMEM = "sensorID";
static const uint8_t NUM_SENSORS = 1;
const char SENSEBOX_ID [] PROGMEM = "senseBox ID";
const char server [] PROGMEM ="ingress.opensensemap.org";
WiFiClient wifiClient;
BearSSLClient client(wifiClient);
typedef struct measurement {
      const char *sensorId;
      float value;
    } measurement;
char buffer[750];
measurement measurements[NUM_SENSORS];
    uint8_t num_measurements = 0;
const int lengthMultiplikator = 35;


unsigned long getTime() {
      return WiFi.getTime();
    }
    void addMeasurement(const char *sensorId, float value) {
    measurements[num_measurements].sensorId = sensorId;
    measurements[num_measurements].value = value;
    num_measurements++;
    }
    void writeMeasurementsToClient() {
    // iterate throug the measurements array
    for (uint8_t i = 0; i < num_measurements; i++) {
      sprintf_P(buffer, PSTR("%s,%9.2f\n"), measurements[i].sensorId,
                measurements[i].value);
      // transmit buffer to client
      client.print(buffer);
    }
    // reset num_measurements
    num_measurements = 0;
  }
  void submitValues() {
if (WiFi.status() != WL_CONNECTED) {
WiFi.disconnect();
delay(1000); // wait 1s
WiFi.begin(ssid, pass);
delay(5000); // wait 5s
}
  if (client.connected()) {
      client.stop();
      delay(1000);
    }
  bool connected = false;
  char _server[strlen_P(server)];
  strcpy_P(_server, server);
  for (uint8_t timeout = 2; timeout != 0; timeout--) {
    Serial.println(F("connecting..."));
    connected = client.connect(_server, 443);
    if (connected == true) {
      // construct the HTTP POST request:
      sprintf_P(buffer,
                PSTR("POST /boxes/%s/data HTTP/1.1\nAuthorization: access_token\nHost: %s\nContent-Type: "
                     "text/csv\nConnection: close\nContent-Length: %i\n\n"),
                SENSEBOX_ID, server, num_measurements * lengthMultiplikator);
      // send the HTTP POST request:
      client.print(buffer);
      // send measurements
      writeMeasurementsToClient();
      // send empty line to end the request
      client.println();
      uint16_t timeout = 0;
      // allow the response to be computed
      while (timeout <= 5000) {
        delay(10);
        timeout = timeout + 10;
        if (client.available()) {
          break;
        }
      }
      while (client.available()) {
        char c = client.read();
        // if the server's disconnected, stop the client:
        if (!client.connected()) {
          client.stop();
          break;
        }
      }
      num_measurements = 0;
      break;
    }
    delay(1000);
  }
  if (connected == false) {
  delay(5000);
  noInterrupts();
 NVIC_SystemReset();
 while (1)
 ;
 }
  }


void setup() {

if (WiFi.status() == WL_NO_SHIELD) {
    while (true);
}
while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000);
}

 Wire.begin();
if (airSensor.begin() == false)
{
  while (1)
    ;
}
airSensor.useStaleData(true);
ArduinoBearSSL.onGetTime(getTime);
}

void loop() {
time_startInterval = millis();

  if (time_startInterval > time_actualInterval + intervalInterval) {
  time_actualInterval = millis();
    addMeasurement(SENSOR_IDRID,airSensor.getCO2());
  submitValues();
}
}


