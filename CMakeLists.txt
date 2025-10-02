cmake_minimum_required(VERSION 3.11) # FetchContent requer pelo menos 3.11, mas 3.14 é recomendado
project(GraphicComputing LANGUAGES CXX)

# Address Sanitizer
if (UNIX)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# DEPENDÊNCIAS

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)

FetchContent_MakeAvailable(glfw glm)

# Tentei usar com o FetchContent, mas não funcionou bem
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# LINKAGEM DAS LIBS & ARQUIVOS

file(GLOB_RECURSE source "src/*.h"  "src/*.hpp" "src/*.cpp")
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})

add_executable(GraphicComputing ${source})

# Linka com as bibliotecas
target_link_libraries(GraphicComputing PRIVATE
  glfw
	GLEW::GLEW
	OpenGL::GL
	glm::glm
)
