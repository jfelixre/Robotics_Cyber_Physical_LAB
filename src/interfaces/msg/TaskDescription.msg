uint32 task_id
uint32 obj_id
uint32 obj_size     #only 1 and 2   (1 robot needed, 2 robots needed)
geometry_msgs/Point goal
uint32 leader_robot_id   #0 no need help,   0> id of leader robot
uint32 priority
uint32 state  #0 pending    1 in progress    2 finished
float64 idle_time_start
uint32 robot_id    #number of robot
uint32 in_progress_time_start
uint32 finish_time
uint32 idle_time
uint32 task_time
uint32 total_time