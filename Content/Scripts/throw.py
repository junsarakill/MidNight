# 양손 / 무릎 아래로 손을 내려서 주먹을 쥐었다 폈다 하면 버리는 모션

import cv2
import mediapipe as mp

# Mediapipe 솔루션 초기화
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

# 웹캠 설정
cap = cv2.VideoCapture(0)

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
        
        # 손이 무릎 아래에 있고, 쥐어졌는지 확인
        if is_hand_below_knees(pose_results.pose_landmarks, hands_results.multi_hand_landmarks, image.shape):
            if is_hand_closed(hands_results.multi_hand_landmarks):
                cv2.putText(image, "Clear", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 3, cv2.LINE_AA)

        # 이미지 좌우 반전 (글씨 포함)
        image = cv2.flip(image, 1)

        # 결과 화면에 출력
        cv2.imshow("frame", image)
        if cv2.waitKey(1) == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()