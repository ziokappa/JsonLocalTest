/*
  This code is to learn how to deserialize and use a Json structure (In the exeample
  the json data is statically assigned in an array of char. Meaning that here we are 
  not considering any http connection. 
  I am using the Arduino IDE 1.8.8 (the very last 1.8.9 was reporting a problem that 
  resulted to be a bug in the compiler). 
  I am also using the ArduinoJson library (6.11.0) by Benoit Blanchon 
  https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
*/

#include <ArduinoJson.h>

// The type of data that we want to store data extract from the json into.
struct clientData {
  double temp;
  double humidity;
} ;

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
}


void loop()
{

  clientData datoGrezzo; // create a variable of type clientData
  
  if (  readReponseContent(&datoGrezzo) ) // Note that we pass the pointer to the datoGrezzo variable 
  {
    printclientData(&datoGrezzo);
  }

  wait();

}



bool readReponseContent(struct clientData* dato) {

  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 48;

  // The input json value is the following
  //{"main": 
  //        {
  //            "t": 
  //                { 
  //                "temp": 296.15, 
  //                "temp_min": 296.15,
  //                "temp_max": 296.15 
  //                }, 
  //            "pressure": 1020, 
  //            "humidity": 69 
  //        }
  //}
  //
  char json[] ="{\"main\": {\"t\":{\"temp\": 296.15, \"temp_min\": 296.15,\"temp_max\": 296.15 }, \"pressure\": 1020, \"humidity\": 69 }}";
  
  // Create a Json document in the heap 
  DynamicJsonDocument jsonDoc(bufferSize);

  // Deserialize the content of json input string into 
  // a jsonDoc variable.
  DeserializationError error = deserializeJson( jsonDoc, json);

  if (error) {
    Serial.print("deserializeJson() failed with code ");
    Serial.println(error.c_str());
    return false;
  }

  // jsonDoc is a multi-dimensional structure in memory
  // that can be accessed with the notation ["..."][".."]
  dato->temp = jsonDoc["main"]["t"]["temp"];
  dato->humidity = jsonDoc["main"]["humidity"];

  return true;
}

// Print the data extracted from the JSON and
// stored into the datoGrezzo variable referenced
// here through its pointer. 
void printclientData(struct clientData* dato) {
  
  Serial.println("Ecco i dati aggiornati!");
  Serial.print("Temp = ");
  Serial.print(dato->temp);
  Serial.println("°");
  Serial.print("Humidity = ");
  Serial.print(dato->humidity);
  Serial.println("%");
  
}


// Pause for a 1 minute
void wait() {
  Serial.println("Wait 60 seconds");
  delay(60000);
}
