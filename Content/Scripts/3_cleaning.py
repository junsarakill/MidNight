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
TCP_IP = '0.0.0.0' # 모든 인터페이스에서 수신
TCP_PORT = 65432 # 서버 포트 번호
BUFFER_SIZE = 1024

# 클라이언트 설정
#CLIENT_TCP_IP = '192.168.1.59' # 서버 IP 주소
CLIENT_TCP_IP = '127.0.0.1'
CLIENT_TCP_PORT = 65432 # 서버 포트 번호

# 손의 움직임 추적을 위한 변수 초기화
shake_threshold = 0.05  # 손 움직임이 이 정도 이상이면 흔들린 것으로 간주
previous_left_hand_x = None
previous_right_hand_x = None
shake_count = 0
shake_direction = 0  # 1이면 오른쪽, -1이면 왼쪽, 0이면 초기상태

def is_hand_above_hips(pose_landmarks, hand_landmarks, image_shape):
    """손이 골반 위에 있는지 확인하는 함수"""
    if pose_landmarks and hand_landmarks:
        left_hip = pose_landmarks.landmark[mp_pose.PoseLandmark.LEFT_HIP]
        right_hip = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_HIP]

        hip_y = min(left_hip.y, right_hip.y) * image_shape[0]  # 골반의 y 좌표

        for hand_landmark in hand_landmarks:
            for point in hand_landmark.landmark:
                if point.y * image_shape[0] < hip_y:
                    return True
    return False

def is_hand_closed(hand_landmarks):
    """손이 쥐어졌는지 확인하는 함수"""
    if hand_landmarks:
        for hand_landmark in hand_landmarks:
            thumb_tip = hand_landmark.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_finger_tip = hand_landmark.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]

            distance = abs(thumb_tip.x - index_finger_tip.x) + abs(thumb_tip.y - index_finger_tip.y)

            if distance < 0.05:
                return True
    return False

def detect_shake(current_x, previous_x, shake_count, shake_direction):
    """손의 좌우 흔들림을 감지"""
    if previous_x is not None:
        movement = current_x - previous_x
        if abs(movement) > shake_threshold:
            if movement > 0 and shake_direction <= 0:
                shake_direction = 1
                shake_count += 1
            elif movement < 0 and shake_direction >= 0:
                shake_direction = -1
                shake_count += 1
    return shake_count, shake_direction

def start_server():
    """서버를 시작하여 클라이언트로부터 데이터를 수신"""
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((TCP_IP, TCP_PORT))
    server_socket.listen(1)
    print("서버가 시작되었습니다. 클라이언트 연결을 기다리는 중...")
    
    conn, addr = server_socket.accept()
    print(f"클라이언트와 연결되었습니다: {addr}")
    
    while True:
        data = conn.recv(BUFFER_SIZE).decode()
        if not data:
            break
        print(f"받은 데이터: {data}")
        if data == "1":
            print("동작이 감지되어 프로그램을 종료합니다.")
            break
    
    conn.close()
    server_socket.close()

def client_process():
    """클라이언트 프로세스"""
    global previous_left_hand_x, previous_right_hand_x, shake_count, shake_direction

    # 클라이언트 소켓 생성
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((CLIENT_TCP_IP, CLIENT_TCP_PORT))

    # 웹캠 설정
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
            hands_results = hands.process(image)

            # 이미지 다시 쓰기로 지정
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            # 포즈와 손 랜드마크 그리기
            if pose_results.pose_landmarks:
                mp_drawing.draw_landmarks(
                    image, pose_results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

            if hands_results.multi_hand_landmarks:
                for hand_landmarks in hands_results.multi_hand_landmarks:
                    mp_drawing.draw_landmarks(
                        image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
                
                # 양손이 골반 위에 있고 쥐어진 상태인지 확인
                if (is_hand_above_hips(pose_results.pose_landmarks, hands_results.multi_hand_landmarks, image.shape) and 
                    is_hand_closed(hands_results.multi_hand_landmarks)):

                    # 양손의 x 좌표를 추출 (각 손이 감지된 경우에만)
                    if len(hands_results.multi_hand_landmarks) > 0:
                        left_hand_x = hands_results.multi_hand_landmarks[0].landmark[mp_hands.HandLandmark.WRIST].x
                        shake_count, shake_direction = detect_shake(left_hand_x, previous_left_hand_x, shake_count, shake_direction)
                        previous_left_hand_x = left_hand_x

                    if len(hands_results.multi_hand_landmarks) > 1:
                        right_hand_x = hands_results.multi_hand_landmarks[1].landmark[mp_hands.HandLandmark.WRIST].x
                        shake_count, shake_direction = detect_shake(right_hand_x, previous_right_hand_x, shake_count, shake_direction)
                        previous_right_hand_x = right_hand_x

                    # 흔들림이 두 번 감지되면 "Clear" 표시
                    if shake_count >= 8:  # 양손이 각각 두 번씩 흔들린 경우
                        cv2.putText(image, "Clear", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 3, cv2.LINE_AA)
                        shake_count = 0  # 초기화
                        shake_direction = 0  # 초기화

                        # TCP로 1 전송
                        message = "1"
                        client_socket.send(message.encode())
                        print("Motion detected, sent:", message)
                        time.sleep(2)
                        break

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
