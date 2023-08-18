#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "iPhone";
const char* password = "12345678";

#define MQTT_SERVER IPAddress(140, 136, 151, 74)
#define MQTT_PORT 1883

WiFiClient espClient;
PubSubClient client(espClient);

const int devicePin = 4;
const char* deviceID = "000001"; // 替換為您的裝置識別符號

void setup() {
  pinMode(devicePin, OUTPUT);

  Serial.begin(115200);
  delay(10);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      messageTemp += (char)payload[i];
    }
    Serial.println();

    if (messageTemp == "\"on\"") {
      digitalWrite(devicePin, HIGH);
      Serial.println("on");
    } else if (messageTemp == "\"off\"") {
      digitalWrite(devicePin, LOW);
      Serial.println("off");
    } else if (messageTemp == "\"state\"") {
      int state = digitalRead(devicePin);
      String topic = "/response/000001/state";
      client.publish(topic.c_str(), String(state).c_str());
      if(state==0)
      Serial.println("state=1");
      else
      Serial.println("state=0");
    }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting to MQTT broker...");
    if (client.connect(deviceID)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe("000001/command");  // 訂閱所有裝置的命令主題
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
