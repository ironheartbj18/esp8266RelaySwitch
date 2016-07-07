/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "van-vlymen2.4GHz";
const char* password = "2108386631";

int ledPin2 = 2; // GPIO2
int ledPin0 = 0; // GPIO0
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  // prepare GPIO0
  pinMode(0, OUTPUT);
  digitalWrite(2, 0);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int value0 = LOW;
  int value2 = LOW;
  if (req.indexOf("/LED=ON=0") != -1){
    digitalWrite(ledPin0, HIGH);
    value0 = HIGH;
  }
  if (req.indexOf("/LED=ON=2") != -1){
    digitalWrite(ledPin2, HIGH);
    value2 = HIGH;
  }
  if (req.indexOf("/LED=OFF=0") != -1){
    digitalWrite(ledPin0, LOW);
    value0 = LOW;
  }
  if (req.indexOf("/LED=OFF=2") != -1){
    digitalWrite(ledPin2, LOW);
    value2 = LOW;
  }


    // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang=\"en\">");

  client.println("<meta charset=\"utf-8\">");
  client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">");
  client.print("Led pin 0 is: ");
   
  if(value0 == LOW) {
    client.print("Off");  
  } 
  else{
    client.print("On");
  }
  client.println("<br><br>");
  client.println("Click LED 0: <a href=\"/LED=ON=0\" class=\"btn btn-primary\">TURN ON 0</a><br>");
  client.println("Click LED 0: <a href=\"/LED=OFF=0\" class=\"btn btn-danger\">TURN OFF 0</a>");
  client.println("<br><br>");
  client.println("<br><br>");
  
  client.print("Led pin 2 is: "); 
  if(value2 == LOW) {
    client.print("Off"); 
  }  
  else{
    client.print("On");
  }
  client.println("<br><br>");
  client.println("Click LED 2: <a href=\"/LED=ON=2\" class=\"btn btn-primary\">TURN ON 2</a><br>");
  client.println("Click LED 2: <a href=\"/LED=OFF=2\" class=\"btn btn-danger\">TURN OFF 2</a><br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}


