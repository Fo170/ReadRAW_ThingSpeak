// #define à définir dans le programme principale :
// #define NOMBRE_DATA  10
// #define SIZE_BUFFER_DynamicJsonDoc  2048
//#define PRINT_DEBUG_MESSAGES_RAW


String timestamp[NOMBRE_DATA];
float value[NOMBRE_DATA];

int httpCode_rawData_ThingSpeak = 0;

// Cette fonction retourne un objet rawDATA qui contient les données brutes du Field choisie
String rawData_ThingSpeak(int channelId, int Num_Field, int Nb_Data_)
{
  String suffixURL = "&results=";
  String url = "http://" + String(THINGSPEAK_URL) + "/channels/" + String(channelId) + "/field/" + String(Num_Field) + ".json?" + suffixURL + String(Nb_Data_);
  String raw_Data = "";
  HTTPClient http;
  http.begin(client, url);  // Envoi de la requête HTTP
  int httpCode = http.GET();
  if(httpCode > 0) 
  {  // Si la requête a réussi
    raw_Data = http.getString();  // Récupération de la réponse
    #ifdef PRINT_DEBUG_MESSAGES_RAW
    Serial.println(raw_Data);
    Serial.println("\n");
    Serial.print("url: ");
    Serial.println(url);
    Serial.print("httpCode: ");
    Serial.println(httpCode);
    #endif
  }
 
 httpCode_rawData_ThingSpeak = httpCode;
 return raw_Data;
}

// Cette fonction extraie les donnée ThingSpeak du Field choisie sur N valeurs antérieur
// Celle-ci sont envoyer dans les tableaux :
// timestamp[NOMBRE_DATA] --> données Horaire en Heure GMT
// value[NOMBRE_DATA] --> données mesures correspondant à l'horodatage
int Extract_N_DATA(String raw, int num_Entries, int size_buffer)
{
    DynamicJsonDocument doc(size_buffer);
    deserializeJson(doc, raw);

    int numEntries_Raw = doc["feeds"].size();

    #ifdef PRINT_DEBUG_MESSAGES_RAW
    Serial.print("\nnumEntries_Raw : "); Serial.println(numEntries_Raw);
    #endif
    String field_x = "field" + String(Num_Field);

    if(numEntries_Raw > num_Entries) numEntries_Raw = num_Entries;

    for (int i = 0; i < numEntries_Raw; i++) 
    {     
      timestamp[i] = doc["feeds"][i]["created_at"].as<String>();
      value[i] = doc["feeds"][i][field_x].as<float>();
    }

    return numEntries_Raw;
}