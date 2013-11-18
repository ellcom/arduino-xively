#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00 }; // <--- DEAD-BEEF-EEEE -- Might change to 0001 for better numbering

// The Reference Voltage
double REF_VOLTAGE = 3.3;

// Your Xively key to let you upload data
char xivelyKey[] = "abc123"; //<-- This should be your API KEY with write permisson!!!

// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
int sensorPinIn = 2; // ANALOG PIN 2 - Indoor Channel
int sensorPinOut = 3;// ANALOG PIN 3 - Outdoor Channel


// Default values for indoor and outdoor last recorded temperatures
float lastIndoor = 1000, lastOutdoor = 1000;

// Define the strings for our datastream IDs
char sensorIdIn[] = "indoor"; // Channel names as defined on xively
char sensorIdOut[] = "outdoor";
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorIdIn, strlen(sensorIdIn), DATASTREAM_FLOAT),
  XivelyDatastream(sensorIdOut, strlen(sensorIdOut), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(974153165, datastreams, 2 /* number of datastreams */); // First Number is the datastread or feed, you'll need to change this

EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogReference(EXTERNAL);

  Serial.println("Starting datastream upload to Xively...");

  while (Ethernet.begin(mac) != 1){
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {

  // Calculate the temperature
  int reading = analogRead(sensorPinIn);
  float voltage = (reading * REF_VOLTAGE) / 1024.0;
  float temperature = (voltage - 0.49) * 100 ; // 0.49 is not 0.5 as my tmp36 has some offset needs

  // Remove some noise
  if(lastIndoor == 1000){ lastIndoor = temperature; }
  if(abs(temperature - lastIndoor) > 8){ temperature = lastIndoor; }

  // Even more noise reduction
  temperature = (temperature + lastIndoor)/2;

  // Update lastIndoor
  lastIndoor = temperature;

  // Add to datastream
  datastreams[0].setFloat(temperature);

  // ReCalculate the temperature
  reading = analogRead(sensorPinOut);
  voltage = (reading * REF_VOLTAGE) / 1024.0;
  temperature = (voltage - 0.49) * 100 ; // 0.49 is not 0.5 as my tmp36 has some offset needs

  // Remove some noise
  if(lastOutdoor == 1000){ lastOutdoor = temperature; }
  if(abs(temperature - lastOutdoor) > 8){ temperature = lastOutdoor; }

  // Even more noise reduction
  temperature = (temperature + lastOutdoor)/2;

  // Update lastOutdoor
  lastOutdoor = temperature;

  // Add to datastream
  datastreams[1].setFloat(temperature);

  Serial.print("Read sensor value indoor: ");
  Serial.println(datastreams[0].getFloat());

  Serial.print("Read sensor value outdoor: ");
  Serial.println(datastreams[1].getFloat());

  Serial.println("Uploading it to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);

  Serial.println();
  delay(60000);
}
