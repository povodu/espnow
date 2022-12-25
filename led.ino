#include <ESP8266WiFi.h>
#include <espnow.h>

bool isLedOn;

const int ledPin = 2;

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    if (len > 0)
    {
        isLedOn = !isLedOn;
        digitalWrite(2, isLedOn);
    }
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    pinMode(ledPin, OUTPUT);

    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
}