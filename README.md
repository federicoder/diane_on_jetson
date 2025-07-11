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

In the repository, the main modules are:
- Dockerfile.tegra: dockerfile in which there are all the necessary images and SDK to build a Ros2 Humble enviromnet (pointcloudtransport, ZED SDK, torch, etc..)
- my_build.sh: to build the above dockerfile.
- my_run.sh: to start a container with all the necessary configuration setup.
- src directory: the src directory copied in the container.
