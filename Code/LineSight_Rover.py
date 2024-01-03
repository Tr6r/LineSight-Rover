import cv2
import numpy as np
import nanocamera as nano
import matplotlib.pyplot as plt
import smbus
import time
#_______________________I2C___________________________
device_address = 0x08
bus = smbus.SMBus(1)
#_______________________Camera________________________
cap = nano.Camera(flip= 0, width = 220,height = 140, fps = 15)
mask = np.zeros((140,220), dtype=np.uint8)
trai= [0,0]
phai= [0,0]
line1 = [0,0,0,0]
line2 = [0,0,0,0]
mid_x = 0
mid_y = 0
center_coordinates = (110,140)
axesLength = (140, 50)
angle = 0  
startAngle = 0
endAngle = 360
color = (255, 255, 255)
thickness = -1
mask = cv2.rectangle(mask , (0, 110), (220,140), color, thickness=-1)
#mask = cv2.ellipse(mask, center_coordinates, axesLength,angle, startAngle, endAngle, color, thickness)
while True:
        frame = cap.read()
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        lower = np.array([0,0,0],dtype=np.uint8)
        up = np.array([180,255,120],dtype=np.uint8)
        #lower = np.array([0, 100, 105], dtype=np.uint8)
        #up = np.array([5, 255, 255], dtype=np.uint8)
        img_InRange = cv2.inRange(hsv,lower,up)
        kernel = np.ones((5,5), np.uint8)
        #img_InRange= cv2.erode(img_InRange, kernel, iterations=9)
        #img_InRange= cv2.dilate(img_InRange, kernel, iterations=5)
        roi = cv2.bitwise_and(mask, img_InRange)
        """
        blurred = cv2.GaussianBlur(roi , (9, 9), 0)
        edges = cv2.Canny(blurred, 10, 50)
        thinned_edges = cv2.ximgproc.thinning(edges)
        lines = cv2.HoughLinesP(thinned_edges, 1, np.pi / 180, threshold=20, minLineLength=2, maxLineGap=2)
        if lines is not None :
            for line in lines:
                x1, y1, x2, y2 = line[0]
                if x1 <= 70 and x2 <= 50:
                    print("line trai")
                    line1[0], line1[1], line1[2],line1[3] = line[0]
                elif x1 >= 150 and x2 >= 150:
                    line2[0], line2[1], line2[2],line2[3]  = line[0]
                if line1[0] != 0 and line2[2] != 0 and line1[0] < 110 and line2[2] > 110:
                    mid_x = (line1[0] + line2[2]) // 2
                    mid_y = (line1[1] + line2[3]) // 2
                print(line1)
                print("_____________________")
                cv2.line(roi, (x1, y1), (x2, y2), (0, 255, 0), 2)
        if mid_x != 0 and mid_y != 0:
                cv2.circle(roi, (mid_x, mid_y), 5, (255, 0, 0), -1)
        cv2.circle(roi, (mid_x, mid_y), 5, (255, 0, 0), -1)
        """
        white_points_coords = np.column_stack(np.where(roi == 255))
        sorted_white_points = sorted(white_points_coords, key=lambda x: (x[0], x[1]))
        if len(sorted_white_points) >= 2:
            if sorted_white_points[0][1] < 70:
                print("trai")
                trai[1] = sorted_white_points[0][0]
                trai[0] = sorted_white_points[0][1]
                print(trai)
            if sorted_white_points[len(white_points_coords)-1][1] > 140 :#and white_points_coords[white_points_coords.shape[0]-1][0]>120:
                print("phai")
                phai[1] = sorted_white_points[len(white_points_coords)-1][0]
                phai[0] = sorted_white_points[len(white_points_coords)-1][1]
        if trai[0]<110 and phai[0]>110:
            offset = 110 - ((trai[0]+phai[0])/2-17)
            print(offset)
            message = str(offset)+"\0"
            for char in message:
                ascii_code = ord(char)
                bus.write_byte(device_address, ascii_code)
        cv2.circle(roi, (trai[0],trai[1]), 3, (255,255,0), -1)
        cv2.circle(roi, (phai[0],phai[1]), 3, (255,0,255), -1)
        cv2.line(roi , (phai[0],phai[1]), (trai[0],trai[1]), color, 2)
        cv2.circle(roi , (int((trai[0]+phai[0])/2-17),int((trai[1]+phai[1])/2)), 3, 255, -1)
        cv2.circle(roi, (110,130), 3, 255, -1)
        cv2.imshow('Frame', roi)
        if cv2.waitKey(1) == 27:
            break
cap.release()
del cap



