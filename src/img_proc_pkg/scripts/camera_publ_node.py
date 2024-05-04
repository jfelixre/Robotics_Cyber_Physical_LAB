#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from sensor_msgs.msg import Image

import cv2 as cv


class CameraPublNode(Node):

    def __init__(self):
        super().__init__('camera_publ_node')
        self.publisher_ = self.create_publisher(Image, 'cameras/camera1/image_raw', 10)
        
        cap = cv.VideoCapture(2)
        if not cap.isOpened():
            print("Cannot open camera")
            rclpy.shutdown()


        while True:
        # Capture frame-by-frame
            ret, frame = cap.read()
            cv.imshow('frame',frame)
            if cv.waitKey(1) == ord('q'):
                break

        cap.release()
        cv.destroyAllWindows()
        rclpy.shutdown()



def main(args=None):
    rclpy.init(args=args)

    camera_publ_node = CameraPublNode()

    rclpy.spin(camera_publ_node)

    camera_publ_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()