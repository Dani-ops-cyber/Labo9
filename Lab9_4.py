import cv2
import numpy as np

# Cargar la imagen de monedas
image_path = "/home/mario/Documentos/Embebidos/monedas_2.jpg"
image = cv2.imread(image_path)

# Verificar que la imagen se haya cargado correctamente
if image is None:
    print("Error al cargar la imagen.")
    exit()

# Paso 1: Convertir la imagen a escala de grises
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# Paso 2: Aplicar un desenfoque Gaussiano para reducir el ruido
blurred = cv2.GaussianBlur(gray, (15, 15), 0)

# Paso 3: Detectar bordes usando el detector Canny
edges = cv2.Canny(blurred, 50, 150)

# Paso 4: Encontrar los contornos en la imagen
contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# Filtrar los contornos que son demasiado pequeños para ser monedas
min_contour_area = 100  # Ajustar este valor según sea necesario
filtered_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > min_contour_area]

# Dibujar los contornos filtrados sobre la imagen original
output_image = image.copy()
cv2.drawContours(output_image, filtered_contours, -1, (0, 255, 0), 3)

# Mostrar la cantidad de monedas detectadas
print(f"Se detectaron {len(filtered_contours)} monedas.")

# Mostrar la imagen con los contornos dibujados
cv2.imshow("Monedas detectadas", output_image)

# Mostrar la imagen en escala de grises, bordes Canny y contornos por separado
cv2.imshow("Escala de grises", gray)
cv2.imshow("Bordes Canny", edges)

# Esperar hasta que se presione una tecla para cerrar la ventana
cv2.waitKey(0)
cv2.destroyAllWindows()
