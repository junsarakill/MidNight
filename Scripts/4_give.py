import cv2
import mediapipe as mp
import requests  # 서버로 데이터를 전송하기 위해 추가

# Mediapipe 솔루션 초기화
mp_pose = mp.solutions.pose
mp_drawing = mp.solutions.drawing_utils

# 웹캠 설정
cap = cv2.VideoCapture(0)

# 서버 URL 설정
server_url = "http://127.0.0.1:8000/giving"

# 동일 선상에 있는지 확인하는 임계값
alignment_threshold = 0.05  # x 좌표 차이의 허용 오차

def is_right_arm_aligned(pose_landmarks, image_shape):
    """오른쪽 어깨, 팔꿈치, 손목이 동일 선상에 있는지 확인"""
    if pose_landmarks:
        right_shoulder = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER]
        right_elbow = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_ELBOW]
        right_wrist = pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_WRIST]

        # 어깨, 팔꿈치, 손목의 x 좌표가 거의 동일한지 확인
        right_alignment = (abs(right_shoulder.x - right_elbow.x) < alignment_threshold) and \
                          (abs(right_elbow.x - right_wrist.x) < alignment_threshold)

        return right_alignment
    return False

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
            
            # 오른쪽 팔이 어깨와 동일 선상에 있는지 확인
            if is_right_arm_aligned(pose_results.pose_landmarks, image.shape):
                cv2.putText(image, "Down", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 3, cv2.LINE_AA)
                # 서버로 값 전송 (1)
                response = requests.post(server_url, json={"value": 0})
                print("서버 응답:", response.json())
            else:
                cv2.putText(image, "Up", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 0, 255), 3, cv2.LINE_AA)
                # 서버로 값 전송 (0)
                response = requests.post(server_url, json={"value": 1})
                print("서버 응답:", response.json())

        # 이미지 좌우 반전 (글씨 포함)
        image = cv2.flip(image, 1)

        # 결과 화면에 출력
        cv2.imshow("frame", image)
        if cv2.waitKey(1) == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()
