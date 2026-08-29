import socket
import time
import random
import threading

def send_request(request_id):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(('127.0.0.1', 8080))
            s.sendall(b"GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")
            data = s.recv(1024)
            print(f"Request {request_id}: {data.decode('utf-8', errors='ignore').splitlines()[0]} - {data.decode('utf-8', errors='ignore').splitlines()[-1]}")
    except Exception as e:
        print(f"Request {request_id} failed: {e}")

if __name__ == "__main__":
    num_requests = 20
    print(f"Sending {num_requests} requests to Load Balancer at 8080...")
    
    threads = []
    for i in range(num_requests):
        t = threading.Thread(target=send_request, args=(i,))
        threads.append(t)
        t.start()
        time.sleep(random.uniform(0.1, 0.5)) # Random delay between requests

    for t in threads:
        t.join()
    
    print("Simulation complete.")
