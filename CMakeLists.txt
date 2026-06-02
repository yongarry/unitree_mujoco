cmake_minimum_required(VERSION 3.16)
project(unitree_mujoco)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message("Debug mode")
  add_compile_options(-g -O0)
else()
  message("Release mode")
  add_compile_options(-O3 -DNDEBUG)
endif()

list(APPEND CMAKE_PREFIX_PATH "/opt/unitree_robotics/lib/cmake")
find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(unitree_sdk2 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

foreach(candidate
  "${CMAKE_CURRENT_SOURCE_DIR}/mujoco-3.8.0"
  "${CMAKE_CURRENT_SOURCE_DIR}/mujoco"
)
  if(EXISTS "${candidate}/include/mujoco/mujoco.h")
    set(MUJOCO_ROOT "${candidate}")
    break()
  endif()
endforeach()
if(NOT MUJOCO_ROOT)
  message(FATAL_ERROR "MuJoCo SDK not found. Put it in ${CMAKE_CURRENT_SOURCE_DIR}/mujoco-3.8.0 or ${CMAKE_CURRENT_SOURCE_DIR}/mujoco")
endif()
message(STATUS "Using MuJoCo SDK: ${MUJOCO_ROOT}")

set(UNITREE_SDK2_ROOT "/opt/unitree_robotics")

file(GLOB SIM_SRC
  src/joystick/joystick.cc

  # exclude ${MUJOCO_ROOT}/simulate/main.cc
  ${MUJOCO_ROOT}/simulate/glfw_*.cc
  ${MUJOCO_ROOT}/simulate/platform_*.cc
  ${MUJOCO_ROOT}/simulate/simulate.cc
  src/lodepng/lodepng.cpp
)

set(UNITREE_MUJOCO_LIBS
  pthread
  mujoco
  glfw
  unitree_sdk2
  boost_program_options
  fmt
)

add_executable(unitree_mujoco ${SIM_SRC} src/main.cc)
target_include_directories(unitree_mujoco PRIVATE
  /usr/local/include/iceoryx/v2.0.2
  ${MUJOCO_ROOT}/include
  ${MUJOCO_ROOT}/simulate
  src/lodepng
)
target_link_directories(unitree_mujoco PRIVATE
  ${MUJOCO_ROOT}/lib
  ${UNITREE_SDK2_ROOT}/lib
)
ament_target_dependencies(unitree_mujoco
  ament_index_cpp
)
target_link_libraries(unitree_mujoco
  ${UNITREE_MUJOCO_LIBS}
)
set_target_properties(unitree_mujoco PROPERTIES
  BUILD_RPATH "${MUJOCO_ROOT}/lib;${UNITREE_SDK2_ROOT}/lib"
  INSTALL_RPATH "${MUJOCO_ROOT}/lib;${UNITREE_SDK2_ROOT}/lib"
)

add_executable(jstest src/joystick/jstest.cc src/joystick/joystick.cc)
target_link_libraries(jstest pthread)

install(TARGETS
  unitree_mujoco
  jstest
  DESTINATION lib/${PROJECT_NAME}
)

foreach(resource_dir
  mujoco_plugin
  launch
)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${resource_dir}")
    install(DIRECTORY ${resource_dir}
      DESTINATION share/${PROJECT_NAME}
    )
  endif()
endforeach()

ament_package()