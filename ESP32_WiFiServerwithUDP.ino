/*
 This software provides a Web Server that utilises UDP to send commands to a series of cleints, enabling the clients to be controlled.
 In this example the Clients are controlled with a simple command of C01H or C01L meaning Client-01 High or Client-01 Low
  
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

#include <WiFi.h>
#include <WiFiUdp.h>

const char * udpAddress = "192.168.0.255"; // Use an IP address on your network that is unliley to be used
                                           // If your router base address is 10.0.0.1 then use 10.0.0.255, adjust accordingly
                                           // Some Router base addresses are 255, if so use e.g. 254, so for example 10.0.0.254
const int    udpPort    = 3333;            // You can use any Port number you like BUT don't use one that is already is use!

const char* ssid        = "Your-SSID";
const char* password    = "Your-PASSWORD";

WiFiServer server(80);
WiFiUDP    udp;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  udp.begin(WiFi.localIP(),udpPort);
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                  // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println(); // Don't delete this line!
            client.println("<!DOCTYPE html><html><head>");
            client.println("<style>");
            // CSS Section to set styles check out W3C School for help
            client.println("body    {background-color: lightblue;font-family: tahoma;}");
            client.println("h1      {color: violet;text-align: center;}");
            client.println("p       {font-size: 20px;text-align: center;colour:MediumOrchid}");
            client.println(".button {background-color:blue;color:white;padding:10px 32px;text-align:center;text-decoration:none;display:inline-block;");
            client.println("font-size:16px;margin: 4px 2px;cursor: pointer;width:8%;}");
            client.println("</style>");
            client.println("</head><body>");
            client.println("<h1>Client Device Control Using UDP</h1>");
            client.println("<p>Select Client Device to control</p>");
            // the content of the HTTP response follows the header:
            client.print("<div style = \"text-align: center;\">");
            // Controls for sending a Turn-ON message
            client.print("<a class=\"button\" href=\"/C01H\">Client 1 Device ON</a>");
            client.print("<a class=\"button\" href=\"/C02H\">Client 2 Device ON</a>");
            client.print("<a class=\"button\" href=\"/C03H\">Client 3 Device ON</a>");
            //client.print("<a class=\"button\" href=\"/C04H\">Client 4 Device ON</a>");
            client.print("<br>"); // New line
            // Controls for sending a Turn-OFF message
            client.print("<a class=\"button\" href=\"/C01L\">Client 1 Device OFF</a>");
            client.print("<a class=\"button\" href=\"/C02L\">Client 2 Device OFF</a>");
            client.print("<a class=\"button\" href=\"/C03L\">Client 3 Device OFF</a>");
            //client.print("<a class=\"button\" href=\"/C04L\">Client 4 Device OFF</a>");
            client.print("<br>"); // New line
            // Controls for sending a general or broadcast like message
            client.print("<a class=\"button\" href=\"/CAON\">Turn All Client Devices ON</a>");
            client.print("<a class=\"button\" href=\"/CAOFF\">Turn All Client Devices OFF</a>");
            client.print("</div>");
            // The HTTP response ends with another blank line:
            client.println();
            client.println("</body></html>");
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /C01H"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C01H"); udp.endPacket();} // Client-01 High
        if (currentLine.endsWith("GET /C02H"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C02H"); udp.endPacket();} // Client-02 High
        if (currentLine.endsWith("GET /C03H"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C03H"); udp.endPacket();} // Client-03 High
        //if (currentLine.endsWith("GET /C04H"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C04H"); udp.endPacket();} // Client-04 High
        if (currentLine.endsWith("GET /C01L"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C01L"); udp.endPacket();} // Client-01 Low
        if (currentLine.endsWith("GET /C02L"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C02L"); udp.endPacket();} // Client-02 Low
        if (currentLine.endsWith("GET /C03L"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C03L"); udp.endPacket();} // Client-03 Low
        //if (currentLine.endsWith("GET /C04L"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("C04L"); udp.endPacket();} // Client-04 Low
        if (currentLine.endsWith("GET /CAON"))  {udp.beginPacket(udpAddress,udpPort); udp.printf("HAC");  udp.endPacket();} // All Clients High
        if (currentLine.endsWith("GET /CAOFF")) {udp.beginPacket(udpAddress,udpPort); udp.printf("LAC");  udp.endPacket();} // All Clients Low
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
/*
 * This server waits for a client to connect and select an option from the buttons, for example if the client/user selects 
 * the 'Client 1 Device ON. button, the UDP message sent will be 'C01H' to all clients. When a 'Client' receives the message C01H (all clients receive the same message!
 * It will match the address e.g. C01 to itself and act on the command H or L or ON or OFF and switch ON the corresponding Device and so-on for the other clients.
 */
