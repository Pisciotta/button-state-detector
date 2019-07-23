#include <WiFi.h>
#include <HTTPClient.h>

//SETTINGS
const char* ssid = ""; //<-- name of the wifi network
const char* password =  ""; //<-- password of the wifi network
const char* test_page = "https://raw.githubusercontent.com/..."; //raw GitHub (e.g. php) file where you will insert the public address given by Tunnel
const int ButtonPin = 23; //<-- depends where is connected your push button
int sensorID = 456; //<-- the sensor's identifier

bool sleep_bool = false;
int ButtonValue = 0;
 
const char* github_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIHqDCCBpCgAwIBAgIQCDqEWS938ueVG/iHzt7JZjANBgkqhkiG9w0BAQsFADBw\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMS8wLQYDVQQDEyZEaWdpQ2VydCBTSEEyIEhpZ2ggQXNz\n" \
"dXJhbmNlIFNlcnZlciBDQTAeFw0xNzAzMjMwMDAwMDBaFw0yMDA1MTMxMjAwMDBa\n" \
"MGoxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\n" \
"YW4gRnJhbmNpc2NvMRUwEwYDVQQKEwxHaXRIdWIsIEluYy4xFzAVBgNVBAMTDnd3\n" \
"dy5naXRodWIuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxtPx\n" \
"ijvPpEXyy3Bn10WfoWmKTW753Uv2PusDNmalx/7mqFqi5BqK4xWQHQgSpyhedgtW\n" \
"IXWCJGHtgFVck+DBAbHiHsE67ewpV1a2l2GpqNCFTU77UsoNVD/xPyx3k+cPX9y8\n" \
"rqjMiZB3xs1zKDYBkcoBVrA+iO323YkJmCLEXCO2O7b1twLFWkNwMd7e7nteu2uC\n" \
"MvxNp5Qg22MIn33t2egMPfIDU/TcKDfyaty5+s6F3gzh7eIgnqNQN0T/5fpaYkqd\n" \
"x8j21QDsIyF/CfSpA5qKLuhluu8xrUbnc0MigX7VThS9PbfxMSQ1cQQfbGdxoQNJ\n" \
"TNHxXv+ZTXAxKCju5wIDAQABo4IEQjCCBD4wHwYDVR0jBBgwFoAUUWj/kK8CB3U8\n" \
"zNllZGKiErhZcjswHQYDVR0OBBYEFDCCKdhtTODUosYQSAWAh6i8qukSMHsGA1Ud\n" \
"EQR0MHKCDnd3dy5naXRodWIuY29tggwqLmdpdGh1Yi5jb22CCmdpdGh1Yi5jb22C\n" \
"CyouZ2l0aHViLmlvgglnaXRodWIuaW+CFyouZ2l0aHVidXNlcmNvbnRlbnQuY29t\n" \
"ghVnaXRodWJ1c2VyY29udGVudC5jb20wDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQW\n" \
"MBQGCCsGAQUFBwMBBggrBgEFBQcDAjB1BgNVHR8EbjBsMDSgMqAwhi5odHRwOi8v\n" \
"Y3JsMy5kaWdpY2VydC5jb20vc2hhMi1oYS1zZXJ2ZXItZzUuY3JsMDSgMqAwhi5o\n" \
"dHRwOi8vY3JsNC5kaWdpY2VydC5jb20vc2hhMi1oYS1zZXJ2ZXItZzUuY3JsMEwG\n" \
"A1UdIARFMEMwNwYJYIZIAYb9bAEBMCowKAYIKwYBBQUHAgEWHGh0dHBzOi8vd3d3\n" \
"LmRpZ2ljZXJ0LmNvbS9DUFMwCAYGZ4EMAQICMIGDBggrBgEFBQcBAQR3MHUwJAYI\n" \
"KwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBNBggrBgEFBQcwAoZB\n" \
"aHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0U0hBMkhpZ2hBc3N1\n" \
"cmFuY2VTZXJ2ZXJDQS5jcnQwDAYDVR0TAQH/BAIwADCCAfUGCisGAQQB1nkCBAIE\n" \
"ggHlBIIB4QHfAHYApLkJkLQYWBSHuxOizGdwCjw1mAT5G9+443fNDsgN3BAAAAFa\n" \
"/UBqBAAABAMARzBFAiBFXsWaC1bup8Q0JgrY9EgIxjqi1v2fA6Zg44iRXSQyywIh\n" \
"AIzhzU1zlseJh5+yXc5U1I+pgqRmXb1XcPIsGL8oOdwjAHUAVhQGmi/XwuzT9eG9\n" \
"RLI+x0Z2ubyZEVzA75SYVdaJ0N0AAAFa/UBqZQAABAMARjBEAiBKQMsySmj69oKZ\n" \
"MeC+MDokLrrVN2tK+OMlzf1T5qgHtgIgRJLNGvfWDmMpCK/iWPSmMsYK2yYyTl9K\n" \
"btHBtP5WpkcAdgDuS723dc5guuFCaR+r4Z5mow9+X7By2IMAxHuJeqj9ywAAAVr9\n" \
"QGofAAAEAwBHMEUCIA2n0TbeAa5KbuOpnXpJbnObwckpOsHsaN+2rA7ZA16YAiEA\n" \
"l7JTnVPdmFcauzwLjgNESMRFtn4Brzm9XJTPJbaWPacAdgC72d+8H4pxtZOUI5eq\n" \
"kntHOFeVCqtS6BqQlmQ2jh7RhQAAAVr9QGoRAAAEAwBHMEUCIQCqrtuq71J6TM7w\n" \
"KMWeSAROdTa8f35GoLMImJXONSNHfQIgONvSu/VH5jlZ1+PD+b6ThFF1+pV7wp7w\n" \
"q+/8xiHUMlswDQYJKoZIhvcNAQELBQADggEBAJl+1i/OG6YV9RWz7/EwwR9UEJKk\n" \
"jEPAvL2lDQBT4kLBhW/lp6lBmUtGEVrd/egnaZe2PKYOKjDbM1O+g7CqCIkEfmY1\n" \
"5VyzLCh/p7HlJ3ltgSaJ6qBVUXAQy+tDWWuqUrRG/dL/iRaKRdoOv4cNU++DJMUX\n" \
"rRJjQHSATb2kyd102d8cYQIKcbCTJC8tqSB6Q4ZEEViKRZvXXOJm66bG8Xyn3N2v\n" \
"J4k598Gamch/NHrZOXODy3N1vBawTqFJLQkSjU4+Y//wiHHfUEYrpTg92zgIlylk\n" \
"3svH64hwWd1i3BZ2LTBq46MvQKU2D8wFdtXgbgRAPWohX79Oo6hs0Jghub0=\n" \
"-----END CERTIFICATE-----\n";



