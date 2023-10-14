#include <Config.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <AutoConnect.h>
#include <WebServer.h>
#include <NTPClient.h>

#include <Display.h>
#include <OpenTransportDataSwiss.h>

// Set offset time in seconds to adjust for your timezone, for example:
// Note: time offset is not necessary
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT 0 = 0
int timeOffset = 3600;
const char timeServer[] = TIME_SERVER;
int sensorValue;
int loopCounter = 0;

OpenTransportDataSwiss api(
    OPEN_DATA_URL,
    OPEN_DATA_API_KEY,
    3);
// OpenTransportDataSwiss api_grimsel(
//     ID_GRIMSELSTRASSE,
//     OPEN_DATA_DIRECTION,
//     OPEN_DATA_URL,
//     OPEN_DATA_API_KEY,
//     "1");

// OpenTransportDataSwiss api_altstetten(
//     ID_ALTSTETTEN_BANHOF,
//     "A",
//     OPEN_DATA_URL,
//     OPEN_DATA_API_KEY,
//     "4");

Display display;

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, timeServer, timeOffset, 60000);
WiFiClient client;

bool startCP(IPAddress ip)
{
  display.connectionMsg(AP_NAME, AP_PASSWORD);
  delay(10000);
  return true;
}

void setup()
{
  delay(1000);

#ifdef DEBUG
  Serial.begin(MONITOR_SPEED);
  Serial.setDebugOutput(true);
  Serial.println("Debug Enabled");
#endif

  display.begin(
      R1_PIN,
      G1_PIN,
      B1_PIN,
      R2_PIN,
      G2_PIN,
      B2_PIN,
      A_PIN,
      B_PIN,
      C_PIN,
      D_PIN,
      E_PIN,
      LAT_PIN,
      OE_PIN,
      CLK_PIN,
      PANEL_RES_X,
      PANEL_RES_Y,
      PANEL_CHAIN);

  display.showSplash();

  delay(3000);

  display.connectingMsg();

  Config.title = "VBZ-Anzeige";
  Config.apid = AP_NAME;
  Config.psk = AP_PASSWORD;
  // Config.staip = IPAddress(192,168,4,100);
  // Config.staGateway = IPAddress(192,168,1,1);
  // Config.staNetmask = IPAddress(255,255,255,0);
  // Config.dns1 = IPAddress(192,168,1,1);

  Portal.config(Config);
  Portal.onDetect(startCP);

  if (Portal.begin())
  {

#ifdef DEBUG
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
#endif

    display.showIpAddress(WiFi.localIP().toString().c_str());
    delay(2000);

    // Initialize a NTPClient to get time
    timeClient.begin();
    timeClient.setTimeOffset(timeOffset);
  }

  // force one update to get epoch time
  while(!timeClient.forceUpdate()); //internal timeout of 1s

}

void loop()
{
  Portal.handleClient();

  static unsigned char ntpErr = 0;
  ntpErr = timeClient.update() ? 0 : ntpErr + 1;

  if(ntpErr>120/POLLING_FREQ_SECONDS)
  {
    display.printError("Unable to get time\nfrom NTP Server:\n" + (String) TIME_SERVER+"\nFailed "+(String)ntpErr+" times");
    ntpErr = 0;
  }

  Serial.println(timeClient.getFormattedDate());

  // brightness sensor
  sensorValue = analogRead(A0);
  sensorValue = map(sensorValue, 0, 4095, 12, 200);
  display.displaySetBrightness(sensorValue);


  StaticJsonDocument<1500> doc;
  JsonArray array = doc.to<JsonArray>();
  int apiErr;

  String nowDate = timeClient.getFormattedDate();
  apiErr = api.getWebData(ID_GRIMSELSTRASSE, nowDate, nowDate, 2, OPEN_DATA_DIRECTION);
  if (!apiErr){
    // Serial.println(api.doc.as<String>());
    for (const auto &value : api.doc.as<JsonArray>()){
      array.add(value);
    }
  }
  else{
    display.printError(api.httpLastError);
  }

  String depDate = timeClient.getFormattedDate(timeClient.getEpochTime() + 6*60);
  apiErr |= api.getWebData(ID_ALTSTETTEN_BANHOF, nowDate, depDate, 5, "A");
  if (!apiErr){
    // Serial.println(api.doc.as<String>());
    for (const auto &value : api.doc.as<JsonArray>()){
      array.add(value);
    }
  }
  else{
    display.printError(api.httpLastError);
  }

  // only draw if no err to allow for error msg to be displayed
  if(!apiErr){
    display.printLines(array);
  }

  delay(POLLING_FREQ_SECONDS*1000);
}
