import requests
from time import sleep

while True:
    url = "http://192.168.137.1:4000"

    res = requests.get(url)

    print(res.text)
    sleep(0.5)