const char* labstack_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEOTCCAyGgAwIBAgIQBEJBmEQGS0ejtJ7dQqq+9zANBgkqhkiG9w0BAQsFADCB\n" \
"gTE6MDgGA1UECwwxZ2VuZXJhdGVkIGJ5IEF2YXN0IEFudGl2aXJ1cyBmb3IgU1NM\n" \
"L1RMUyBzY2FubmluZzEeMBwGA1UECgwVQXZhc3QgV2ViL01haWwgU2hpZWxkMSMw\n" \
"IQYDVQQDDBpBdmFzdCBXZWIvTWFpbCBTaGllbGQgUm9vdDAeFw0xOTA1MTYyMTU1\n" \
"MThaFw0xOTA4MTQyMTU1MThaMBcxFTATBgNVBAMTDGxhYnN0YWNrLmNvbTCCASIw\n" \
"DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANYjhP64CifjHbbk/a0nraFp7spu\n" \
"X6C3TEW3rlmO0nlV5mGBjCGszwQGBRIsVUdOK23vo9vGILMWuHw2TbSv7Jj0LwL0\n" \
"7bFFFKC9H2Tyk+qQyPHF7v0W1GSn/Ld0JCoqLSp68f5RFA7NLqpvuM86Oj7Bj8A9\n" \
"WlrAyT0o9Yo8Nrl6XLnfd48AnHaYP0qAsaZSZT99O7q8/b1yieMxnqzWKxbfw3nE\n" \
"D+a5QC4ox2D5+vksqJzLb4OP8TM++uvu8vHmLrsBHU7cZthDZVtue2ADzH3Mjt9j\n" \
"KihARpuWzBvsLHkuOivltEteQACUDIURXAw4egk0rK5xQTEOTWsMcHVGuUMCAwEA\n" \
"AaOCARQwggEQMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYI\n" \
"KwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUPv+6ovNJUS9nL+CCEV+p\n" \
"k8bwXf0wHwYDVR0jBBgwFoAUn8FgMhXAeGz8kmWhnlaDQ1jw+IEwgZAGA1UdEQSB\n" \
"iDCBhYIIKi5sYWIuc3SCDioubGFic3RhY2suY29tgg0qLmxhYnN0YWNrLm1lghEq\n" \
"Lmxlby5sYWJzdGFjay5tZYIRKi5tYXgubGFic3RhY2subWWCESouem9lLmxhYnN0\n" \
"YWNrLm1lggZsYWIuc3SCDGxhYnN0YWNrLmNvbYILbGFic3RhY2subWUwDQYJKoZI\n" \
"hvcNAQELBQADggEBANwmZlfqkaSoYF0rxJZ3/s2BV3nWjvvNAxA8UWI527Q/JdBT\n" \
"hoGCp+tYfQufy/DrUKV57jjWOrPT11gd8pXOrTTLoh8eXNZ04iZQ5OBRtMPSlLBs\n" \
"07D1bsnyUHPfQbLdtk9e3ddA22CkiHLBF8D2nXAUew/1SrLyOk4oRGpuZ6ssssoX\n" \
"P4Mcxiig9R3czIr3Uv8rJ6z027piF3rgtGmZPdlsUNiVlC8iHqV9ZYKbw+x2HNH8\n" \
"Zf9cD0xgbTFm51Il+SEJIVO6PhfhKIpfAa9/jcB4zq8Ph2Nx6AbuE+O2oXhGqbq6\n" \
"TVy6a3NsTns++fWjjSrFv1X7VFt3NAdR0FHuv8o=\n" \
"-----END CERTIFICATE-----\n";


