#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "filemanager.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <ESP8266HTTPClient.h>

#include "upt.h"
#include <arduino-timer.h>
#include "ws.h"
#include "led.h"
#include "wifistrength.h"
#include "credential.h"
#include "ntp.h"
#include "wm.h"
#include "onebtn.h"

ESP8266WebServer server(80);

auto timer = timer_create_default();

FSInfo fs_info;

String hostIPAddress, roomID, URL;

WiFiClient client;
HTTPClient http;

void handleRoot()
{
  File file = LittleFS.open("/dashboard.html", "r");
  if (!file)
  {
    Serial.println("No Saved Data!");
  }

  server.streamFile(file, "text/html");
  file.close();
}

void handleRedirect()
{
  // TRACE("Redirect...");
  String url = "/dashboard.html";

  if (!LittleFS.exists(url))
  {
    url = "/$update.html";
  }

  server.sendHeader("Location", url, true);
  server.send(302);
} // handleRedirect()

void handleTambah()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  hostIPAddress = server.arg("hostIPAddress");
  roomID = server.arg("roomID");

  Serial.println(hostIPAddress);
  Serial.println(roomID);

  JsonObject obj = data.to<JsonObject>();

  obj["hostIPAddress"] = hostIPAddress;
  obj["roomID"] = roomID;

  // String id = String(data.size());
  // Serial.println("id: " + id);

  // Person person = {id, name, age};
  // JsonObject obj = data.createNestedObject(id);
  // obj["id"] = person.id;
  // obj["name"] = person.name;
  // obj["age"] = person.age;

  File outFile = LittleFS.open("/dataku.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);

  serializeJsonPretty(data, Serial);

  outFile.close();

  server.send(200, "text/plain", "Record added successfully");
}

void handleBuatData()
{
  if (server.method() == HTTP_POST)
  {
    Serial.println("handleCreateData");

    fancyled();

    String data = server.arg("plain");
    File file = LittleFS.open("/dataku.json", "w");
    file.print(data);
    file.close();
    server.send(200, "text/plain", "Data on littleFS saved successfully");
  }
}

void handleDapat()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  String output;
  serializeJsonPretty(data, output);

  server.send(200, "application/json", output);
}

void readDataku()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  const char *hostIPAddressku = data["hostIPAddress"];
  const char *roomIDku = data["roomID"];

  hostIPAddress = String(hostIPAddressku);
  roomID = String(roomIDku);

  String output;
  serializeJsonPretty(data, output);

  // server.send(200, "application/json", output);
}

int bytestoKB(int bkb)
{
  return bkb = bkb / 1024;
}

