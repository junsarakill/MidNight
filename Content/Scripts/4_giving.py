import cv2
import mediapipe as mp
import socket
import threading
import time

# Mediapipe 솔루션 초기화
mp_pose = mp.solutions.pose
mp_drawing = mp.solutions.drawing_utils

# 서버 설정
TCP_IP = '0.0.0.0' # 모든 인터페이스에서 수신
TCP_PORT = 23458 # 서버 포트 번호
BUFFER_SIZE = 1024

# 클라이언트 설정
#CLIENT_TCP_IP = '192.168.1.59'  # 서버 IP 주소
CLIENT_TCP_IP = '192.168.1.51'
CLIENT_TCP_PORT = 23458  # 서버 포트 번호

# 클라이언트 리스트를 관리하기 위한 리스트
clients = []
clients_lock = threading.Lock()

def is_right_arm_aligned(pose_landmarks, image_shape):
    """오른쪽 어깨, 팔꿈치, 손목이 동일 선상에 있는지 확인"""
    if pose_landmarks:
        right_shoulder = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER]
        right_elbow = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_ELBOW]
        right_wrist = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_WRIST]

        # 어깨, 팔꿈치, 손목의 x 좌표가 거의 동일한지 확인
        right_alignment = (abs(right_shoulder.x - right_elbow.x) < 0.02) and \
                          (abs(right_elbow.x - right_wrist.x) < 0.02)

        return right_alignment
    return False

def is_hand_above_abdomen(pose_landmarks, image_shape):
    """손이 배 위에 있는지 확인하는 함수"""
    if pose_landmarks:
        right_hip = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_HIP]
        right_shoulder = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER]
        right_wrist = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_WRIST]

        # 배의 y 좌표를 어깨와 골반의 중간지점으로 설정
        abdomen_y = (right_shoulder.y + right_hip.y) / 2

        # 손목의 y 좌표가 골반과 어깨 사이의 중간지점(배)보다 위에 있는지 확인
        if right_wrist.y < abdomen_y:
            return True
    return False

def handle_client(conn, addr):
    """클라이언트의 요청을 처리하는 함수"""
    global clients
    with clients_lock:
        clients.append(conn)  # 클라이언트를 리스트에 추가
    print(f"클라이언트와 연결되었습니다: {addr}")
    
    try:
        while True:  # 클라이언트와 데이터 통신
            data = conn.recv(BUFFER_SIZE).decode()
            if not data:
                break
            print(f"받은 데이터: {data}")

            # 모든 클라이언트에 데이터 전송
            with clients_lock:
                for client in clients:
                    if client != conn:  # 데이터를 보낸 클라이언트를 제외
                        client.send(data.encode())
    finally:
        with clients_lock:
            clients.remove(conn)  # 클라이언트 리스트에서 제거
        conn.close()  # 클라이언트와의 연결 종료
        print(f"클라이언트와의 연결이 종료되었습니다: {addr}")

def start_server():
    """서버를 시작하여 클라이언트로부터 데이터를 수신"""
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((TCP_IP, TCP_PORT))
    server_socket.listen(5)  # 최대 5개의 클라이언트를 대기
    print("서버가 시작되었습니다. 클라이언트 연결을 기다리는 중...")
    
    while True:
        conn, addr = server_socket.accept()  # 클라이언트 연결 수립
        # 각 클라이언트에 대해 새로운 스레드 생성
        threading.Thread(target=handle_client, args=(conn, addr)).start()

    server_socket.close()  # 서버 소켓 종료


def client_process():
    """클라이언트 프로세스"""
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((CLIENT_TCP_IP, CLIENT_TCP_PORT))

    cap = cv2.VideoCapture(0)

    with mp_pose.Pose(min_detection_confidence=0.95, min_tracking_confidence=0.95) as pose:

        while cap.isOpened():
            res, image = cap.read()
            if not res:
                print('웹캠에서 이미지를 가져오는 것을 실패')
                break
            
            # BGR 이미지를 RGB로 변환
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

            # 속도 개선을 위한 이미지 쓰기 불가능
            image.flags.writeable = False

            # 포즈 인식
            pose_results = pose.process(image)

            # 이미지 다시 쓰기로 지정
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            # 포즈 랜드마크 그리기
            if pose_results.pose_landmarks:
                mp_drawing.draw_landmarks(
                    image, pose_results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
                
                # 손이 배 위에 있고, 팔이 펴져 있는지 확인
                if is_hand_above_abdomen(pose_results.pose_landmarks, image.shape) and \
                   is_right_arm_aligned(pose_results.pose_landmarks, image.shape):
                    cv2.putText(image, "Action Detected", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 3, cv2.LINE_AA)
                    # TCP로 1 전송
                    message = "1"
                    client_socket.send(message.encode())
                    print("동작 감지, 전송됨:", message)
                    break
                else:
                    cv2.putText(image, "", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 0, 255), 3, cv2.LINE_AA)
                    # TCP로 0 전송
                    message = "0"
                    client_socket.send(message.encode())
                    print("No Action, 전송됨:", message)

            # 이미지 좌우 반전 (글씨 포함)
            image = cv2.flip(image, 1)

            # 결과 화면에 출력
            cv2.imshow("frame", image)
            if cv2.waitKey(1) == ord('q'):
                break

    cap.release()
    client_socket.close()
    cv2.destroyAllWindows()

# 서버를 스레드로 실행
server_thread = threading.Thread(target=start_server)
server_thread.start()

# 클라이언트 프로세스 실행
client_process()

# 서버 스레드가 종료될 때까지 대기
server_thread.join()
