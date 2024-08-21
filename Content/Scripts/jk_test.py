import socket

host = "0.0.0.0"
port = 23457

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as socket_server:
    # 서버에서 클라이언트 연결을 기다리는 중
    socket_server.bind((host, port))

    # 클라이언트에서 요청하고 난 후
    socket_server.listen()
    print("서버가 연결되었습니다.", host, port)

    # 클라이언트의 소켓 객체와 주소를 반환
    conn, addr = socket_server.accept()

    with conn:
        print("연결된 소켓은 :", addr)

        while True:
            try:
                data = str(1).encode()
                conn.sendall(data)
            except KeyboardInterrupt:
                print("-----------")
                socket_server.close()
                break