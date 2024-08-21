import cv2
import mediapipe as mp
import socket
import threading

# Mediapipe 솔루션 초기화
mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands
mp_pose = mp.solutions.pose

# 서버 설정
TCP_IP = '0.0.0.0'  # 모든 인터페이스에서 수신
TCP_PORT = 65432  # 서버 포트 번호
BUFFER_SIZE = 1024

# 클라이언트 설정
# CLIENT_TCP_IP = '192.168.1.59'  # 서버 IP 주소
CLIENT_TCP_IP = '192.168.1.59'
CLIENT_TCP_PORT = 23458  # 서버 포트 번호

# 클라이언트 소켓을 저장할 리스트
clients = []
clients_lock = threading.Lock()

def is_hand_above_elbow(pose_landmarks, hand_landmarks, hand_label):
    """손이 해당 팔꿈치보다 위에 있는지 확인하는 함수"""
    if pose_landmarks and hand_landmarks:
        wrist = hand_landmarks.landmark[mp_hands.HandLandmark.WRIST]
        
        # 왼손과 오른손에 맞는 팔꿈치 위치를 가져옵니다.
        if hand_label == "Right":  # 실제 오른손에 해당하는 경우
            elbow = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_ELBOW]
        else:  # 실제 왼손에 해당하는 경우
            elbow = pose_landmarks.landmark[mp_pose.PoseLandmark.LEFT_ELBOW]

        # 손목이 팔꿈치보다 위에 있으면 True 반환
        return wrist.y < elbow.y
    return False

def is_hand_above_abdomen(pose_landmarks, hand_landmarks):
    """손이 배 위에 있는지 확인하는 함수"""
    if pose_landmarks and hand_landmarks:
        right_hip = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_HIP]
        left_hip = pose_landmarks.landmark[mp_pose.PoseLandmark.LEFT_HIP]
        abdomen_y = min(right_hip.y, left_hip.y)  # 배의 y 좌표를 골반의 y 좌표로 설정

        for point in hand_landmarks.landmark:
            if point.y < abdomen_y:  # 손의 y 좌표가 배보다 위에 있는지 확인
                return True
    return False

def detect_pouring_motion(hand_landmarks):
    """손이 약물을 붓는 동작인지 확인"""
    if hand_landmarks:
        wrist = hand_landmarks.landmark[mp_hands.HandLandmark.WRIST]
        index_finger_tip = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
        
        # 손목이 인덱스 손가락 끝보다 높이 위치해 있는지 확인
        return wrist.y < index_finger_tip.y
    return False

def is_hand_closed(hand_landmarks):
    """손이 주먹을 쥐었는지 확인하는 함수"""
    if hand_landmarks:
        thumb_tip = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
        index_finger_tip = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
        middle_finger_tip = hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP]
        
        # 손가락 끝이 서로 가까워지면 주먹을 쥔 것으로 간주
        return (
            abs(thumb_tip.x - index_finger_tip.x) < 0.05 and
            abs(thumb_tip.y - index_finger_tip.y) < 0.05 and
            abs(middle_finger_tip.x - index_finger_tip.x) < 0.05 and
            abs(middle_finger_tip.y - index_finger_tip.y) < 0.05
        )
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

    with mp_hands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.5) as hands, \
         mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose:

        hand_closed = False  # 주먹을 쥐었는지 여부를 추적하는 변수

        while cap.isOpened():
            res, image = cap.read()
            if not res:
                print('웹캠에서 이미지를 가져오는 것을 실패')
                break
            
            # BGR 이미지를 RGB로 변환 (좌우 반전 없음)
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            image.flags.writeable = False

            # 손과 포즈 인식 수행 (원본 이미지 사용)
            hand_results = hands.process(image)
            pose_results = pose.process(image)

            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            if hand_results.multi_hand_landmarks and pose_results.pose_landmarks:
                for hand_landmarks, handedness in zip(hand_results.multi_hand_landmarks, hand_results.multi_handedness):
                    # 손이 왼손인지 오른손인지 확인
                    hand_label = handedness.classification[0].label

                    # 주먹을 쥔 상태인지 확인
                    if is_hand_closed(hand_landmarks):
                        hand_closed = True

                    if hand_closed and is_hand_above_abdomen(pose_results.pose_landmarks, hand_landmarks) and \
                       is_hand_above_elbow(pose_results.pose_landmarks, hand_landmarks, hand_label) and \
                       detect_pouring_motion(hand_landmarks):
                        if hand_label == "Right":  # 왼손으로 붓는 동작을 감지
                            message = "1"  # 왼손이면 "1" 전송
                            client_socket.send(message.encode())
                            print("왼손 동작 감지, 1 전송")
                            break
                        elif hand_label == "Left":  # 오른손으로 붓는 동작을 감지
                            message = "2"  # 오른손이면 "2" 전송
                            client_socket.send(message.encode())
                            print("오른손 동작 감지, 2 전송")
                            break
                        hand_closed = False  # 동작이 완료되면 주먹 상태를 초기화
                    
                    # 이미지 랜드마크 그리기
                    mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
                
                # 포즈 랜드마크 그리기
                mp_drawing.draw_landmarks(image, pose_results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
            
            # 이미지를 좌우 반전하여 화면에 출력
            flipped_image = cv2.flip(image, 1)
            
            # 결과 이미지 출력
            cv2.imshow("Hand and Pose Tracking", flipped_image)
            if cv2.waitKey(1) == ord('q'):
                break

        # 루프가 끝나면 웹캠과 창을 정상적으로 종료합니다.
        cap.release()
        cv2.destroyAllWindows()
        client_socket.close()

# 서버를 스레드로 실행
server_thread = threading.Thread(target=start_server)
server_thread.start()

# 클라이언트 프로세스 실행
client_process()

# 서버 스레드가 종료될 때까지 대기
server_thread.join()
