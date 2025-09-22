#include <ESP8266WiFi.h>    // include files for ESP32
#include <WiFiUdp.h>        // UDP is the wireless data transmission protocol used

// UDP since its fast and ideal for gaming applications not extremely reliable but the next packet comming right after so doesnt matter much

  
const char *SSID = "keios";   // WIFI NAME
const char *PASS = "";        // WIFI PWD

const uint16_t port = 5555;   // Port on recieving device, can be any value bw 1024–65535 // 
const char *host = "192.168.217.81";  // ip or dns // get this for the wifi by 'ipconfig' on the recieving machine

WiFiUDP Udp;                  // initialise WIFIUPD class

int JXRead = D7;              // MUX S0
int JYRead = D5;              // MUX S0'
float joyX, joyY;             // Joystick inputs X and Y axis
float joyXZeroOffset, joyYZeroOffset;   // For calibrating (error evaluation)
const int bUP = D6;           // UP button    Input
const int bDOWN = D3;         // DOWN button  Input
const int bLEFT = D4;         // LEFT  button Input
const int bRIGHT = D2;        // RIGHT button Input

bool buttonState[] = { false, false, false, false };  //UP, DOWN , LEFT , RIGHT // Initial values of the inputs from the buttons

char name[20] = "MyName";     // DEVICE NAME
char moveCommand[19] = "0000| 0.000, 0.000";  // SIGNALS : UP,DOWN,LEFT,RIGHT | JX,JY
char msgToSend[50];           // SIGNAL TO BE SENT

void setup() {
	Serial.begin(115200);		// SERIAL COMM BAUD RATE

	pinMode(JXRead, OUTPUT);			///////////////////	
	pinMode(JYRead, OUTPUT);			//				 //	
	pinMode(bUP, INPUT_PULLUP);			// SET PIN MODES //	
	pinMode(bDOWN, INPUT_PULLUP);		//				 //	
	pinMode(bLEFT, INPUT_PULLUP);		//				 //	
	pinMode(bRIGHT, INPUT_PULLUP);		///////////////////	


	//Calibrating joystick center

	digitalWrite(JXRead,HIGH);
	digitalWrite(JYRead,LOW);
	joyXZeroOffset = analogRead(A0);		// X-input Callibration

	digitalWrite(JXRead,LOW);
	digitalWrite(JYRead,HIGH);
	joyYZeroOffset = analogRead(A0);		// Y-input Callibration



    WiFi.begin(SSID, PASS);
    Serial.println("\n");

    Serial.print("Waiting for WiFi... ");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }																	// Prints the waiting with increasing '.'s

	
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {

    buttonState[0] = (digitalRead(bUP) == LOW);			// 						// 
    buttonState[1] = digitalRead(bDOWN) == LOW; 		// If input == low		//    // SINCE THE BUTTONS CONNECTED TO GND (LOW)
    buttonState[2] = digitalRead(bLEFT) == LOW; 		// 		button == 1		// 
    buttonState[3] = digitalRead(bRIGHT) == LOW;		// 						// 
    
	  digitalWrite(JXRead,HIGH);										// Mux Signals
    digitalWrite(JYRead,LOW); 										// Mux Signals
    delay(10);
    joyX = -(analogRead(A0) - joyXZeroOffset); 
    
    digitalWrite(JXRead,LOW); 										// Mux Signals
    digitalWrite(JYRead,HIGH);										// Mux Signals
    delay(10);
    joyY = -(analogRead(A0) - joyYZeroOffset); 

  	// Calibration Corrections	
	/*
	What the code below does is to normalise the input from [-1,1]
	-1 == left
	 0 == center
	 1 == right
								*/


    if (joyX < 0)
    {
      joyX = joyX/(920-joyXZeroOffset);
    }
    else
    {
      joyX = joyX / joyXZeroOffset;
    }

  if (joyY < 0)
    {
      joyY = joyY/(920-joyYZeroOffset);
    }
    else
    {
      joyY = joyY /joyXZeroOffset;
    }


	// for the transmission of the message

    snprintf(moveCommand, sizeof(moveCommand), "|%d%d%d%d|%6.3f,%6.3f", buttonState[0], buttonState[1], buttonState[2], buttonState[3], joyX, joyY); 	// The string to be sent via UDP [formatting done in C using snprintf]
														// 6.3f == 3 decimal places and max width of num =6
	  strcat(msgToSend, name);
    strcat(msgToSend, moveCommand);
    Serial.println(msgToSend);

    Udp.beginPacket(host, port);
    Udp.write(msgToSend);
    Udp.endPacket();
    msgToSend[0] = '\0';		// '/0' is the end of string char
    delay(20);
}