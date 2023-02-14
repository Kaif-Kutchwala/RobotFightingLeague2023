# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer
import pygame.joystick as js
import pygame
import json

pygame.display.init()
js.init()
joystick = js.Joystick(0)

hostName = "0.0.0.0"
serverPort = 4000


def map_range(OldValue, OldMin, OldMax, NewMin, NewMax):
    OldRange = (OldMax - OldMin)
    NewRange = (NewMax - NewMin)
    NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
    return NewValue


class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        pygame.event.pump()
        x = round(joystick.get_axis(0), 1)
        y = round(joystick.get_axis(1), 1) * -1

        IN1 = 0  # motor1
        IN2 = 0  # motor1
        IN3 = 0  # motor2
        IN4 = 0  # motor2
        ENA = 0  # motor 1 speed
        ENB = 0  # motor 2 speed

        if y > 0.1:
            IN1 = 1
            IN3 = 1
            if x > 0.1:
                ENA = map_range((x**2 + y**2)**0.5, 0, 2**0.5, 0, 255)
                ENB = map_range(y, 0, 2**0.5, 0, 255)
            elif x < -0.1:
                ENA = map_range(y, 0, 2**0.5, 0, 255)
                ENB = map_range((x**2 + y**2)**0.5, 0, 2**0.5, 0, 255)
            else:
                ENA = map_range(y, 0, 1, 0, 255)
                # max is 1 because both are inverse i.e. when y is 1 x is 0
                ENB = map_range(y, 0, 1, 0, 255)
        elif y < -0.1:
            IN2 = 1
            IN4 = 1
            if x > 0.1:
                ENA = map_range((x**2 + y**2)**0.5, 0, 2**0.5, 0, 255)
                ENB = map_range(abs(y), 0, 2**0.5, 0, 255)
            elif x < -0.1:
                ENA = map_range(abs(y), 0, 2**0.5, 0, 255)
                ENB = map_range((x**2 + y**2)**0.5, 0, 2**0.5, 0, 255)
            else:
                ENA = map_range(abs(y), 0, 2**0.5, 0, 255)
                ENB = map_range(abs(y), 0, 2**0.5, 0, 255)
        else:
            if x < -0.1 or x > 0.1:
                ENA = map_range(abs(x), 0, 1, 0, 255)
                ENB = map_range(abs(x), 0, 1, 0, 255)

            if x > 0.1:
                IN1 = 1
                IN4 = 1

            elif x < -0.1:
                IN2 = 1
                IN3 = 1

        response = json.dumps({
            "IN1": IN1,
            "IN2": IN2,
            "IN3": IN3,
            "IN4": IN4,
            "ENA": ENA,
            "ENB": ENB,
            "x": x,
            "y": y
        })

        print(response)

        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        self.wfile.write(bytes(response, "utf-8"))


if __name__ == "__main__":
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.allow_reuse_address = True
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
