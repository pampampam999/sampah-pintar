#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5); // (Rx, Tx)
 
#include <ESP8266WiFi.h>
 
//Konfigurasi WiFi
const char *ssid = "sampahpintar";
const char *password = "12345678";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.43.142";

//SET UP NAMA SAMPAH INI
const char *nama_sampah = "RUMAH";

 float berat = 0 ;
 
void setup() {
  Serial.begin(9600);
  s.begin(9600);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //Jika koneksi berhasil, maka akan muncul address di serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
int value = 0;
 
void loop() {
  if (s.available()>0)
  {
    berat=s.read();
    if(berat < 0 ){
      berat=0;
    }
    Serial.println(berat);
  }
  // Proses Pengiriman -----------------------------------------------------------
  delay(500);
  ++value;
 
  
 
  Serial.print("connecting to ");
  Serial.println(host);
 
// Mengirimkan ke alamat host dengan port 80 -----------------------------------
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
// Isi Konten yang dikirim adalah alamat ip si esp -----------------------------
  String url = "/smarttrash/kirim.php?nama_sampah=";
  url += nama_sampah;
  url += "&berat=";
  url += berat;
 
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
// Mengirimkan Request ke Server -----------------------------------------------
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
// Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");
}
