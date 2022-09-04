import numpy as np
import cv2
from calibration import *

#-------------------Detector Aruco -----------------

#Inicialización de los parametros
parametros = cv2.aruco.DetectorParameters_create()

#cargando el diccionario de nuestro aruco
diccionario = cv2.aruco.Dictionary_get(cv2.aruco.DICT_5X5_100)

#Lectura de camara
cap = cv2.VideoCapture(0)
cap.set(3,1280)
cap.set(4,720)
cont = 0

#calibracion
calibracion = calibration()
matrix, dist = calibracion.calibration_cam()
print("Matriz de la camara:", matrix)
print("Coeficiente de Distoriciones", dist)

while True:
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    esquinas, ids, candidatos_malos = cv2.aruco.detectMarkers(gray,diccionario, parameters=parametros) #cameraMatrix = matrix, distCoeff = dist)

    try:
        if np.all(ids != None):
            for i in range(0, len(ids)):
                rvec, tvec, markerPoints = cv2.aruco.estimatePoseSingleMarkers(esquinas[i], 0.05, matrix, dist)

                (rvec - tvec).any()

                cv2.aruco.drawDetectedMarkers(frame, esquinas)

                cv2.drawFrameAxes(frame, matrix, dist, rvec, tvec, 0.05)

                c_x = (esquinas[i][0][0][0]+esquinas[i][0][1][0]+esquinas[i][0][2][0]+esquinas[i][0][3][0])/4

                c_y = (esquinas[i][0][0][1]+esquinas[i][0][1][1]+esquinas[i][0][2][1]+esquinas[i][0][3][1])/4

                cv2.putText(frame,"id" + str(ids[i]),(int(c_x),int(c_y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (50,225,250),2)

    except:
        if ids is None or len(ids) == 0:
            print("**** Marker Detection Failed ****")

    cv2.imshow("Realidad virtual", frame)

    k = cv2.waitKey(1)


    if k == 97:
        print("Imagen Guardada")
        cv2.imwrite("cali{}.png".format(cont), frame)
        count = count + 1

    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()