# 양손 / 손이 쥐어져있는 상태로 어깨위에 올라가고 입을 벌리면 먹는거

import cv2
import mediapipe as mp

# 얼굴 메시 객체와 그리기 객체 초기화
mp_face_mesh = mp.solutions.face_mesh
mp_drawing = mp.solutions.drawing_utils
drawing_spec = mp_drawing.DrawingSpec(thickness=1, circle_radius=1)

# 입술의 랜드마크 인덱스를 정의
UPPER_LIP_INDICES = [61, 185, 40, 39, 37, 0, 267, 269, 270, 409, 415]
LOWER_LIP_INDICES = [146, 91, 181, 84, 17, 314, 405, 321, 375, 291, 308]

# 웹캠 설정
cap = cv2.VideoCapture(0)

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

with mp_face_mesh.FaceMesh(
    max_num_faces=1,
    refine_landmarks=True,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as face_mesh:

    while cap.isOpened():
        res, image = cap.read()
        if not res:
            print('웹캠에서 이미지를 가져오는 것을 실패')
            break
        
        # BGR 이미지를 RGB로 변환
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # 속도 개선을 위한 이미지 쓰기 불가능
        image.flags.writeable = False

        # 얼굴감지
        results = face_mesh.process(image)

        # 이미지 다시 쓰기로 지정
        image.flags.writeable = True

        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        if results.multi_face_landmarks:
            for face_landmarks in results.multi_face_landmarks:

                # 윗입술 그리기
                for idx in UPPER_LIP_INDICES:
                    point = face_landmarks.landmark[idx]
                    x = int(point.x * image.shape[1])
                    y = int(point.y * image.shape[0])
                    cv2.circle(image, (x, y), 1, (0, 255, 0), -1)
                
                # 아랫입술 그리기
                for idx in LOWER_LIP_INDICES:
                    point = face_landmarks.landmark[idx]
                    x = int(point.x * image.shape[1])
                    y = int(point.y * image.shape[0])
                    cv2.circle(image, (x, y), 1, (0, 0, 255), -1)
                
                # 입이 열려 있는지 확인
                if is_mouth_open(face_landmarks.landmark, image.shape):
                    cv2.putText(image, "eating", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
        
        image = cv2.flip(image, 1)
        cv2.imshow("frame", image)
        if cv2.waitKey(1) == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()
