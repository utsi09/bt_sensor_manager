# bt_sensor_manager
![image](https://github.com/user-attachments/assets/fcf8780e-3443-4264-922f-11ec56a6c23d)

![image](https://github.com/user-attachments/assets/c35fecc2-bace-4ed7-ab04-ead4af5ecb7f)



## 개요

이 프로젝트는 **NUC**, **Jetson**, 그리고 **호스트 PC** 세 대의 시스템을 통해 여러 센서를 ROS2로 운용하고, **Behavior Tree**를 이용해 센서 이상 발생 시 자동으로 재시작하게끔 만드는 것임.

## 시스템 구성

1. **NUC**  
   - **Hesai LiDAR** (PointCloud2 형식 메시지)  
   - **OAK-D 카메라** 2대 (RGB 이미지 메시지)  
   - 센서 드라이버 노드를 구동하여 ROS2 토픽을 발행함.

2. **Jetson**  
   - **ZED 카메라 (USB 타입)** 1대  
   - **ZED X 카메라 (GMSL 타입)** 2대  
   - 센서 드라이버 노드를 구동하여 ROS2 토픽을 발행함.

3. **호스트 PC**  
   - RViz2를 사용해 위 두 시스템(NUC, Jetson)에서 발행되는 토픽을 구독(subscribe)하고 시각화함.  
   - Visual Studio Code의 SSH 기능을 통해 NUC와 Jetson에 원격 접속, 센서 런치 파일을 실행 및 관리함.

## 목표

- **Behavior Tree**를 통해 센서 상태를 모니터링함.  
- 센서가 중단되거나 일정 시간 동안 메시지가 없으면, 자동으로 센서를 재시작하도록 구성함.  
- 시스템 전반의 안정성을 높이는 것이 주된 목표임.

## 주요 기능

1. **센서 상태 모니터링**  
   - 토픽 기반 Condition 노드(`ImageAliveCondition`, `TopicAliveCondition`)를 사용해 이미지 및 포인트클라우드 메시지가 일정 시간 동안 들어오지 않으면 FAILURE 상태를 반환함.

2. **센서 재시작 액션**  
   - `RestartSensorAction` 노드가 실행되면, SSH 명령어를 통해 원격 PC의 센서 런치 파일을 재실행함.  
   - 토픽이 다시 발행되면 Behavior Tree에서 SUCCESS를 반환하고, 센서가 정상적으로 동작 중임을 확인함.

3. **Behavior Tree 구조**  
   - `<Fallback>` 노드 구조를 사용하여, Condition 노드가 FAILURE를 반환할 때만 재시작 액션을 호출함.  
   - 센서가 정상 동작 중이면 Condition이 SUCCESS를 반환해 재시작 액션은 호출되지 않음.
