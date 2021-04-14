#include <DHT.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 4
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Definir el pin del sensor IR
byte sensorpir = 10;

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

/*char ssid[] = "peredios";            // your network SSID (name)
char pass[] = "12345678";        // your network password
*/
char ssid[] = "ARRIS-62E2";            // your network SSID (name)
char pass[] = "8FD254B43AECD5BE";        // your network password

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;   // number of requests received

int abanico=0;
int statusabanico=0;


char serverr[] = "192.168.0.4";

WiFiEspClient espclient;
WiFiEspServer server(80);
RingBuffer buf(12);

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
  dht.begin();
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
}


void loop()
{
    float h,t;
  
    // Proceso sin cliente
    h = dht.readHumidity();
    t = dht.readTemperature();    
    // Comprobamos si ha habido algún error en la lectura
    //if (isnan(h) || isnan(t) || isnan(f)) {
    if (isnan(t) || isnan(h)) 
    {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    } 

    Serial.print("Temperatura: ");
    Serial.println(h);
    
  delay(500);
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) 
  {    
    Serial.println("New client");
    // an http request ends with a blank line
    buf.init(); 
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        //buf.init(); 
        char c = client.read();
        buf.push(c);
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");      


           // Leemos la humedad relativa
          h = dht.readHumidity();
          t = dht.readTemperature();    
          // Comprobamos si ha habido algún error en la lectura
          //if (isnan(h) || isnan(t) || isnan(f)) {
          if (isnan(t) || isnan(h)) 
          {
            Serial.println("Error obteniendo los datos del sensor DHT11");
            return;
          }       
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Access-Control-Allow-Origin: *");
          client.println("");   
          client.print("{\"temp\":\"");
          client.print(t);
          client.print("\",\"hum\":\"");                                            
          client.print(h);
          client.print("\",\"iluminacion\":\"");                                      
          if(digitalRead(12)==LOW)
          {
            client.print("OFF");                                                      
          }
          else
          {
            client.print("ON");
          }   
          client.print("\",\"proximidad\":\"");                                      
          if(digitalRead(sensorpir)==LOW)
          {
            client.print("OFF");                                                      
          }
          else
          {
            client.print("ON");
          }                   
          client.println("\"}");
          break;
        }
        
        if (buf.endsWith("GET /H")) {
          Serial.println("Turn led ON");
          //ledStatus = HIGH;
          digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
          if (espclient.connect(serverr, 80)) 
          {
            espclient.print("http://192.168.0.4/IOT/peticiones.php?ilum=OFF&temp=");
            espclient.print(t);
            espclient.print("&hum=");
            espclient.println(h); // This          
            espclient.println(); // Empty line
            delay(10);
            espclient.stop();  // Closing connection to server            
           }
           else
           {
            // If Arduino can't connect to the server (your computer or web page)
            Serial.println("--> connection failed\n");
           }           
        }
        else if (buf.endsWith("GET /L")) {
          Serial.println("Turn led OFF");
          //ledStatus = LOW;
          digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
          if (espclient.connect(serverr, 80)) 
          {
            espclient.print("GET http://192.168.0.4/IOT/peticiones.php?ilum=OFF&temp=");
            espclient.print(t);
            espclient.print("&hum=");
            espclient.println(h); // This          
            espclient.println(); // Empty line
            delay(10);
            espclient.stop();  // Closing connection to server            
           }
           else
           {
            // If Arduino can't connect to the server (your computer or web page)
            Serial.println("--> connection failed\n");
           }   

          
        }
        else if (buf.endsWith("GET /OFF")) {
          Serial.println("Turn led OFF");
          //ledStatus = LOW;
          digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
        }
        else if (buf.endsWith("GET /ON")) {
          Serial.println("Turn led ON");
          //ledStatus = LOW;
          digitalWrite(12, HIGH);    // turn the LED off by making the voltage LOW
        }
       
        
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
