#add_library(header_units)



add_library(foo)
# Add the module file to the library
target_sources(foo
  PUBLIC
    FILE_SET CXX_MODULES FILES
      stm32g474.cppm trace_board.cppm
)
target_include_directories(foo PRIVATE
    ../../motorlib/CMSIS/Include ../../motorlib/device/stm32g4/Include)
target_link_libraries(foo header_units)
# Create an executable
add_executable(main main.cpp)
# Link to the library foo
target_link_libraries(main foo)