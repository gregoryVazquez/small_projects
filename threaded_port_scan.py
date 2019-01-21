import optparse
import socket
from threading import *
from sys import exit


s_lock = Semaphore(value=1)
open_ports = []

def scan_port(host:str, port:int) -> None:
    try:
        skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        skt.connect((host, port))
        s_lock.acquire()
        open_ports.append(f"[+] Port {port} is open")
    except Exception as e:
        pass
    finally:
        s_lock.release()
        skt.close()

def scan(host:str, what:str) -> None:
    if what == "well":
        START=1
        END=1023
        MSG="well known ports"
    elif what == "registered":
        START=1024
        END=49515
        MSG="registered ports"
    elif what == "dynamic":
        START=49516
        END=65535
        MSG="dynamic ports"
    try:
        ip_addr = socket.gethostbyname(host)
    except:
        print(f"[!] Can't resolve \"{host}\": Unkown host")
        return None
    try:
        tgt = socket.gethostbyaddr(ip_addr)
        print(f"\nScanning all {MSG} for {tgt[0]} ... this may take a while ...")
    except:
        print(f"\nScanning all {MSG} for {ip_addr} ... this may take a while ...")
    socket.setdefaulttimeout(1)
    for port in range(START, END):
        thread_scan = Thread(target=scan_port, args=(host, port))
        thread_scan.start()

if __name__ == '__main__':
    parser = optparse.OptionParser("Usage: ./threaded_port_scan.py -t [target] -p [well|registered|dynamic]")
    parser.add_option('-t', dest='host', type='string', help='select who to scan')
    parser.add_option('-p', dest='what', type='string', help='select between well-known, registered and dynamic ports')
    (options, args) = parser.parse_args()
    host = options.host
    what = options.what

    if host is None or what is None:
        print(parser.usage)
        exit(0)

    scan(host, what)
    if len(open_ports) == 0:
        print("No open ports were found...")
    else:
        for p in open_ports:
            print(p)
