import cv2
import numpy as np
import os

# Ruta al video en la carpeta Descargas
video_path = os.path.expanduser("/home/mario/Documentos/Embebidos/bouncing.mp4.mp4")

# Cargar el video
cap = cv2.VideoCapture(video_path)

# Verificar que el video se cargó correctamente
if not cap.isOpened():
    print("Error al abrir el video.")
    exit()

# Crear el sustractor de fondo MOG2, que suele ser más adecuado para detección de movimiento persistente
backSub = cv2.createBackgroundSubtractorMOG2(history=500, varThreshold=50, detectShadows=True)

# Procesar el video cuadro a cuadro
while True:
    ret, frame = cap.read()
    if not ret:
        break  # Salir cuando el video termine

    # Redimensionar el cuadro para optimizar el procesamiento
    frame = cv2.resize(frame, (640, 480))

    # Aplicar el sustractor de fondo al cuadro actual para obtener la máscara
    fg_mask = backSub.apply(frame)

    # Filtrar las sombras del primer plano
    _, fg_mask = cv2.threshold(fg_mask, 240, 255, cv2.THRESH_BINARY)

    # Usar la máscara para obtener solo los objetos en movimiento en color
    fg_frame = cv2.bitwise_and(frame, frame, mask=fg_mask)

    # Crear un fondo blanco
    white_background = np.full_like(frame, 255)

    # Crear el fondo enmascarado donde se conserva solo el fondo blanco
    inverse_mask = cv2.bitwise_not(fg_mask)
    bg_with_white = cv2.bitwise_and(white_background, white_background, mask=inverse_mask)

    # Combinar el fondo blanco y los objetos en color
    final_frame = cv2.add(bg_with_white, fg_frame)

    # Mostrar el resultado
    cv2.imshow("Video con fondo blanco y objetos en movimiento persistentes", final_frame)

    # Salir al presionar la tecla 'q'
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

# Liberar el video y cerrar ventanas
cap.release()
cv2.destroyAllWindows()
