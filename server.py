# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer
import pygame.joystick as js
import pygame
import json

joysticks = {}
pygame.display.init()
js.init()
joystick = js.Joystick(0)

hostName = "0.0.0.0"
serverPort = 5000

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        pygame.event.pump()
        leftX = round(joystick.get_axis(0),2)
        leftY = round(joystick.get_axis(1),2)
        rightX = round(joystick.get_axis(2),2)
        rightY = round(joystick.get_axis(3),2)

        response = json.dumps({
            "lx": leftX,
            "ly": leftY,
            "rx": rightX,
            "ry": rightY
        })

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