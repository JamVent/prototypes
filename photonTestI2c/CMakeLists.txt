#Generated by VisualGDB project wizard.
#Note: VisualGDB will automatically update this file when you add new sources to the project.

cmake_minimum_required(VERSION 2.7)
project(photonTestI2c)
set(LIBRARIES_FROM_REFERENCES "")

file(GLOB photonTestI2c_h . *.h)

add_executable(photonTestI2c photonTestI2c.cpp ${photonTestI2c_h})
target_link_libraries(photonTestI2c pigpio "${LIBRARIES_FROM_REFERENCES}")
