import cv2
import mediapipe as mp
import requests  # 서버에 데이터를 전송하기 위해 추가

# Mediapipe 솔루션 초기화
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

# 웹캠 설정
cap = cv2.VideoCapture(0)

# 서버 URL 설정
server_url = "http://127.0.0.1:8000/cleaning"

# 손의 움직임 추적을 위한 변수 초기화
previous_left_hand_x = None
previous_right_hand_x = None
shake_count = 0
shake_threshold = 0.05  # 손 움직임이 이 정도 이상이면 흔들린 것으로 간주
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
                if shake_count >= 4:  # 양손이 각각 두 번씩 흔들린 경우
                    cv2.putText(image, "Clear", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 3, cv2.LINE_AA)
                    shake_count = 0  # 초기화
                    shake_direction = 0  # 초기화

                    # 서버로 1의 값을 POST 방식으로 전송
                    response = requests.post(server_url, json={"value": 1})
                    print("서버 응답:", response.json())
                else:
                    # 행동이 완성되지 않았을 때 0을 전송
                    response = requests.post(server_url, json={"value": 0})
                    print("서버 응답:", response.json())
            else:
                # 조건이 충족되지 않았을 때 0을 전송
                response = requests.post(server_url, json={"value": 0})
                print("서버 응답:", response.json())
        else:
            # 손이 감지되지 않았을 때 0을 전송
            response = requests.post(server_url, json={"value": 0})
            print("서버 응답:", response.json())

        # 이미지 좌우 반전 (글씨 포함)
        image = cv2.flip(image, 1)

        # 결과 화면에 출력
        cv2.imshow("frame", image)
        if cv2.waitKey(1) == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()
