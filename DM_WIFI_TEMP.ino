#include <ESP8266WiFi.h>
#include <math.h>

//  http://arduino.esp8266.com/stable/package_esp8266com_index.json 

unsigned int Rs = 150000;
double Vcc = 3.3;

// WiFi parameters
const char* ssid = "wifi name";
const char* password = "password";

WiFiServer server(80);
void setup() {
Serial.begin(115200);
delay(10);
Serial.println();
// Connect to WiFi network
WiFi.mode(WIFI_STA);
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
Serial.println(WiFi.localIP());
}

void loop() {

  delay(1000);

  Serial.println(Thermister(AnalogRead()));
  float temp = Thermister(AnalogRead());
 
WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 3");  // refresh the page automatically every 5 sec
client.println();
client.println("<!DOCTYPE html>");
client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
client.println("<head>\n<meta charset='UTF-8'>");
client.println("<title>WifiTempSensor</title>");
client.println("<link href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' rel='stylesheet'>");
client.println("</head>\n<body>");
client.println("<div class='container'>");
client.println("<div class='panel panel-default' margin:15px>");
client.println("<div class='panel-heading'>");
client.println("<H2>Wifi Temp Sensor</H2>");
client.println("<div class='panel-body' style='background-color: powdergreen'>");
client.println("<pre>");
client.print("Temperature (°C)  : ");
client.println(temp, 2);
client.println("</pre>");
client.println("</div>");
client.println("</div>");
client.println("</div>");
client.print("</body>\n</html>");
}

int AnalogRead() {
  int val = 0;
  for(int i = 0; i < 20; i++) {
    val += analogRead(A0);
    delay(1);
  }

  val = val / 20;
  return val;
}

double Thermister(int val) {
  double V_NTC = (double)val / 1024;
  double R_NTC = (Rs * V_NTC) / (Vcc - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC ))* R_NTC );
  //Temp = Temp - 273.15;       
   Temp = Temp - 303.01;         
  return Temp;

}
