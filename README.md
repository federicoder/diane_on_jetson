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
  - [C3lab_test_pub](#c3lab_test_pub)
  - [Diane_control](#diane_control)
  - [Point_cloud_transport_plugins](#point_cloud_transport_plugins)
    - [Diane_point_cloud_transport](#diane_point_cloud_transport)
    - [C3lab_point_cloud_transport](#c3lab_point_cloud_transport)
- [Installation & Usage](#installation--usage)
    - [Docker Environment Setup](#1-docker-environment-setup)
    - [Building the Workspace](#2-building-the-workspace-inside-the-container)
    - [Launching the nodes](#3-launching-the-nodes)
        - [Launcher](#option-31--use-launcher-script-for-encoding-and-decoding)
        - [Run nodes separately](#option-32--run-encoding-and-decoding-nodes-separately)
- [Features](#features)


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
It is the main subdirectory of `point_cloud_transport` that implements in c++ language the codec `DIANE`. It is possible to find the src directory in which there are all implemented codes and the include directory to see the the related headers files.

----
