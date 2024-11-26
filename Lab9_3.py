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

# Crear el sustractor de fondo MOG2 para detectar movimiento
backSub = cv2.createBackgroundSubtractorMOG2(history=500, varThreshold=50, detectShadows=True)

# Procesar el video cuadro a cuadro
while True:
    ret, frame = cap.read()
    if not ret:
        break  # Salir cuando el video termine

    # Redimensionar el cuadro para optimizar el procesamiento
    frame = cv2.resize(frame, (640, 480))

    # Aplicar el sustractor de fondo para obtener la máscara de movimiento
    fg_mask = backSub.apply(frame)

    # Filtrar las sombras del primer plano
    _, fg_mask = cv2.threshold(fg_mask, 240, 255, cv2.THRESH_BINARY)

    # Encontrar contornos en la máscara de movimiento
    contours, _ = cv2.findContours(fg_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Dibujar los contornos encontrados en el cuadro original
    cv2.drawContours(frame, contours, -1, (0, 255, 0), 2)

    # Mostrar el resultado con los contornos dibujados
    cv2.imshow("Contornos de objetos en movimiento", frame)

    # Salir al presionar la tecla 'q'
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

# Liberar el video y cerrar ventanas
cap.release()
cv2.destroyAllWindows()
