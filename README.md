## Introduction

autodrive roboracer(f1tenth) 시뮬레이터에서 slam을 하기 위해 tf transform을 수행하는 패키지입니다.

</br></br>

## rf2o version

rf2o 패키지가 필요합니다.
https://github.com/MAPIRlab/rf2o_laser_odometry

</br>

```bash
cd src && git clone https://github.com/wndudwkd003/tf_transform_f1tenth.git
```

</br>

```bash
cd .. && colcon build && source install/setup.bash
```

</br>

```bash
# 각각 다른 터미널에서 실행해야합니다.

ros2 launch autodrive_roboracer bringup_graphics.launch.py

ros2 launch rf2o_laser_odometry rf2o_laser_odometry.launch.py

ros2 run tf_transform_f1tenth tf_publish_for_rf2o_node
```

</br>

![](https://github.com/wndudwkd003/tf_transform_f1tenth/blob/ros2_humble/docs/frames.png)
