/*
  This software, the ideas and concepts is Copyright (c) David Bird 2019 and beyond.
  All rights to this software are reserved.

  It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:
  1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
  2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
  3. You may not, except with my express written permission, distribute or commercially exploit the content.
  4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.
  5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies or substantial portions of the software and where the software use is visible to an end-user.

  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
  FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid           = "Your-SSID";
const char * password       = "Your-PASSWORD";
String       client_address = "C01";
int          udpPort        = 3333; // Make sure this port matches the 'Server' port!
int          Control_pin    = 22;   // The device pin on the client to control e.g. an LED or Relay On/Off
String       received_message;

AsyncUDP udp;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("Listening on Port: " + String(udpPort));
  if (udp.listen(udpPort)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      received_message = packet.readStringUntil('\n');
      //Serial.println(received_message);
      received_message.trim(); // Remove unwanted characters
      // Test for a valid message structure 
      if (received_message.startsWith(client_address) || received_message.startsWith("HAC") || received_message.startsWith("LAC"))
      { // e.g. C01H or C)1L or ACH or ACL
        if (received_message.endsWith("H")) {
          Serial.println("Turning ON Device for "  + client_address);
          Device_Control("ON");
        }
        if (received_message.endsWith("L")) {
          Serial.println("Turning OFF Device for " + client_address);
          Device_Control("OFF");
        }
        if (received_message == "HAC")      {
          Serial.println("Turning ON Device for "  + client_address + " - global ON");
          Device_Control("ON");
        }
        if (received_message == "LAC")      {
          Serial.println("Turning OFF Device for " + client_address + " - global OFF");
          Device_Control("OFF");
        }
      }
    });
  }
  //udp.broadcast("Device operated correctly");
}

void loop()
{

}

void Device_Control(String Device_mode) {
  if (Device_mode ==  "ON") {
    pinMode(Control_pin, OUTPUT);
    digitalWrite(Control_pin, HIGH);
  }
  else {
    pinMode(Control_pin, OUTPUT);
    digitalWrite(Control_pin, LOW);
  }
}

// Examples of information avialable by the receiver
//Serial.print("UDP Packet Type: ");
//Serial.println(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
//Serial.print(", From: ");
//Serial.print(packet.remoteIP());
//Serial.print(":");
//Serial.print(packet.remotePort());
//Serial.print(", To: ");
//Serial.print(packet.localIP());
//Serial.print(":");
//Serial.print(packet.localPort());
//Serial.print(", Length: ");
//Serial.print(packet.length());
//Serial.print(" Received this: '");
//Serial.write(packet.data(), packet.length());
