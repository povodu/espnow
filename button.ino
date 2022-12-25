#include <ESP8266WiFi.h>
#include <espnow.h>
#include <AceButton.h>

const int button_pin = D6;

//Karşı tarafın mac adresi girilecek 2c:f4:32:11:52:aa
uint8_t broadcastAddress1[] = {0x40, 0x91, 0x51, 0x4f, 0x8f, 0x0c};
uint8_t broadcastAddress2[] = {0x40, 0x91, 0x51, 0x4e, 0xe1, 0xab};

using namespace ace_button;

ButtonConfig config;
AceButton button(&config);

typedef struct Message
{
    String text = "click";
};

Message msg;

void handleEvent(AceButton *, uint8_t eventType, uint8_t)
{
    switch (eventType)
    {
    case AceButton::kEventClicked:
        Serial.println("tıkla");
        esp_now_send(0, (uint8_t *)&msg, sizeof(msg));
        break;
    }
}

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    if (sendStatus == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    pinMode(button_pin, INPUT);

    button.init(button_pin);
    config.setEventHandler(handleEvent);
    config.setFeature(ButtonConfig::kFeatureClick);

    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);

    esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
    button.check();
}
