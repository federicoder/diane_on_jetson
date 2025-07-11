# DIANE on JETSON

<table align="center">
  <tr>
    <td align="center">
      <img src="/connection.png" alt="Connessione" width="500"/>
    </td>
  </tr>
</table>


The following repository, named **diane_on_jetson**, was created for the master’s thesis project with the purpose of implementing a lossy codec called **DIANE** on the ROS 2 Humble `point_cloud_transport` plugin.

In the repository, the main modules are:
- Dockerfile.tegra: dockerfile in which there are all the necessary images and SDK to build a Ros2 Humble enviromnet (pointcloudtransport, ZED SDK, torch, etc..)
- my_build.sh: to build the above dockerfile.
- my_run.sh: to start a container with all the necessary configuration setup.
- src directory: the src directory copied in the container.
  
