#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//Librerias para OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "../lib/ota.h"
#include "../lib/interfazweb.h"
#include "../lib/reloj.h"

//Librerias para OTA

conexionWeb *webInterface;

DateTime *reloj;
StaticJsonDocument<512> horaActual;
StaticJsonDocument<512> post_ejemplo;
StaticJsonDocument<1024> get_ejemplo;

TaskHandle_t hilo1;
TaskHandle_t hilo2;
int variableContador2 = 0;
int variableContador = 0;
void contador(void *parameter)
{
  for (;;)
  {
    variableContador++;
    delay(1);
    if (variableContador > 1000)
      delay(1000);
  }
}
void contador2(void *parameter)
{
  for (;;)
  {
    variableContador2++;
    delay(1);
    if (variableContador2 > 10000)
      delay(1000);
  }
}
const char *urlPost = "http://192.168.0.21:9200/pacientes/salud";
const char *geturl = "http://192.168.0.21/app/public/api/resetpassword";

void setup()
{
  Serial.begin(115200);
  //setupOTA("TemplateSketch");

  webInterface = new conexionWeb(ssid, passwrd);
  reloj = new DateTime();

  pinMode(2, OUTPUT);
  /* xTaskCreate(contador, "contador", 4096, NULL, 5, &hilo1);
  xTaskCreate(contador2, "contado2", 4096, NULL, 4, &hilo2); */
}
int sumatoria = 0;

void loop()
{
  String nombres[]={
    "Balbuena",
    "Saladino",
    "Mariachi",
    "Michell",
    "Jhonny",
    "Elizabeth"
    "Dross",
    "Giselle",
    "Arturo"    
    };
int bpms[]={
    78,
    95, 
    94,
    63,
    130,
    144,
    240
    };
  //#ifndef ESP32_RTOS
  //  ArduinoOTA.handle();
  //#endif
  sumatoria++;
  horaActual.clear();
  post_ejemplo.clear();
  reloj->getTime();
  post_ejemplo["hora"] = reloj->timeStringBuff;  
  post_ejemplo["BPM"] = bpms[random((sizeof(bpms)/sizeof(int)))];
  post_ejemplo["paciente"] = nombres[random(5)];
  post_ejemplo["temperatura"] = random(39,40);
  post_ejemplo["Presion_sistolica"] = random(120,160);
  post_ejemplo["Presion_diastolica"] = random(90,110);
  webInterface->webPOST(post_ejemplo, urlPost);
  post_ejemplo.clear();
  post_ejemplo["hora"] = reloj->timeStringBuff;  
  post_ejemplo["BPM"] = random(90,110);
  post_ejemplo["paciente"] = nombres[7];
  post_ejemplo["temperatura"] = random(35,37);
  post_ejemplo["Presion_sistolica"] = random(100,110);
  post_ejemplo["Presion_diastolica"] = random(60,75);

  webInterface->webPOST(post_ejemplo, urlPost);
  Serial.print("Sumatoria: ");
  Serial.println(sumatoria);
  serializeJsonPretty(post_ejemplo, Serial);
  delay(100);
  //webInterface->webGET(geturl);
  //deserializeJson(get_ejemplo, webInterface->POSTresponse);
  //serializeJsonPretty(get_ejemplo, Serial);
  // serializeJson(horaActual,Serial);
}
