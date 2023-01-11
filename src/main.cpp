#include <Arduino.h>
#include <Wire.h>

#include "wificonnect.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCCBDMgoFLSm3EKDVlvrQMCRRD2avPmiQI"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://smarthome-18.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

// Device Location config
String device_location = "Living Room"; 

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

// Firebase database path
String databasePath = ""; 
// Firebase Unique Identifier
String fuid = "0668ceea-7680-44f4-9b42-0f742f059e94"; 
// Stores the elapsed time from device start up
unsigned long elapsedMillis = 0; 
// The frequency of sensor updates to firebase, set to 10seconds
unsigned long update_interval = 10000; 
// Dummy counter to test initial firebase updates
int count = 0; 
// Store device authentication status
bool isAuthenticated = false;

/* Put your SSID & Password */
const char* ssid;  // Enter SSID here
const char* password;  //Enter Password here
const char* deviceName;

// Initialize server
AsyncWebServer server(80);
// Make variable prototype
void handleRequest(AsyncWebServerRequest *request);


void firebase_init() {
// configure firebase API Key
config.api_key = API_KEY;
// configure firebase realtime database url
config.database_url = DATABASE_URL;
// Enable WiFi reconnection 
Firebase.reconnectWiFi(true);
Serial.println("------------------------------------");
Serial.println("Sign up new user...");
// Sign in to firebase Anonymously
if (Firebase.signUp(&config, &auth, "", ""))
{
Serial.println("Success");
  isAuthenticated = true;
// Set the database path where updates will be loaded for this device
  databasePath = "/" + device_location;
  fuid = auth.token.uid.c_str();
}
else
{
  Serial.printf("Failed, %s\n", config.signer.signupError.message.c_str());
  isAuthenticated = false;
}
// Assign the callback function for the long running token generation task, see addons/TokenHelper.h
config.token_status_callback = tokenStatusCallback;
// Initialise the firebase library
Firebase.begin(&config, &auth);
}

void setup() {
 /*  Common cathode led RGB */
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for native USB

  //  Connecting to wifi -----------------------
  ssid = "Puzzle24";  // Enter SSID here
  password = "gzcmb94463";  // Enter Password here
  deviceName = "Smart Lamp"; // DHCP Hostname (useful for finding device for static lease)
  wificonnect(ssid, password, deviceName);

  Serial.println(WiFi.gatewayIP());

  firebase_init();

    //  Creating handle request from client -----------------------
  server.on("/", HTTP_ANY, handleRequest);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
   Serial.printf("Get string... %s\n", Firebase.RTDB.getString(&fbdo, F("/relay/state")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
}

void handleRequest(AsyncWebServerRequest *request){
  request->send(200, "text/plain", "Testing Koneksi Berhasi;");
}