docker build \
  -f Dockerfile.tegra \
  -t ros2-humble-tegra_ubuntu22:latest \
  --build-arg ROS_DISTRO=humble \
  --build-arg ROS_ROOT=/opt/ros2_humble \
  --build-arg UBUNTU_RELEASE_YEAR=22 \
  --build-arg ZED_SDK_MAJOR=4 \
  --build-arg ZED_SDK_MINOR=1 \
  .

