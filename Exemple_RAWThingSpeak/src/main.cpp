/*
Exemple pour extraire les donnée ThingSpeak du "Field" choisie sur 10 valeurs antérieurs
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <ThingSpeak.h>  // https://github.com/mathworks/thingspeak-arduino
int channelId = 123456; 
int Num_Field = 1; // Valeurs
const char* WriteAPI_Keys = "xxxxxxxxxxxxxxxx";
const char* ReadAPI_Keys  = "yyyyyyyyyyyyyyyy";

#include <ArduinoJson.h> // https://arduinojson.org/?utm_source=meta&utm_medium=library.json
#include <Time.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiClient client;

#define NOMBRE_DATA  10
#define SIZE_BUFFER_DynamicJsonDoc  2048
// #define PRINT_DEBUG_MESSAGES_RAW

String rawData = "";
int Nb_Data = NOMBRE_DATA; // Nombre d'entrées à lire
float moy = 0.0;

int year, month, day, hour, minute, second;
void Extract_DateTime_timestamp(String ts)
{
  sscanf(ts.c_str(), "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second);
}

#include <ReadRAW_ThingSpeak.h>

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);
}

void loop() 
{
  //========================================================

  float val = 3.14159;
  ThingSpeak.setField(Num_Field, val);
  ThingSpeak.writeFields(channelId, WriteAPI_Keys);

  //========================================================

  Serial.println("===============================================================\n");

  rawData = rawData_ThingSpeak(channelId, Num_Field, Nb_Data);

  if(httpCode_rawData_ThingSpeak > 0) // Si la requête a réussi
  {  
    int x_Entries_raw = 0;

    x_Entries_raw = Extract_N_DATA(rawData, Nb_Data, SIZE_BUFFER_DynamicJsonDoc);

    moy = 0.0;

    for(int i = 0; i < x_Entries_raw; i++) 
    {
      Serial.print("i( ");
      Serial.print(i);
      Serial.print(" ) ");

      Extract_DateTime_timestamp(timestamp[i]);

      Serial.print("Date GMT: ");  
      Serial.print(day);
      Serial.print("/");
      Serial.print(month);
      Serial.print("/");
      Serial.print(year);

      Serial.print(" Heure GMT: ");
      Serial.print(hour);
      Serial.print(":");
      Serial.print(minute);
      Serial.print(":");
      Serial.print(second);
      Serial.print(", ");

      Serial.print("field");
      Serial.print(Num_Field);
      
      Serial.print(", Value: ");
 
      Serial.print(value[i]);
      Serial.println("'C");
      moy += value[i];
    }

    moy /= (float)x_Entries_raw;
    Serial.print("\nMoyenne : ");
    Serial.print(moy);
    Serial.println("'C");

  } 
  else 
  {
    Serial.println("Error retrieving data from server.");
  }

  //========================================================
  delay(60 * 1000); // toutes les 60 secondes
}

