import socket
import threading
import sys

def start_server(port):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.bind(('0.0.0.0', port))
            s.listen(5)
            print(f"Backend server listening on port {port}...")
            while True:
                conn, addr = s.accept()
                with conn:
                    request = conn.recv(1024).decode('utf-8', errors='ignore')
                    print(f"[{port}] Received request from {addr}")
                    response = f"HTTP/1.1 200 OK\r\nContent-Length: 18\r\n\r\nHello from port {port}"
                    conn.sendall(response.encode('utf-8'))
    except Exception as e:
        print(f"Error on port {port}: {e}")

if __name__ == "__main__":
    ports = [8001, 8002, 8003]
    threads = []
    try:
        for port in ports:
            t = threading.Thread(target=start_server, args=(port,), daemon=True)
            t.start()
            threads.append(t)
        print("All backends are running. Press Ctrl+C to stop.")
        import time
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nStopping backends...")
        sys.exit(0)
