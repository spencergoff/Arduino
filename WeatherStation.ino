
//code derived from http://www.instructables.com/id/Tweeting-From-MKR1000/ and the Arduino IDE example Wifi101 -> ScanNetworks
#include <dht.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Twitter.h>
#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

dht DHT;
char ssid[] = "Spencer's iPhone";     //  your network SSID (name)
char pass[] = "spencers";  // your network password
int status = WL_IDLE_STATUS;     // the WiFi radio's status

Twitter twitter("934813958439043072-RmWRNCKFCTwSm6aafgHdktL6nbUgqZD"); //my token

void setup(){
  
  /*Setup for DHT sensor*********/
  Serial.begin(9600);
  delay(300);//Let system settle
  Serial.println("Humidity and temperature\n\n");
  delay(1000);//Wait rest of 1000ms recommended delay before
  
  /*setup for Twitter stuff**********/
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWiFiData();
  
}//end "setup()"

void loop(){
  //read from the pin and print the temperature and humidity
  DHT.read11(dht_dpin);
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

  String temperature = String(DHT.temperature);
  String humidity = String(DHT.humidity);

  String message = "The current temperature is " + temperature + ". The current humidity is " + humidity + ".";
  char msg[100];
  message.toCharArray(msg, 100);
  
  //post the temperature and humidity to Twitter
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }

  //Wait 1 minute, then check and post again
  delay(60000); //60000 ms = 1 minute; should be once per two seconds, fastest
  
}// end loop()

void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