String tunnel_address;
String response;
int httpCode;

void start();

void setup(){}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
        sleep_bool = !sleep_bool;
        if(sleep_bool){
          gpio_wakeup_enable(GPIO_NUM_23, GPIO_INTR_LOW_LEVEL);
        }else{
          gpio_wakeup_enable(GPIO_NUM_23, GPIO_INTR_HIGH_LEVEL);
        }
        esp_sleep_enable_gpio_wakeup();
        delay(1000);
        esp_light_sleep_start();
        ButtonValue=digitalRead(ButtonPin);
        unsigned long millis1 = millis();
        bool doit = true;
        int count = 0;
do{
    count++;
    HTTPClient http;
    Serial.println(tunnel_address + "/iot/Test/");
    http.begin(tunnel_address + "/iot/Test/index.php", labstack_ca); //Specify the URL and certificate
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    Serial.println();
    Serial.println("Occupied?" + String(ButtonValue));

   String _post  = "occupied=";
          _post += String(ButtonValue);
          _post += "&sensorid=";
          _post += String(sensorID);
          _post += "&timestamp=";
          _post += String(millis()-millis1);
   
    httpCode = http.POST(_post);                                                 
 
    if (httpCode > 0) { //Check for the returning code
        response = http.getString();
        Serial.println(httpCode);
        Serial.println(response);
        doit = false;
      } else {
        Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
     delay(3000);
    Serial.println("Count = " + String(count));
     if(count >= 10){
      start();
      doit=false;
     }
  }while(doit);
}
}



void start(){
  pinMode(ButtonPin, INPUT);
  Serial.begin(115200);
  delay(1000);
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");


  
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
  HTTPClient http;
  http.begin(test_page, github_ca); //Specify the URL and certificate
  httpCode = http.GET();
    do{      
      if (httpCode > 0) { //Check for the returning code
          tunnel_address = http.getString();
          tunnel_address.trim();
          Serial.println(httpCode);
          Serial.println(tunnel_address);
      }else{
          Serial.println("Error on HTTP request");
      }
      delay(1000);
    } while (httpCode <= 0);
    http.end(); //Free the resources
  }
}
