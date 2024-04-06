cmake_minimum_required(VERSION 3.26.4)

project(bpmndoc)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)


# Add your source files
file(GLOB SOURCES 
  src/bpmndoc.cpp
  src/node.cpp
  src/mdgen.cpp
)

# Add executable
add_executable(bpmndoc ${SOURCES})

# Set the output directory for the executable to the project's main directory
set_target_properties(bpmndoc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Check dependencies
find_library(XERCES_LIB xerces-c)
if(NOT XERCES_LIB)
  message(FATAL_ERROR "xerces-c library not found.\nTo install run: sudo apt install libxerces-c-dev")
endif()

find_library(BPMNPP bpmn++)
if(NOT BPMNPP)
    message(FATAL_ERROR "bpmn++ not found")
endif()

# Link the application to required libraries
target_link_libraries(bpmndoc ${XERCES_LIB} ${BPMNPP})

target_compile_options(${CMAKE_PROJECT_NAME}
  PRIVATE
  -O3
#  -Werror
#  -pedantic-errors
  -Wpedantic
  -Wall
  -Wextra
  -Wconversion
  -Wsign-conversion
)

# Install the application
install(TARGETS bpmndoc DESTINATION bin)

