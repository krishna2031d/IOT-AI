Python Code

import serial
import cv2
object_labels = {
    "light": (180, 200, 150, 220, 100, 150), 
    "television": (100, 150, 50, 100, 150, 200)  
}


arduino = serial.Serial('COM3', 9600)  # Replace with your port and baud rate

while True:
  data = arduino.readline().decode().strip()

 
  if data == "Camera activated!":
    print("Camera activated by Arduino.")



      image_data = arduino.read(image_size)
      image_bytes = bytearray(image_data)

      image = cv2.imdecode(np.frombuffer(image_bytes, np.uint8), cv2.IMREAD_COLOR)

      average_color = cv2.mean(image)

      for object_name, color_range in object_labels.items():
        if (color_range[0] <= average_color[0] <= color_range[1] and
            color_range[2] <= average_color[1] <= color_range[3] and
            color_range[4] <= average_color[2] <= color_range[5]):
          control_action(object_name)

        
          arduino.write(object_name.encode())  

          break 

  else:
    pass

Python Code description

The Python code utilizes OpenCV for object detection. It establishes serial communication with the Arduino. Upon receiving a signal indicating camera activation, it retrieves image data from the Arduino. It converts the byte array to an OpenCV image and calculates the average color. By comparing the average color to predefined ranges for objects like "light" and "television," it attempts to identify the object in view. If a match is found, it triggers a message and sends the object name back to the Arduino.