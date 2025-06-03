xhost +local:
docker run -it --rm --net=host --volume="/dev:/dev" \
	 -e DISPLAY=unix$DISPLAY \
	 -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
	 --privileged --runtime nvidia --gpus all \
	 -e NVIDIA_DRIVER_CAPABILITIES=all \
         --name zedros2 ros2-humble-tegra_ubuntu22 \
	/bin/bash
