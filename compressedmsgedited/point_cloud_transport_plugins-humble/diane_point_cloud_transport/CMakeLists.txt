cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 14)

project(diane_point_cloud_transport)

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)
find_package(point_cloud_interfaces REQUIRED)
find_package(point_cloud_transport REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcpputils REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
set(Torch_DIR "/usr/local/lib/python3.8/dist-packages/torch/share/cmake/Torch")
find_package(Torch REQUIRED)
find_package(OpenMP REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set(dependencies  
  pluginlib::pluginlib
  ${point_cloud_interfaces_TARGETS}
  point_cloud_transport::point_cloud_transport
  rclcpp::rclcpp
  rcpputils::rcpputils
  ${sensor_msgs_TARGETS}
  ${std_msgs_TARGETS}
)

include_directories(include)

add_library(${PROJECT_NAME}
  SHARED
  src/diane_encoder.cpp
  src/diane_publisher.cpp
  src/diane_subscriber.cpp
  src/manifest.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${dependencies}
  ${TORCH_LIBRARIES}
  OpenMP::OpenMP_CXX
)

set_property(TARGET diane_point_cloud_transport PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PRIVATE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
  # ${Torch_DIR}
)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

pluginlib_export_plugin_description_file(point_cloud_transport diane_plugins.xml)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(${dependencies})
ament_package()
