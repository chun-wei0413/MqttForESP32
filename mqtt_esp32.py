from flask import Flask, request
import paho.mqtt.client as mqtt

app = Flask(__name__)

mqtt_broker = "140.136.151.74"
mqtt_port = 1883
device_id = "esp32"  # 與ESP32程式中的deviceID相同

client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker with result code " + str(rc))
    # 當連接到 MQTT broker 後，訂閱指定的主題
    client.subscribe("/response/esp32/state")

def on_publish(client, userdata, mid):
    print("Message published")
def on_message(client, userdata, msg):
    # 當收到 MQTT 訊息時的處理函式
    if msg.topic == "/response/esp32/state":
        # 根據收到的狀態進行相應的處理
        state = msg.payload.decode("utf-8")
        # 在這裡可以根據狀態執行相應的邏輯
        if state == "0":
            print("Received state:on")
        elif state == "1":
            print("Received state:off")

@app.route("/turn-on")
def turn_on():
    command_topic = f"{device_id}/command"
    client.publish(command_topic, "off")
    return "Turned on"

@app.route("/turn-off")
def turn_off():
    command_topic = f"{device_id}/command"
    client.publish(command_topic, "on")
    return "Turned off"

@app.route("/get-state")
def get_state():
    command_topic = f"{device_id}/command"
    client.publish(command_topic, "state")
    return "Requested state"

if __name__ == '__main__':
    client.on_connect = on_connect
    client.on_publish = on_publish
    client.on_message = on_message

    client.connect(mqtt_broker, mqtt_port, 60)
    client.loop_start()

    app.run(host='140.136.151.74', port=8080)
