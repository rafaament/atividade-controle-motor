import paho.mqtt.client as mqtt
import os

broker_address = os.getenv('mqtt_broker_address')
port = 1883
topic_motor_direction = "motor_direction"
topic_motor_intensity = "motor_intensity"

def on_connect(client, userdata, flags, rc):
    print("Conectado ao MQTT Broker com código de resultado: " + str(rc))

def on_message(client, userdata, msg):
    print("Mensagem recebida: " + msg.topic + " " + str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker_address, port)

client.subscribe(topic_motor_direction)
client.subscribe(topic_motor_intensity)

def enviar_sentido(sentido):
    client.publish(topic_motor_direction, str(sentido))

def enviar_intensidade(intensidade):
    client.publish(topic_motor_intensity, str(intensidade))

def teste_sentido_0():
    print("Enviando sentido horário")
    enviar_sentido(0)

def teste_sentido_1():
    print("Enviando sentido anti-horário")
    enviar_sentido(1)

def teste_intensidade():
    print("Enviando intensidade 50")
    enviar_intensidade(50)

teste_sentido_0()
teste_sentido_1()
teste_intensidade()

client.loop_forever()
