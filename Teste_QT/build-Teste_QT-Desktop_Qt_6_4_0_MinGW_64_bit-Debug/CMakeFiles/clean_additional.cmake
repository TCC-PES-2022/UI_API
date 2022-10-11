# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Teste_UI_API_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Teste_UI_API_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\mylib_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mylib_autogen.dir\\ParseCache.txt"
  "Teste_UI_API_autogen"
  "mylib_autogen"
  )
endif()
