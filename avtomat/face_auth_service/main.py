import math
import time

import cv2
import dlib
import numpy as np
import hashlib
import asyncio
import websockets

# Инициализация детектора лиц и предсказателя ключевых точек
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')


# Функция для получения ключевых точек лица
def get_face_landmarks(image, detector, predictor):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    faces = detector(gray)
    if len(faces) == 0:
        return None
    landmarks = predictor(gray, faces[0])
    points = [(landmarks.part(i).x, landmarks.part(i).y) for i in range(68)]

    return points

# TODO: 1
#  1) как получаю ключ мат часть
#  2) учитывается ли наклон лица и удаления лица от камеры
#  3) зачем используется веб сокет, почему нельзя другие протоколы использовать
#  4) Какие библиотеки использую для получения изобр. с камеры, для поиска биом. точки

# TODO: 17.0 30.0 18.0 17.0 (24.06)

# Функция для создания ключа из ключевых точек лица
def create_face_key(points):
    eyebrow_width = abs(points[21][0] - points[22][0]) / abs(points[0][0] - points[16][0])  # расстояние между бровями
    nose_bridge_y = abs(points[27][1] - points[30][1]) / abs(points[0][0] - points[16][0])  # высота носа
    left_eye_width = abs(points[36][0] - points[39][0]) / abs(points[0][0] - points[16][0])  # ширина левого глаза
    right_eye_width = abs(points[42][0] - points[45][0]) / abs(points[0][0] - points[16][0])  # ширина правого глаза
    print(round(eyebrow_width, 2) * 100, round(nose_bridge_y, 2) * 100, round(left_eye_width, 2) * 100,
          round(right_eye_width, 2) * 100)
    key = f"{round(eyebrow_width, 2) * 100}-{round(nose_bridge_y, 2) * 100}-{round(left_eye_width, 2) * 100}-{round(right_eye_width, 2) * 100}"
    face_key = hashlib.sha256(key.encode()).hexdigest()
    return face_key


# Переменные для хранения сумм координат точек и количества обновлений
sum_points = None
num_updates = 0


# Функция для усреднения координат точек
def smooth_points(points):
    global sum_points, num_updates
    if sum_points is None:
        sum_points = np.zeros((68, 2), dtype=np.float32)
    num_updates += 1
    for i, point in enumerate(points):
        sum_points[i][0] += point[0]
        sum_points[i][1] += point[1]
    # находим сред. арифм для каждой точки
    smoothed_points = [(int(sum_points[i][0] / num_updates), int(sum_points[i][1] / num_updates)) for i in range(68)]
    return smoothed_points


async def authByFace(websocket: websockets.WebSocketServerProtocol):
    global sum_points, num_updates

    # Захват видео с веб-камеры
    cap = cv2.VideoCapture(0)

    prev_time_mat = time.time()
    prev_time_key = time.time()

    oval_width = 300
    oval_height = 350
    oval_color = (0, 255, 0)

    start_time = time.time()
    duration = 65
    while (time.time() - start_time) < duration:
        ret, frame = cap.read()
        if not ret:
            break

        # Рисование овала
        frame_height, frame_width, _ = frame.shape
        oval_x = (frame_width) // 2
        oval_y = (frame_height) // 2
        cv2.ellipse(frame, (oval_x, oval_y), (oval_width // 2, oval_height // 2), 0, 0, 360, oval_color, 2)

        points = get_face_landmarks(frame, detector, predictor)
        # Сбрасываем мат ожид каждые 5 сек
        if (time.time() - prev_time_mat) > 5:
            sum_points = None
            num_updates = 0
            prev_time_mat = time.time()

        if points:
            smoothed_points = smooth_points(points)

            if (time.time() - prev_time_key) > 1:
                prev_time_key = time.time()
                face_key = create_face_key(smoothed_points)
                print(f"{face_key}")
                try:
                    await websocket.send(face_key)
                except websockets.exceptions.ConnectionClosedError:
                    print("Клиент отключился")
                    break

            for i, point in enumerate(smoothed_points):
                cv2.circle(frame, point, 1, (0, 255, 0), -1)
                cv2.putText(frame, str(i + 1), (point[0], point[1] - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (0, 255, 0),
                            1)

        # Отображение видеопотока
        cv2.imshow('Video', frame)
        cv2.waitKey(1)
    cap.release()
    cv2.destroyAllWindows()


# Создаем сервер
async def handler(websocket, path):
    await authByFace(websocket)


start_server = websockets.serve(handler, 'localhost', 8765)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
