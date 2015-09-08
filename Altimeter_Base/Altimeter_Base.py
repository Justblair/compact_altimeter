print('Hello World')
import pyttsx
import socket
import struct
import pygame
from pygame import mixer

UDP_IP = ''
UDP_PORT = 55056

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))
engine = pyttsx.init()
engine.setProperty("rate", 250)
engine.setProperty('name', 2)

oldData = 0.0
mixer.init()
mixer.music.load("D:\Music\A Hot Piece Of Grass\Hayseed Dixie - Duelling Banjos.mp3")
mixer.music.play()

while True:
    print "trying...."
    data, addr = sock.recvfrom(100) # buffer size is 1024 bytes
    if not data: 
        break
    if data [0] == 'A':
         # print "received message:", data , ":", data[2:100]
        if oldData < float (data[2:8]):
            oldData = float(data[2:8])
            engine.say("Highest")
    engine.say(data[2:100])
    engine.runAndWait()


