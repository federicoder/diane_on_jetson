# DIANE on JETSON

<table align="center">
  <tr>
    <td align="center">
      <img src="/connection.png" alt="Connessione" width="500"/>
    </td>
  </tr>
</table>  
**Table of Contents**

- [Project Structure and Description](#project-structure-and-description)
  - [DIANE_POINT_CLOUD_TRANSPORT](#DIANE_POINT_CLOUD_TRANSPORT)
  - [zed_pointcloud_publisher](#zed_pointcloud_publisher)
  - [dynamic_encoder_pkg](#dynamic_encoder_pkg)
- [Installation & Usage](#installation--usage)
    - [Docker Environment Setup](#1-docker-environment-setup)
    - [Building the Workspace](#2-building-the-workspace-inside-the-container)
    - [Launching the nodes](#3-launching-the-nodes)
        - [Launcher](#option-31--use-launcher-script-for-encoding-and-decoding)
        - [Run nodes separately](#option-32--run-encoding-and-decoding-nodes-separately)

### Project Structure and Description

The following repository, named **diane_on_jetson**, was created for the master’s thesis project with the purpose of implementing a lossy codec called **DIANE** on the ROS 2 Humble `point_cloud_transport` plugin.
This repository is part of a master's thesis project aimed at extending the [`point_cloud_transport`](https://github.com/ros-perception/point_cloud_transport_plugins/tree/humble) repository, a standard ROS 2 plugin-based system for publishing and subscribing to compressed `sensor_msgs/PointCloud2` messages.

The goal of this work is to create a pipeline used to perform lossy compression operationn for point clouds with ROS2 HUMBLE. To implement this pipeline, it is used a particoular image of **ROS2 HUMBLE** containing **ZED SDK**, and all requirements needed to use Torch (wheeled version) in a JETSON XAVIER NX.
The  new **lossy point cloud compression plugin** used in `point_cloud_transport` is called `DIANE` and it enables real-time performance using a custom algorithm based on **PyTorch** and its c++ wheeled version **Torch**. This plugin complements existing ones like `zlib`, `zstd`, and `draco`, and is designed to optimize the trade-off between compression ratio, processing time, with a strong focus on runtime efficiency for real-time robotics applications.

In the repository, the main modules are:
- Dockerfile.tegra: dockerfile in which there are all the necessary images and SDK to build a Ros2 Humble enviromnet (pointcloudtransport, ZED SDK, torch, etc..)
- my_build.sh: to build the above dockerfile.
- my_run.sh: to start a container with all the necessary configuration setup.
- src directory: the src directory copied in the container.

### DIANE_POINT_CLOUD_TRANSPORT
It is the main subdirectory of `point_cloud_transport` that implements in c++ language the codec `DIANE`. It can be found in `src/point_cloud_transport_plugins-humble`. It is possible to find the src directory in which there are all implemented codes and the include directory to see the the related headers files.

### zed_pointcloud_publisher
It is the ros2 package used to acquire from the `ZED-CAMERA` and `ZED-SDK` the point cloud. This package is built in python languange and it can be easly used to change all paramterers of the acquisition process.

### dynamic_encoder_pkg
It is the launchdr used to change the operation paramters (bandwidth and FPS) of the DIANE publisher in `point_cloud_transport`. It acquire all the bandwidth and FPS needed by using the commands:
```
ros2 topic bw
ros2 topic hz
```
pointing the Diane Subscriber in which is perfomred the acquisition and conversion of the compressed msgs.

----
## Installation & Usage

This section describes how to set up and run the `DIANE` for `point_cloud_transport` using Docker. The process is divided into three main phases: Docker environment setup, workspace build, and node launching.

### 1. Docker Environment Setup

First, build and run the Docker container that contains all necessary dependencies:

```bash
./my_builder.sh
./my_run.sh
```

### 2. Building the Workspace (inside the container)
Once inside the container (in `/ros2_ws`), build the workspace using:

```bash
colcon build --merge-install --event-handlers console_direct+
```
This command will compile the `point_cloud_transport` plugins and all required ROS 2 packages.

### 3. Launching the Nodes
After everithing is compiled, it is possible to start the zed_pointcloud_publisher and starting to acquire the pointloud giving in input the command:
```
ros2 run zed_pointcloud_publisher zed_pointcloud_publisher
```
Now that the /zed_pointcloud topic is started, it is possible to do two kind of operations to compress the point clouds with `DIANE` in `point_cloud_transport`:
- To start the `standalone` version
- To start the `launcher` version
----
##### Standalone version


##### Launcher version
