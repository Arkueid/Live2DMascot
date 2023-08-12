# Install script for directory: D:/Live2D/CubismSdkForNative-4-r.5.1/Framework/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Demo")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Effect/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Id/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Math/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Model/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Motion/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Physics/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Rendering/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Type/cmake_install.cmake")
  include("D:/Live2D/CubismSdkForNative-4-r.5.1/Samples/OpenGL/Demo/proj.win.cmake/build/proj_msvc2022_x86_mt/Framework/src/Utils/cmake_install.cmake")

endif()

