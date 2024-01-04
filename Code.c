#include <Arduino.h>
#if defined(ESP32)

//#elif defined(ESP8266)

#endif

String MainString = "";  //The MainString variable is declared as an empty string.

unsigned long sendDataPrevMillis = 0;
int count = 0;


char Rec_Buff()  //The Rec_Buff function reads a character from the Serial2 port and returns it. It waits until data is available using a while loop.
{
  char inChar = 0;
  while (Serial2.available() == 0)
  {
    // Wait for data to be available
  }
  inChar = (char)Serial2.read();
  return inChar;
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial2.println("AT+CPIN?"); //+CPIN: READY: Indicates that the SIM card is ready 
  delay(1000);
  //Serial2.println("AT");
  delay(1000);
}

void loop()
{
  while (1)
  {
    Serial2.println("AT+CLBS=1"); //It sends an AT command to request the location information (AT+CLBS=1) and waits for a response from the SIM card.
    delay(3000);
    String Rx = "";
    while (Serial2.available() > 0)
    {
      Rx += (char)Serial2.read();
      delay(2);
    }
    Serial.print("DATA: ");
    Serial.println(Rx);
    int val1 = 0, val2 = 0;
    if (-1 != (Rx.indexOf("+CLBS"))) //is used to find the position of a specified substring within a string Rx
    {
      val1 = Rx.indexOf(",");
      val2 = Rx.length();
      String value1 = Rx.substring(val1 + 1, val2);
      Serial.print("value 1: ");
      Serial.println(value1);

      val1 = val2 = 0;  //val1 and val2 are declared and initialized to 0. These variables will be used to store the indices for string manipulation.
      val1 = value1.indexOf(","); //Inside the if block, the code extracts the values after the first comma in the Rx string. It finds the position of the first comma using Rx.indexOf(",") and stores it in val1. 
      val2 = value1.length();  //The length of the Rx string is stored in val2.
      String Latitude = value1.substring(0, val1 - 1);
      float Lat = Latitude.toFloat();
      String value2 = value1.substring(val1 + 1, val2);
      Serial.print("Latitude: ");
      Serial.println(Lat, 6);

      val1 = val2 = 0;
      val1 = value2.indexOf(",");
      val2 = value2.length();
      String Longitude = value2.substring(0, val1 - 1);
      float Long = Longitude.toFloat();
      Serial.print("Longitude: ");
      Serial.println(Long, 6);
      val1 = val2 = 0;

      int dataCount = 20; // Number of latitude and longitude data points to send

  

      if ((Lat > 0) && (Long > 0))
      {
       
        MainString = "{\"Latitude\":" + String(Latitude) + ",\"Longitude\":" + String(Longitude) + "}"; 
        int Length = MainString.length(); //The length of the MainString is calculated, and a series of AT commands is sent to the Serial2 port to set up an HTTP request to the Firebase Realtime Database.
        Serial2.print("AT+HTTPPARA=\"URL\",https://bus-tracking-343fd-default-rtdb.asia-southeast1.firebasedatabase.app/.json?");//It is typically used to configure settings related to HTTP requests, such as the URL, content type, timeout, etc.
        //Serial2.println("auth=YOUR_FIREBASE_AUTH_TOKEN");
        delay(2000);
        Serial2.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
        delay(2000);
        Serial2.println("AT+HTTPDATA=" + String(Length) + ",10000"); //The AT+HTTPDATA command usually takes two parameters: the length of the data and the timeout duration. The length parameter specifies the size of the data payload in bytes, and the timeout parameter determines the duration in milliseconds within which the data needs to be sent.
        delay(3000);
        Serial2.println(MainString);
        Serial.println(MainString);
        delay(3000);
        Serial2.println("AT+HTTPACTION=1"); //0: This value indicates an HTTP action to be performed with an HTTP GET method.
             //1: This value indicates an HTTP action to be performed with an HTTP POST method.
//2: This value indicates an HTTP action to be performed with an HTTP HEAD method.
        delay(3000);
        Serial2.println("AT+HTTPREAD");  //The AT+HTTPREAD command is used to read and retrieve this response data from the module.
        delay(3000);
      }
    }
    Rx = "";
  }
}