void hwInit()
{
  Serial.println("hwInit");

  String FreeHeap = String(ESP.getFreeHeap());
  String SketchSize = String(ESP.getSketchSize());

  String ChipRealSize = String(ESP.getFlashChipRealSize());
  String VendorID = String(ESP.getFlashChipVendorId());
  String FlashChipID = String(ESP.getFlashChipId());
  String CPUFreqMhz = String(ESP.getCpuFreqMHz());
  uint16_t v = ESP.getVcc();
  // float vcc = ((float)v / 1024.0f);
  // char v_str[10];
  // dtostrf(vcc, 5, 3, v_str);
  // sprintf(v_str, "%s V", v_str);
  // Serial.println(v_str);

  String VCC = String(v);

  String WiFiSSID = String(WiFi.SSID());
  String LocalIP = WiFi.localIP().toString();
  String SubnetMask = WiFi.subnetMask().toString();
  String GateWay = WiFi.gatewayIP().toString();
  String DNSIP = WiFi.dnsIP().toString();
  String MACAddressHW = WiFi.macAddress();

  LittleFS.info(fs_info);
  String TotalSize = String(fs_info.totalBytes);
  String UsedSize = String(fs_info.usedBytes);

  int TotalSizeKB = bytestoKB(TotalSize.toInt());

  int FreeSpaces = ChipRealSize.toInt() - TotalSize.toInt() - SketchSize.toInt();
  float FreeSpacesPercent = FreeSpaces / ChipRealSize.toFloat();
  float TotalSizePercent = TotalSize.toFloat() / ChipRealSize.toFloat();
  float SketchSizePercent = SketchSize.toFloat() / ChipRealSize.toFloat();

  int FreeSpacesPercentInt = FreeSpacesPercent * 100;
  int TotalSizePercentInt = TotalSizePercent * 100;
  int SketchSizePercentInt = SketchSizePercent * 100;

  String FreeSpace = String(FreeSpaces);

  WiFiStrength = String(dBmtoPercentage(WiFi.RSSI()));

  File file = LittleFS.open("/system.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  // int id = server.arg("id").toInt();

  JsonObject obj = data.to<JsonObject>();

  obj["FreeHeap"] = FreeHeap;
  obj["SketchSize"] = SketchSize;
  obj["FlashChipID"] = FlashChipID;
  obj["ChipRealSize"] = ChipRealSize;
  obj["FreeSpace"] = FreeSpace;
  obj["TotalSize"] = TotalSize;
  obj["TotalSizeKB"] = String(TotalSizeKB);
  obj["UsedSize"] = UsedSize;
  obj["FreeSpacesPercent"] = String(FreeSpacesPercentInt);
  obj["TotalSizePercent"] = String(TotalSizePercentInt);
  obj["SketchSizePercent"] = String(SketchSizePercentInt);
  obj["VendorID"] = VendorID;
  obj["CPUFreqMhz"] = CPUFreqMhz;
  obj["VCC"] = VCC;
  obj["WiFiSSID"] = WiFiSSID;
  obj["LocalIP"] = LocalIP;
  obj["SubnetMask"] = SubnetMask;
  obj["GateWay"] = GateWay;
  obj["DNSIP"] = DNSIP;
  obj["MACAddressHW"] = MACAddressHW;
  obj["TotalSize"] = TotalSize;
  obj["UsedSize"] = UsedSize;
  obj["WiFiStrength"] = WiFiStrength;
  obj["Days"] = Days;
  obj["Hours"] = Hours;
  obj["Minutes"] = Minutes;
  obj["Seconds"] = Seconds;
  obj["MilliSeconds"] = Milliseconds;

  File outFile = LittleFS.open("/system.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    // server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);
  serializeJsonPretty(data, Serial);
  outFile.close();
  fancyled();
}

void handleSystem()
{
  hwInit();

  File file = LittleFS.open("/system.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  String output;
  serializeJsonPretty(data, output);

  server.send(200, "application/json", output);
}

void handleRestart()
{
  server.send(200, "text/plain", "ESP Restart");
  fancyled();
  delay(500);
  ESP.reset();
}

void handleFormat()
{
  fancyled();
  LittleFS.format();
  server.send(200, "text/plain", "LittleFS Formated");
}

// this function will be called when the button was pressed 1 time only.
void singleClick()
{
  Serial.println("singleClick() detected.");

  http.begin(client, URL); // "http://192.168.0.18/call?id=1&status=1"
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String payload = http.getString();
    Serial.println(payload);
    Serial.println(httpCode);
  }
  else
  {
    Serial.println("Error on HTTP request : " + httpCode);
  }

  http.end();
} // singleClick

void setup()
{
  Serial.begin(115200);
  // put your setup code here, to run once:
  // login into WiFi
  Serial.println("\n\nESP8266WebFlMgr Demo basic");

  ESPxWebFlMgr_FileSystem.begin();

  // enable the led output.
  pinMode(PIN_LED, OUTPUT); // sets the digital pin as output
  digitalWrite(PIN_LED, ledState);

  // setup interrupt routine
  // when not registering to the interrupt the sketch also works when the tick is called frequently.
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), checkTicks, CHANGE);

  // link the xxxclick functions to be called on xxxclick event.
  button.attachClick(singleClick);
  button.attachDoubleClick(doubleClick);
  // button.attachMultiClick(multiClick);

  // button.setPressMs(1000); // that is the time when LongPressStart is called
  // button.attachLongPressStart(pressStart);
  // button.attachLongPressStop(pressStop);

  // set led pin as output
  pinMode(LED_BUILTIN, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  // reset settings - for testing
  // wifiManager.resetSettings();

  // set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  // fetches ssid and pass and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("WASISTechOne"))
  {
    Serial.println("failed to connect and hit timeout");
    // reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  // if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  // keep LED on
  digitalWrite(LED_BUILTIN, LOW);

  // login into WiFi
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(1);
  // }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Open Filemanager with http://");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.print(filemanagerport);
    Serial.print("/");
    Serial.println();
  }

  pinMode(blue, OUTPUT);

  ArduinoOTA.onStart([]()
                     {
    analogWrite(blue, 0);
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { 
                    fancyled();
                    Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });
  ArduinoOTA.begin();

  readDataku();

  URL = hostIPAddress + "/call?id=" + roomID + "&status=1";

  Serial.println(URL);

  server.on("/create", HTTP_POST, handleBuatData);
  server.on("/add", handleTambah);
  server.on("/get", handleDapat);
  // server.on("/getid", handleDapatById);
  // server.on("/updateid", HTTP_POST, handlePerbarui);
  // server.on("/deleteid", HTTP_POST, handleHapus);
  // server.on("/call", handleCall);
  server.on("/system", handleSystem);
  server.on("/restart", handleRestart);
  server.on("/format", handleFormat);

  // UPLOAD and DELETE of files in the file system using a request handler.
  // server.addHandler(new FileServerHandler());

  // enable CORS header in webserver results
  server.enableCORS(true);

  // enable ETAG header in webserver results from serveStatic handler
  server.enableETag(true);

  // serve all static files
  server.serveStatic("/index.html", LittleFS, "/index.html");
  server.serveStatic("/", LittleFS, "/dashboard.html");
  server.serveStatic("/main.js", LittleFS, "/main.js");
  server.serveStatic("/styles.css", LittleFS, "/styles.css");
  server.serveStatic("/bed.html", LittleFS, "/bed.html");
  server.serveStatic("/bed.js", LittleFS, "/bed.js");
  server.serveStatic("/bed.css", LittleFS, "/bed.css");
  server.serveStatic("/system.html", LittleFS, "/system.html");
  server.serveStatic("/system.css", LittleFS, "/system.css");
  server.serveStatic("/system.js", LittleFS, "/system.js");
  server.serveStatic("/dashboard.html", LittleFS, "/dashboard.html");
  // server.serveStatic("/dashboard.css", LittleFS, "/dashboard.css");
  server.serveStatic("/dashboard.js", LittleFS, "/dashboard.js");

  // handle cases when file is not found
  // server.onNotFound([](){
  // standard not found in browser.
  // server.send(404, "text/html", FPSTR(notFoundContent)); });

  server.begin();
  filemgr.begin();
  hwInit();

  // call the toggle_led function every 1000 millis (1 second)
  timer.every(250, UPTime);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  webSocket2.begin();
  webSocket2.onEvent(webSocketEvent);
  // LittleFS.format();

  timeClient.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  filemgr.handleClient();
  delay(2);
  server.handleClient();
  delay(2);
  ArduinoOTA.handle();

  // First call calculate_uptime() to calculate the uptime
  // and then read the uptime variables.
  uptime::calculateUptime();
  timer.tick();
  // keep watching the push button, even when no interrupt happens:
  button.tick();

  webSocket.loop();
  webSocket2.loop();
  led_indicator();
  webSocket.broadcastTXT(UPTimes);

  timeClient.update();
  // Serial.println(timeClient.getFormattedTime());
  String NTPTime = timeClient.getFormattedTime();

  webSocket2.broadcastTXT(NTPTime);
}