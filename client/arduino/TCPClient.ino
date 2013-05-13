/*
  modified Webclient from the Arduino Examples (by David A. Mellis)
  to a simple TCP Client
  
  created 18.04.2012
  by Laurid Meyer
  
  http://www.ahorndesign.com
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(188, 165, 193, 200); // IP Adress to our Server
int serverPort=33333;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to
EthernetClient client;

const int button_pin = 7;
int button_state = 0;

String tweet_pin;
String tweet_length;
boolean is_after_pipe;

void setup() {
  pinMode(button_pin, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  
  // start the serial for debugging
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  connectToServer();
}

void loop()
{
  button_state = digitalRead(button_pin);
  
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    
    if(c != '|' && is_after_pipe == false) {
      tweet_pin += c;
    } else if(c != '<' && is_after_pipe == true) {
      tweet_length += c;
    } else if(c == '|') {
      is_after_pipe = true;
    } else if(c == '<') {
      processTweet(tweet_pin, tweet_length);
      tweet_pin = "";
      tweet_length = "";
      is_after_pipe = false;
    }
  }

  if(button_state == HIGH) {
    connectToServer();
    delay(1000);
  }
}


void processTweet(String tweet_pin, String tweet_length) {
  int timer_pin2 = millis();
  Serial.println(tweet_pin.toInt());
  Serial.println(tweet_length);
  switch(tweet_pin.toInt()){
    case 1:
    case 2:
    case 3:{
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
      break;     
    }
    case 4: case 5: case 6:{
     digitalWrite(3, HIGH);
     delay(500);
      digitalWrite(3, LOW);
     break; 
    }
    case 7: case 8: case 9: case 10:{
       digitalWrite(5, HIGH);
      delay(500);
      digitalWrite(5, LOW);
     break; 
    } 
  }
    
}



void disconnectFromServer() {
  // if the server's disconnected, stop the client:
    client.stop();
}

void connectToServer() {
  disconnectFromServer();
  
   Serial.println("connecting to Server ...");

  // if you get a connection to the Server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("connected");//report it to the Serial
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}


 String split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

