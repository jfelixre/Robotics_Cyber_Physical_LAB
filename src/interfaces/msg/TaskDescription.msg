uint32 task_id
uint32 obj_id
uint32 obj_size     #only 1 and 2   (1 robot needed, 2 robots needed)
geometry_msgs/Point goal
uint32 leader_follower   #Set 1 to new tasks
uint32 priority
uint32 state  #0 pending    1 in progress    2 finished
uint32_t idle_time_start
uint32_t in_progress_time_start
uint32_t finished_time
uint32_t idle_time
uint32_t task_time
uint32_t total_time