import cv2
import mediapipe as mp
import requests  # 서버에 데이터를 전송하기 위해 추가

# Mediapipe 솔루션 초기화
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands
mp_face_mesh = mp.solutions.face_mesh
mp_drawing = mp.solutions.drawing_utils

# 입술의 랜드마크 인덱스를 정의
UPPER_LIP_INDICES = [61, 185, 40, 39, 37, 0, 267, 269, 270, 409, 415]
LOWER_LIP_INDICES = [146, 91, 181, 84, 17, 314, 405, 321, 375, 291, 308]

# 웹캠 설정
cap = cv2.VideoCapture(0)

# 서버 URL 설정
server_url = "http://127.0.0.1:8000/eating"

def is_mouth_open(landmarks, image_shape):
    """입이 열려있는지 확인하는 함수"""
    top_lip = landmarks[13]  # 윗입술 중앙
    bottom_lip = landmarks[14]  # 아랫입술 중앙

    top_lip_y = int(top_lip.y * image_shape[0])
    bottom_lip_y = int(bottom_lip.y * image_shape[0])

    # 입술 간의 거리 계산
    lip_distance = bottom_lip_y - top_lip_y

    # 특정 임계값 이상으로 입이 열려있으면 먹는 행동으로 간주
    return lip_distance > 20

def are_hands_above_shoulders(pose_landmarks, hand_landmarks, image_shape):
    """손이 어깨 위에 있는지 확인하는 함수"""
    if pose_landmarks and hand_landmarks:
        left_shoulder = pose_landmarks.landmark[mp_pose.PoseLandmark.LEFT_SHOULDER]
        right_shoulder = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER]

        shoulder_y = min(left_shoulder.y, right_shoulder.y) * image_shape[0]  # 어깨의 y 좌표

        for hand_landmark in hand_landmarks:
            for point in hand_landmark.landmark:
                if point.y * image_shape[0] < shoulder_y:
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

with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose, \
     mp_hands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.5) as hands, \
     mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5) as face_mesh:

    while cap.isOpened():
        res, image = cap.read()
        if not res:
            print('웹캠에서 이미지를 가져오는 것을 실패')
            break
        
        # BGR 이미지를 RGB로 변환
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # 속도 개선을 위한 이미지 쓰기 불가능
        image.flags.writeable = False

        # 포즈, 손 및 얼굴 인식
        pose_results = pose.process(image)
        hands_results = hands.process(image)
        face_results = face_mesh.process(image)

        # 이미지 다시 쓰기로 지정
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        if face_results.multi_face_landmarks and pose_results.pose_landmarks and hands_results.multi_hand_landmarks:
            for face_landmarks in face_results.multi_face_landmarks:
                # 입이 열려 있는지 확인
                if is_mouth_open(face_landmarks.landmark, image.shape):
                    # 손이 어깨 위에 있고 쥐어져 있는지 확인
                    if are_hands_above_shoulders(pose_results.pose_landmarks, hands_results.multi_hand_landmarks, image.shape):
                        if is_hand_closed(hands_results.multi_hand_landmarks):
                            cv2.putText(image, "Clear", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 3, cv2.LINE_AA)
                            # 서버로 1의 값을 POST 방식으로 전송
                            response = requests.post(server_url, json={"value": 1})
                            print("서버 응답:", response.json())
                        else:
                            # 손이 쥐어지지 않았을 때 0을 보냄
                            response = requests.post(server_url, json={"value": 0})
                            print("서버 응답:", response.json())
                    else:
                        # 손이 어깨 위에 있지 않을 때 0을 보냄
                        response = requests.post(server_url, json={"value": 0})
                        print("서버 응답:", response.json())
                else:
                    # 입이 열려 있지 않을 때 0을 보냄
                    response = requests.post(server_url, json={"value": 0})
                    print("서버 응답:", response.json())
        else:
            # 얼굴, 포즈, 손 중 하나라도 감지되지 않았을 때 0을 보냄
            response = requests.post(server_url, json={"value": 0})
            print("서버 응답:", response.json())

        # 포즈와 손 랜드마크 그리기
        if pose_results.pose_landmarks:
            mp_drawing.draw_landmarks(
                image, pose_results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

        if hands_results.multi_hand_landmarks:
            for hand_landmarks in hands_results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

        # 이미지 좌우 반전 (글씨 포함)
        image = cv2.flip(image, 1)

        # 결과 화면에 출력
        cv2.imshow("frame", image)
        if cv2.waitKey(1) == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()
