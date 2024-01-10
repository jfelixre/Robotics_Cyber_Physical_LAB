uint32 task_id
uint32 obj_id
uint32 obj_size     #only 1 and 2   (1 robot needed, 2 robots needed)
geometry_msgs/Point goal
uint32 leader_follower   #Set 1 to new tasks
uint32 priority
uint32 state  #0 pending    1 in progress    2 finished