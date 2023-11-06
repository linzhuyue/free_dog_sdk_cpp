# Unitree Go 1 Free Dog SDK CPP
Welcome to the repository designed for general users without the need for "Krypton gold". 

This repository serves as an experimental platform and should be utilized primarily for educational purposes or non-commercial development. 

We extend our gratitude to Bin4ry for their invaluable contributions to the protocol analysis and the Python iteration.
# Note!!

1. Only test on go 1 air.
2. But it can be extended to other robots.
3. Thanks Bin4ry for the Python version.

## Current State
1. Testing with legged_control [Legged_controller(3Q qiayuan)](https://github.com/linzhuyue/legged_control/tree/master)
2. Testing wired,wifi high and low level control. especially torque tracking control.
3. Without any safety, you can clip with your controller with urdf limits.
4. I fix some bugs in python version, it can use for RL.

## How to use?
1. I make it as a ROS pkg, you can delete the ROS relative things, then you can use any where you wanna.
2. Clone into your ROS ws. And catkin build
3. If you used wired, you should set up your IP just like : 192.168.123.xxx
4. Then catkin build agian and ```source devel/setup.bash```
5. ```rosrun fdsc_utils fdsc_utils_pushup_node``` (This is most safe motion. If you use wireless with robot please modify connection settings as "LOW_WIFI_DEFAULTS")
## Examples:
1. Highlevel
   1. robot stand up and down
   2. rotation walk with trot
   3. force stand and walk
2. Lowlevel
   1. joint pd control
   2. joint velocity control
   3. joint tau control
   4. jont tau tracking control
   
## Before you run lowlevel control. You should use the RC and do following cmd sequence:
```
L2 + B
L1 + L2 + START
```

## The IP config and meanings in the file "unitreeConnectBoost.hpp":
```    const std::string localIpWifi = "192.168.12.222"; // Your PC wifi will give by the DHCP
    const std::string localIpEth = "192.168.123.12"; // Your PC
    const std::string addrWifi = "192.168.12.1"; // wifi pi
    const std::string addrLow = "192.168.123.10"; // Nano can't connect
    const std::string addrHigh = "192.168.123.161"; // Pi wired IP
    const std::string sim_addr = "127.0.0.1"; // Local
```
## Demos:
<p align = "center">
<img src="misc/demos.gif" width = "430" height = "260" border="5" />
</p>
   
## LICENSE
MIT LICENSE

## Referneces
1. [Bin4ry Python Version(Many Thks)](https://github.com/Bin4ry/free-dog-sdk/tree/main)
2. [legged_controller](https://github.com/qiayuanl/legged_control)