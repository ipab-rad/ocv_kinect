cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(GAZEBO_INCLUDE_DIRS /usr/include/gazebo-4.1;/usr/include/sdformat-2.0)
set(GAZEBO_LIBRARY_DIRS /usr/lib/x86_64-linux-gnu/gazebo-4.1/plugins)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO REQUIRED gazebo)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(OGRE-Terrain OGRE-Terrain)
endif()
include_directories(${GAZEBO_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS} ${OGRE-Terrain_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS} ${OGRE_LIBRARY_DIRS})

add_library(cameracontrol SHARED cameracontrol.cpp)
target_link_libraries(cameracontrol ${GAZEBO_LIBRARIES} ${OGRE_LIBRARIES} ${Boost_LIBRARIES})