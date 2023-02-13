import pygame.joystick as js
import pygame
import json
import time

joysticks = {}
pygame.display.init()
js.init()

if js.get_init():
    while True:
        pygame.event.pump()
        joystick = js.Joystick(0)
        leftX = round(joystick.get_axis(0),2)
        leftY = round(joystick.get_axis(1),2)
        rightX = round(joystick.get_axis(2),2)
        rightY = round(joystick.get_axis(3),2)
        
        f = open("state.json", "r+")
        f.truncate(0)
        f.write(json.dumps({
            "lx": leftX,
            "ly": leftY,
            "rx": rightX,
            "ry": rightY
        }))
        f.close()
        time.sleep(0.1)
        
