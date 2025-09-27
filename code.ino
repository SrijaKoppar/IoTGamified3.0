#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *SSID = "wifi name";
const char *PASS = "password";

const uint16_t port = 5555;  
const char *host = "10.57.203.81";  // Replace with receiver PC's IP

WiFiUDP Udp;

// Pin mapping (NodeMCU labels in comments)
int JXRead = 13;    // D7
int JYRead = 14;    // D5
const int bUP    = 12;  // D6
const int bDOWN  = 0;  // D3
const int bLEFT  = 2;  // D4
const int bRIGHT = 4;  // D2

float joyX, joyY;
float joyXZeroOffset, joyYZeroOffset;

bool buttonState[] = { false, false, false, false };

char name[20] = "MyDevice";
char moveCommand[40];   // Bigger buffer
char msgToSend[60];     // Safe buffer size

void setup() {
  Serial.begin(115200);

  pinMode(JXRead, OUTPUT);
  pinMode(JYRead, OUTPUT);
  pinMode(bUP, INPUT_PULLUP);
  pinMode(bDOWN, INPUT_PULLUP);
  pinMode(bLEFT, INPUT_PULLUP);
  pinMode(bRIGHT, INPUT_PULLUP);

  // Calibrate joystick center
  digitalWrite(JXRead, HIGH);
  digitalWrite(JYRead, LOW);
  delayMicroseconds(200);
  joyXZeroOffset = analogRead(A0);

  digitalWrite(JXRead, LOW);
  digitalWrite(JYRead, HIGH);
  delayMicroseconds(200);
  joyYZeroOffset = analogRead(A0);

  // WiFi connect
  WiFi.begin(SSID, PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Button reads (active LOW)
  buttonState[0] = digitalRead(bUP) == LOW;
  buttonState[1] = digitalRead(bDOWN) == LOW;
  buttonState[2] = digitalRead(bLEFT) == LOW;
  buttonState[3] = digitalRead(bRIGHT) == LOW;

  // Joystick X
  digitalWrite(JXRead, HIGH);
  digitalWrite(JYRead, LOW);
  delayMicroseconds(200);
  joyX = -(analogRead(A0) - joyXZeroOffset);

  // Joystick Y
  digitalWrite(JXRead, LOW);
  digitalWrite(JYRead, HIGH);
  delayMicroseconds(200);
  joyY = -(analogRead(A0) - joyYZeroOffset);

  // Normalize [-1, 1]
  joyX = (joyX < 0) ? (joyX / (920 - joyXZeroOffset)) : (joyX / joyXZeroOffset);
  joyY = (joyY < 0) ? (joyY / (920 - joyYZeroOffset)) : (joyY / joyYZeroOffset);

  // Format string
  snprintf(moveCommand, sizeof(moveCommand), "|%d%d%d%d|%6.3f,%6.3f",
           buttonState[0], buttonState[1], buttonState[2], buttonState[3],
           joyX, joyY);

  msgToSend[0] = '\0';          // clear buffer
  strcat(msgToSend, name);
  strcat(msgToSend, moveCommand);

  Serial.println(msgToSend);

  Udp.beginPacket(host, port);
  Udp.write(msgToSend);
  Udp.endPacket();

  delay(1000); // small pause
}
