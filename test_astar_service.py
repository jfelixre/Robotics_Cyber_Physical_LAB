#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from interfaces.srv import PathFinding

class TestClient(Node):
    def __init__(self):
        super().__init__('test_client')
        self.cli = self.create_client(PathFinding, '/robot_01/path_finding_server')
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Service not available, waiting...')
        self.req = PathFinding.Request()

    def send_request(self):
        # Grid libre (7200 elementos, todos 1)
        self.req.grid = [1] * 7200
        self.req.src_x = 42
        self.req.src_y = 46
        self.req.dst_x = 60
        self.req.dst_y = 30
        
        self.get_logger().info(f'Sending request: ({self.req.src_x},{self.req.src_y}) -> ({self.req.dst_x},{self.req.dst_y})')
        self.future = self.cli.call_async(self.req)
        return self.future

def main(args=None):
    rclpy.init(args=args)
    client = TestClient()
    future = client.send_request()
    
    rclpy.spin_until_future_complete(client, future, timeout_sec=5.0)
    
    if future.done():
        try:
            response = future.result()
            client.get_logger().info(f'Response received:')
            client.get_logger().info(f'  Success: {response.success}')
            client.get_logger().info(f'  Path length: {len(response.path_x)}')
            if len(response.path_x) > 0:
                client.get_logger().info(f'  Start: ({response.path_x[0]},{response.path_y[0]})')
                client.get_logger().info(f'  End: ({response.path_x[-1]},{response.path_y[-1]})')
        except Exception as e:
            client.get_logger().error(f'Service call failed: {e}')
    else:
        client.get_logger().error('Service call timed out!')
    
    client.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
