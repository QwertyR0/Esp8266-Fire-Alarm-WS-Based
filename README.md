# WebSocket-based fire alarm (esp8266)

This is a simple fire alarm system based on esp8266 and WebSocket. The system is designed to be used in a local network and it is not secure for the internet, although you might be able to make it secure by adding some security measures. It uses a flame sensor that detects flames using infrared light and sends a signal to the esp8266. The esp8266 waits for 2 seconds to see if the sensor continues to send signals, then sends a message to the server using WebSocket. The server then broadcasts a message to all of the clients. The clients (UI and other stuff) are currently being programmed and will be added soon. There's also a possibility to use an LM35 sensor to detect temperature. In the future, a more advanced version may be built for combining these two sensors. You might also want to add a buzzer for attention. I would recommend using a good quality USB cable while testing to prevent weird issues with connection.

### Notes:
My main aim was to make the smallest functional fire alram - flame detecter possible so it was obvious to use an Esp8266-01 which I owned. but I didn't have any 3.3v supplies laying around which forced me to use a nodemcu instead (Nodemcu has a built-in voltage regulator, like one that I need to order for my esp8266-01 to work with 5v supplies).

### [Click here for the library for WebSocket server.](https://github.com/Links2004/arduinoWebSockets)

### protoype 1:
![preview](https://github.com/QwertyR0/Esp8266-Fire-Alarm-WS-Based/blob/main/preview.jpg?raw=true)
