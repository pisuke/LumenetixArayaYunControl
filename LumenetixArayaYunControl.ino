/* Arduino Yun control for Lumenetix Araya module using DMX + 1-10V interface
 * 
 * 
 * 
 * 
 */

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

#include <DmxSimple.h>

const int brightnessPin = 11;  // the pin for brightness control, purple+ grey- (strangely DMX channel 1 does not do anything to brightness ...)

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;

void setup() {

  pinMode(brightnessPin, OUTPUT);
  Bridge.begin();
  
  /* The most common pin for DMX output is pin 3, which DmxSimple
  ** uses by default. If you need to change that, do it here. */
  DmxSimple.usePin(3);

  // initialize the serial communication:
  //Serial.begin(9600);  
  
  DmxSimple.maxChannel(4);
  
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {

    // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms

}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "brightness" command?
  if (command == "brightness") {
    brightnessCommand(client);
  }

  // is "cct" command?
  if (command == "cct") {
    cctCommand(client);
  }

  // is "saturation" command?
  if (command == "saturation") {
    saturationCommand(client);
  }

  // is "hue" command?
  if (command == "hue") {
    hueCommand(client);
  }
}


void brightnessCommand(BridgeClient client) {
  int brightness;

  // Read pin number
  brightness = client.parseInt();
  analogWrite(brightnessPin, brightness);

  // Send feedback to client
  client.print(F("Brightness set to "));
  client.println(brightness);
}

void cctCommand(BridgeClient client) {
  int cct;

  // Read pin number
  cct = client.parseInt();
  DmxSimple.write(1, cct);

  // Send feedback to client
  client.print(F("CCT set to "));
  client.println(cct);
}

void saturationCommand(BridgeClient client) {
  int saturation;

  // Read pin number
  saturation = client.parseInt();
  DmxSimple.write(2, saturation);

  // Send feedback to client
  client.print(F("Saturation set to "));
  client.println(saturation);
}


void hueCommand(BridgeClient client) {
  int hue;

  // Read pin number
  hue = client.parseInt();
  DmxSimple.write(3, hue);

  // Send feedback to client
  client.print(F("Hue set to "));
  client.println(hue);
}

    
  
  
