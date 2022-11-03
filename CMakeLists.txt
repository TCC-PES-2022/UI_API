# CMakeList.txt: projeto de CMake para p2, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

project ("p2")
# Adicione a origem ao executável deste projeto.
add_executable (p2 "p2.cpp" "p2.h" "UI_API.cpp" "UI_API.h")
target_link_libraries(p2 pthread)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET p2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
