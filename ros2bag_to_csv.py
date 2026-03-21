#!/usr/bin/env python3
"""
Convert a ROS 2 bag file to CSV format.
 
This script reads messages from a ROS 2 bag file and converts them to CSV format,
creating separate CSV files for each topic in the bag.
 
Usage:
  python3 ros2bag_to_csv.py <input_bag_path> <output_folder>
 
Subscription Topics:
    None (reads from bag file)
 
Publishing Topics:
    None (writes to CSV files)
 
:author: Addison Sears-Collins
:date: November 26, 2024
"""
 
import argparse
import csv
import os
 
from rclpy.serialization import deserialize_message
from rosidl_runtime_py.utilities import get_message
import rosbag2_py
from std_msgs.msg import String
 
 
def read_messages(input_bag: str):
    """
    Read messages from a ROS 2 bag file.
 
    :param input_bag: Path to the input bag file.
    :return: A generator yielding (topic, message, timestamp) tuples.
    """
    # Create a SequentialReader to read messages from the bag file
    reader = rosbag2_py.SequentialReader()
 
    # Open the bag file for reading
    reader.open(
        rosbag2_py.StorageOptions(uri=input_bag, storage_id="sqlite3"),
        rosbag2_py.ConverterOptions(
            input_serialization_format="cdr", output_serialization_format="cdr"
        ),
    )
 
    # Get all the topic types available in the bag file
    topic_types = reader.get_all_topics_and_types()
 
    def typename(topic_name):
        """
        Get the message type for a given topic.
 
        :param topic_name: The name of the topic.
        :return: The message type as a string.
        :raises ValueError: If the topic is not found in the bag.
        """
        # Iterate through the topic types to find the matching topic name
        for topic_type in topic_types:
            if topic_type.name == topic_name:
                return topic_type.type
        raise ValueError(f"topic {topic_name} not in bag")
 
    # Iterate through the messages in the bag file
    while reader.has_next():
        topic, data, timestamp = reader.read_next()
        msg_type = get_message(typename(topic))
        msg = deserialize_message(data, msg_type)
        yield topic, msg, timestamp
 
    # Clean up the reader
    del reader
 
 
def main():
    """
    Main function to parse arguments and convert ROS 2 bag to CSV.
    """
    # Create an argument parser
    parser = argparse.ArgumentParser(description="Convert ROS 2 bag to CSV")
    parser.add_argument("input", help="Input bag path (folder) to read from")
    parser.add_argument("output", help="Output folder to save CSV files")
 
    # Parse the command-line arguments
    args = parser.parse_args()
 
    # Create the output folder if it doesn't exist
    if not os.path.exists(args.output):
        os.makedirs(args.output)
 
    # Initialize dictionaries to store topic files and writers
    topic_files = {}
    topic_writers = {}
 
    # Iterate through the messages in the bag file
    for topic, msg, timestamp in read_messages(args.input):
        # If the topic hasn't been processed before, create a new CSV file and writer
        if topic not in topic_files:
            output_file = os.path.join(args.output, topic.replace('/', '_') + '.csv')
            topic_files[topic] = open(output_file, mode='w',
                                      newline='', encoding='utf-8')
            topic_writers[topic] = csv.writer(topic_files[topic])
            topic_writers[topic].writerow(['timestamp', 'data'])
 
        # Write the message data to the corresponding CSV file
        if isinstance(msg, String):
            topic_writers[topic].writerow([timestamp, msg.data])
        else:
            topic_writers[topic].writerow([timestamp, str(msg)])
 
    # Close all the topic files
    for file in topic_files.values():
        file.close()
 
 
if __name__ == "__main__":
    main()