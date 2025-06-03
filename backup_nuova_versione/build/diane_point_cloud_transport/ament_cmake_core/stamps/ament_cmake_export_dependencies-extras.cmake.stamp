# generated from ament_cmake_export_dependencies/cmake/ament_cmake_export_dependencies-extras.cmake.in

set(_exported_dependencies "pluginlib::pluginlib;point_cloud_interfaces::point_cloud_interfaces__rosidl_generator_c;point_cloud_interfaces::point_cloud_interfaces__rosidl_typesupport_fastrtps_c;point_cloud_interfaces::point_cloud_interfaces__rosidl_generator_cpp;point_cloud_interfaces::point_cloud_interfaces__rosidl_typesupport_fastrtps_cpp;point_cloud_interfaces::point_cloud_interfaces__rosidl_typesupport_introspection_c;point_cloud_interfaces::point_cloud_interfaces__rosidl_typesupport_c;point_cloud_interfaces::point_cloud_interfaces__rosidl_typesupport_introspection_cpp;point_cloud_interfaces::point_cloud_interfaces__rosidl_typesupport_cpp;point_cloud_interfaces::point_cloud_interfaces__rosidl_generator_py;point_cloud_transport::point_cloud_transport;rclcpp::rclcpp;rcpputils::rcpputils;sensor_msgs::sensor_msgs__rosidl_generator_c;sensor_msgs::sensor_msgs__rosidl_typesupport_fastrtps_c;sensor_msgs::sensor_msgs__rosidl_generator_cpp;sensor_msgs::sensor_msgs__rosidl_typesupport_fastrtps_cpp;sensor_msgs::sensor_msgs__rosidl_typesupport_introspection_c;sensor_msgs::sensor_msgs__rosidl_typesupport_c;sensor_msgs::sensor_msgs__rosidl_typesupport_introspection_cpp;sensor_msgs::sensor_msgs__rosidl_typesupport_cpp;sensor_msgs::sensor_msgs__rosidl_generator_py;sensor_msgs::sensor_msgs_library;std_msgs::std_msgs__rosidl_generator_c;std_msgs::std_msgs__rosidl_typesupport_fastrtps_c;std_msgs::std_msgs__rosidl_generator_cpp;std_msgs::std_msgs__rosidl_typesupport_fastrtps_cpp;std_msgs::std_msgs__rosidl_typesupport_introspection_c;std_msgs::std_msgs__rosidl_typesupport_c;std_msgs::std_msgs__rosidl_typesupport_introspection_cpp;std_msgs::std_msgs__rosidl_typesupport_cpp;std_msgs::std_msgs__rosidl_generator_py")

find_package(ament_cmake_libraries QUIET REQUIRED)

# find_package() all dependencies
# and append their DEFINITIONS INCLUDE_DIRS, LIBRARIES, and LINK_FLAGS
# variables to diane_point_cloud_transport_DEFINITIONS, diane_point_cloud_transport_INCLUDE_DIRS,
# diane_point_cloud_transport_LIBRARIES, and diane_point_cloud_transport_LINK_FLAGS.
# Additionally collect the direct dependency names in
# diane_point_cloud_transport_DEPENDENCIES as well as the recursive dependency names
# in diane_point_cloud_transport_RECURSIVE_DEPENDENCIES.
if(NOT _exported_dependencies STREQUAL "")
  find_package(ament_cmake_core QUIET REQUIRED)
  set(diane_point_cloud_transport_DEPENDENCIES ${_exported_dependencies})
  set(diane_point_cloud_transport_RECURSIVE_DEPENDENCIES ${_exported_dependencies})
  set(_libraries)
  foreach(_dep ${_exported_dependencies})
    if(NOT ${_dep}_FOUND)
      find_package("${_dep}" QUIET REQUIRED)
    endif()
    # if a package provides modern CMake interface targets use them
    # exclusively assuming the classic CMake variables only exist for
    # backward compatibility
    set(use_modern_cmake FALSE)
    if(NOT "${${_dep}_TARGETS}" STREQUAL "")
      foreach(_target ${${_dep}_TARGETS})
        # only use actual targets
        # in case a package uses this variable for other content
        if(TARGET "${_target}")
          get_target_property(_include_dirs ${_target} INTERFACE_INCLUDE_DIRECTORIES)
          if(_include_dirs)
            list_append_unique(diane_point_cloud_transport_INCLUDE_DIRS "${_include_dirs}")
          endif()

          get_target_property(_imported_configurations ${_target} IMPORTED_CONFIGURATIONS)
          if(_imported_configurations)
            string(TOUPPER "${_imported_configurations}" _imported_configurations)
            if(DEBUG_CONFIGURATIONS)
              string(TOUPPER "${DEBUG_CONFIGURATIONS}" _debug_configurations_uppercase)
            else()
              set(_debug_configurations_uppercase "DEBUG")
            endif()
            foreach(_imported_config ${_imported_configurations})
              get_target_property(_imported_implib ${_target} IMPORTED_IMPLIB_${_imported_config})
              if(_imported_implib)
                set(_imported_implib_config "optimized")
                if(${_imported_config} IN_LIST _debug_configurations_uppercase)
                  set(_imported_implib_config "debug")
                endif()
                list(APPEND _libraries ${_imported_implib_config} ${_imported_implib})
              else()
                get_target_property(_imported_location ${_target} IMPORTED_LOCATION_${_imported_config})
                if(_imported_location)
                  list(APPEND _libraries "${_imported_location}")
                endif()
              endif()
            endforeach()
          endif()

          get_target_property(_link_libraries ${_target} INTERFACE_LINK_LIBRARIES)
          if(_link_libraries)
            list(APPEND _libraries "${_link_libraries}")
          endif()
          set(use_modern_cmake TRUE)
        endif()
      endforeach()
    endif()
    if(NOT use_modern_cmake)
      if(${_dep}_DEFINITIONS)
        list_append_unique(diane_point_cloud_transport_DEFINITIONS "${${_dep}_DEFINITIONS}")
      endif()
      if(${_dep}_INCLUDE_DIRS)
        list_append_unique(diane_point_cloud_transport_INCLUDE_DIRS "${${_dep}_INCLUDE_DIRS}")
      endif()
      if(${_dep}_LIBRARIES)
        list(APPEND _libraries "${${_dep}_LIBRARIES}")
      endif()
      if(${_dep}_LINK_FLAGS)
        list_append_unique(diane_point_cloud_transport_LINK_FLAGS "${${_dep}_LINK_FLAGS}")
      endif()
      if(${_dep}_RECURSIVE_DEPENDENCIES)
        list_append_unique(diane_point_cloud_transport_RECURSIVE_DEPENDENCIES "${${_dep}_RECURSIVE_DEPENDENCIES}")
      endif()
    endif()
    if(_libraries)
      ament_libraries_deduplicate(_libraries "${_libraries}")
      list(APPEND diane_point_cloud_transport_LIBRARIES "${_libraries}")
    endif()
  endforeach()
endif()
