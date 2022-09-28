add_library(example)
add_library(UI_API)

target_sources(example
  PRIVATE
    example.cpp
  PUBLIC
    example.h
  )

target_sources(UI_API
  PRIVATE
    UI_API.cpp
  PUBLIC
    UI_API.h
  )

target_include_directories(example
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
  
target_include_directories(UI_API
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
  
  # we use this to get code coverage
# flags are only valid with the GNU compiler and on Linux
if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(example
    PUBLIC
      "--coverage"
    )
  target_link_options(example
    INTERFACE
      "--coverage"
    )
endif()


if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(UI_API
    PUBLIC
      "--coverage"
    )
  target_link_options(UI_API
    INTERFACE
      "--coverage"
    )
endif()