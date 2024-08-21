import cv2
import mediapipe as mp
import socket
import threading
import time

# Mediapipe 솔루션 초기화
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

# 서버 설정
TCP_IP = '0.0.0.0'  # 모든 인터페이스에서 수신
TCP_PORT = 23458  # 서버 포트 번호
BUFFER_SIZE = 1024

# 클라이언트 설정
#CLIENT_TCP_IP = '192.168.1.59'  # 서버 IP 주소
CLIENT_TCP_IP = '192.168.1.51'
CLIENT_TCP_PORT = 23458  # 서버 포트 번호

# 클라이언트 소켓을 저장할 리스트
clients = []

def handle_client(conn, addr):
    """클라이언트의 요청을 처리하는 함수"""
    global clients
    clients.append(conn)  # 클라이언트를 리스트에 추가
    print(f"클라이언트와 연결되었습니다: {addr}")
    
    try:
        while True:  # 클라이언트와 데이터 통신
            data = conn.recv(BUFFER_SIZE).decode()
            if not data:
                break
            print(f"받은 데이터: {data}")

            # 모든 클라이언트에 데이터 전송
            for client in clients:
                if client != conn:  # 데이터를 보낸 클라이언트를 제외
                    client.send(data.encode())
    finally:
        conn.close()  # 클라이언트와의 연결 종료
        clients.remove(conn)  # 클라이언트 리스트에서 제거
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

def is_hand_below_knees(pose_landmarks, hand_landmarks, image_shape):
    """손이 무릎 아래에 있는지 확인하는 함수"""
    if pose_landmarks and hand_landmarks:
        left_knee = pose_landmarks.landmark[mp_pose.PoseLandmark.LEFT_KNEE]
        right_knee = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_KNEE]

        knee_y = max(left_knee.y, right_knee.y) * image_shape[0]  # 무릎의 y 좌표

        for hand_landmark in hand_landmarks:
            for point in hand_landmark.landmark:
                if point.y * image_shape[0] > knee_y:
                    return True
    return False

def is_hand_closed(hand_landmarks):
    """손이 쥐어졌는지 확인하는 함수"""
    if hand_landmarks:
        for hand_landmark in hand_landmarks:
            thumb_tip = hand_landmark.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_finger_tip = hand_landmark.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]

            distance = abs(thumb_tip.x - index_finger_tip.x) + abs(thumb_tip.y - index_finger_tip.y)

            # 손가락 끝과 엄지 사이의 거리가 특정 임계값 이하인 경우 손이 쥐어졌다고 간주
            if distance < 0.05:
                return True
    return False

def is_hand_open(hand_landmarks):
    """손이 펴졌는지 확인하는 함수"""
    if hand_landmarks:
        for hand_landmark in hand_landmarks:
            thumb_tip = hand_landmark.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_finger_tip = hand_landmark.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]

            distance = abs(thumb_tip.x - index_finger_tip.x) + abs(thumb_tip.y - index_finger_tip.y)

            # 손가락 끝과 엄지 사이의 거리가 특정 임계값 이상인 경우 손이 펴졌다고 간주
            if distance >= 0.1:  # 손가락 사이가 더 벌어지면 손이 펴졌다고 판단
                return True
    return False

def client_process():
    """클라이언트 소켓을 통해 손 동작을 분석하여 서버에 데이터를 전송"""
    previous_hand_closed = False

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((CLIENT_TCP_IP, CLIENT_TCP_PORT))

    cap = cv2.VideoCapture(0)

    with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose, \
        mp_hands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.5) as hands:

        while cap.isOpened():
            res, image = cap.read()
            if not res:
                print('웹캠에서 이미지를 가져오는 것을 실패')
                break
            
            # BGR 이미지를 RGB로 변환
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

            # 속도 개선을 위한 이미지 쓰기 불가능
            image.flags.writeable = False

            # 포즈와 손 인식
            pose_results = pose.process(image)
            hand_results = hands.process(image)

            image_shape = image.shape

            # 현재 손이 쥐어진 상태인지 확인
            hand_closed = is_hand_closed(hand_results.multi_hand_landmarks)
            hand_open = is_hand_open(hand_results.multi_hand_landmarks)

            # 손이 무릎 아래에 있고, 손이 이전에는 쥐어져 있었지만 지금은 펴진 상태인지 확인
            if is_hand_below_knees(pose_results.pose_landmarks, hand_results.multi_hand_landmarks, image_shape) and previous_hand_closed and hand_open:
                # 조건을 충족하면 TCP로 1 전송
                message = "1"
                client_socket.send(message.encode())
                print("Hand opened after being closed, sent:", message)
                time.sleep(2)
                break
            else:
                # 조건을 충족하지 않으면 TCP로 0 전송
                message = "0"
                client_socket.send(message.encode())
                print("No action detected, sent:", message)

            # 현재 상태를 이전 상태로 업데이트
            previous_hand_closed = hand_closed

            # 이미지 다시 쓰기로 지정
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            # 포즈와 손 랜드마크 그리기
            if pose_results.pose_landmarks:
                mp_drawing.draw_landmarks(image, pose_results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
            if hand_results.multi_hand_landmarks:
                for hand_landmarks in hand_results.multi_hand_landmarks:
                    mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # 이미지 좌우 반전 (글씨 포함)
            image = cv2.flip(image, 1)

            # 결과 화면에 출력
            cv2.imshow("Hand and Pose Detection", image)
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
