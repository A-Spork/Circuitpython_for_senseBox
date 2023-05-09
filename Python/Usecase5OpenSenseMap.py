# code nicht lauffähig

import board
import busio
import time
import adafruit_scd30
import wifi
import socketpool as socket
import ssl






# Wi-Fi-Verbindung konfigurieren
WIFI_SSID = "SSID"
WIFI_PASSWORD = "Password"
wifi.radio.connect(WIFI_SSID, WIFI_PASSWORD)

# Sensor initialisieren
i2c = busio.I2C(board.SCL, board.SDA)
airSensor = adafruit_scd30.SCD30(i2c)

# SenseBox-Parameter konfigurieren
SENSOR_IDRID = "sensorID"
SENSEBOX_ID = "senseBoxID"
SERVER = "ingress.opensensemap.org"
Authorization = "authentificationCode"
CONTENT_LENGTH_MULTIPLIER = 35

# SSL-Verbindung konfigurieren
pool = socket.SocketPool(wifi.radio)
ssl_context = ssl.create_default_context()

def add_measurement(sensor_id, value):
    global measurements
    measurements.append((sensor_id, value))

def write_measurements_to_socket(sock):
    global measurements
    for measurement in measurements:
        buffer = "{},{}\n".format(measurement[0], measurement[1])
        print(5)
        sock.send(buffer.encode("utf-8"))
    measurements = []

def submit_values():
    global measurements
    global pool
    global ssl_context
    measurements_length = len(measurements)
    if measurements_length == 0:
        return
    # Socket-Verbindung herstellen
    sock = pool.socket()
    addr_info = pool.getaddrinfo(SERVER, 443)
    addr = addr_info[0][-1]
    sock.connect(addr)
    # SSL-Verbindung herstellen
    sock = ssl_context.wrap_socket(sock, server_hostname=SERVER)
    # HTTP-POST-Anfrage senden
    content_length = measurements_length * CONTENT_LENGTH_MULTIPLIER
    buffer = "POST /boxes/{}/data HTTP/1.1 \r \n Host: {} \r \n Authorization: "+Authorization" \r \n Content-Type: application/json; charset=utf-8 \r \n Content-Length: {} \r \n Connection: close \r \n \r \n ".format(
        SENSEBOX_ID, SERVER, content_length)
    sock.send(buffer.encode("utf-8"))
    # Messwerte senden
    write_measurements_to_socket(sock)
    # Leere Zeile senden, um die Anfrage abzuschließen
    sock.send(b"\r\n")
    # Auf die Antwort warten
    response = sock.recv_into(1024)
    while response:
        response = sock.recv_into(1024)
    # Verbindung schließen
    sock.close()

# Messungen alle 10 Sekunden durchführen und senden
INTERVAL = 10.0
last_time = time.monotonic()
measurements = []
while True:
    if time.monotonic() - last_time >= INTERVAL:
        last_time = time.monotonic()
        add_measurement(SENSOR_IDRID, airSensor.CO2)
        submit_values()



# Define the Wi-Fi interface and create a socket pool
wifi.radio.connect(ssid, password)
pool = socket.SocketPool(wifi.radio)

# Define the openSensemap server
SERVER = "ingress.opensensemap.org"
PORT = 443

# Define the SCD30 sensor
i2c = busio.I2C(board.SCL, board.SDA)
scd = adafruit_scd30.SCD30(i2c)

# Define the time interval for measurements
INTERVAL = 10

# Define the number of measurements
NUM_MEASUREMENTS = 3

# Define the buffer length multiplier
LENGTH_MULTIPLIER = 35

# Define the measurements array
measurements = []

def add_measurement(sensor_id, value):
    measurement = {"sensor_id": sensor_id, "value": value}
    measurements.append(measurement)

def write_measurements_to_client(client):
    for measurement in measurements:
        buffer = "{},{}\n".format(measurement["sensor_id"], measurement["value"])
        client.send(buffer.encode("utf-8"))

def submit_values():
    # Connect to the server
    sock = pool.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_SEC)
    sock.settimeout(5)
    wrapped_socket = ssl.create_default_context().wrap_socket(sock, server_hostname=SERVER)
    wrapped_socket.connect((SERVER, PORT))

    # Construct the HTTP POST request
    request = "POST /boxes/{}/data HTTP/1.1\r\n".format(sensebox_id)
    request += "Host: {}\r\n".format(SERVER)
    request += "Authorization: {}\r\n".format(access_token)
    request += "Content-Type: text/csv\r\n"
    request += "Connection: close\r\n"
    content_length = len(measurements) * LENGTH_MULTIPLIER
    request += "Content-Length: {}\r\n".format(content_length)
    request += "\r\n"

    # Send the request and measurements
    wrapped_socket.send(request.encode("utf-8"))
    write_measurements_to_client(wrapped_socket)

    # Wait for the response
    response = wrapped_socket.recv(4096)
    wrapped_socket.close()

def main():
    last_time = 0
    while True:
        current_time = time.monotonic()
        if current_time > last_time + INTERVAL:
            last_time = current_time
            co2 = scd.CO2
            add_measurement(sensor_id + "_co2", co2)
            submit_values()
        time.sleep(0.1)

if __name__ == "__main__":
    main()
