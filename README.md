# bt_sensor_manager
![image](https://github.com/user-attachments/assets/fcf8780e-3443-4264-922f-11ec56a6c23d)

![image](https://github.com/user-attachments/assets/c35fecc2-bace-4ed7-ab04-ead4af5ecb7f)


### 개요
본 프로젝트는 NUC, Jetson, 그리고 호스트 PC(한성 노트북) 세 대의 컴퓨터를 이용하여 다양한 센서를 ROS2 환경에서 운영하고, Behavior Tree를 통해 센서 이상 발생 시 자동으로 재시작하는 기능을 구현합니다.
---
### 시스템 구성
NUC

Hesai LiDAR (PointCloud2 메시지)

OAK-D 카메라 2대 (RGB 이미지 메시지)

NUC에서 센서 드라이버 노드를 실행하고, ROS2 토픽을 퍼블리시합니다.

Jetson

ZED 카메라 (USB 타입) 1대

ZED X 카메라 (GMSL 타입) 2대

Jetson에서 센서 드라이버 노드를 실행하고, ROS2 토픽을 퍼블리시합니다.

호스트 PC (한성 노트북)

RViz2를 통해 위 두 시스템(NUC, Jetson)에서 퍼블리시하는 토픽을 구독(subscribe)하고 시각화합니다.

VSC(Visual Studio Code)의 SSH 기능을 이용해 NUC와 Jetson에 원격 접속하여 센서 런치 파일을 실행/관리합니다.

### 목표
Behavior Tree를 사용하여 센서 상태를 모니터링하고,

센서가 중단되거나 일정 시간 동안 메시지를 퍼블리시하지 않으면,

자동으로 센서를 재시작하는 로직을 구현합니다.

### 주요 기능
센서 상태 모니터링

토픽 기반 Condition 노드(ImageAliveCondition, TopicAliveCondition)를 통해 이미지/포인트 클라우드 메시지가 일정 시간 동안 수신되지 않으면 FAILURE 상태를 반환합니다.

센서 재시작 액션

RestartSensorAction 노드가 실행되면, SSH 명령어를 통해 해당 센서를 구동하는 런치 파일을 원격으로 재시작합니다.

성공적으로 토픽이 다시 발행되면 Behavior Tree에서 SUCCESS를 반환하고, 센서가 정상 동작 중임을 확인합니다.

### Behavior Tree 구조

<Fallback> 노드로 구성하여, Condition이 FAILURE를 반환할 경우에만 Action(재시작)을 호출하도록 합니다.

Condition이 SUCCESS 상태면, Action을 호출하지 않고 넘어갑니다.

### 프로젝트 구조
├── bt_sensor_manager
│   ├── include/bt_sensor_manager/
│   │   ├── image_alive_condition.hpp
│   │   ├── topic_alive_condition.hpp
│   │   └── restart_sensor_action.hpp
│   ├── src/
│   │   ├── image_alive_condition.cpp
│   │   ├── topic_alive_condition.cpp
│   │   └── restart_sensor_action.cpp
│   ├── behavior_trees/
│   │   └── monitor_tree.xml
│   └── CMakeLists.txt
└── ...

image_alive_condition.cpp / topic_alive_condition.cpp
: 토픽 메시지(이미지/포인트 클라우드)의 수신 여부를 일정 시간 간격으로 체크

restart_sensor_action.cpp
: SSH를 이용해 원격 PC에서 센서를 재시작하는 명령어를 실행

monitor_tree.xml
: Behavior Tree XML 정의

