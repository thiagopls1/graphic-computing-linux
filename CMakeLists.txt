cmake_minimum_required(VERSION 3.14)  # FetchContent requer pelo menos 3.11, mas 3.14 é recomendado
project(GraphicComputing LANGUAGES CXX)

# Address Sanitizer
if (UNIX)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Inclui o módulo FetchContent
include(FetchContent)

# Declara as dependências e suas origens
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

# FetchContent_Declare(
#     glew
#     GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git  # Versão com CMake
#     GIT_TAG        origin/master
# )

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)

# Torna as dependências disponíveis
FetchContent_MakeAvailable(glfw glm)

# Set `source` variable to all .h and .cpp files in `src`
file(GLOB_RECURSE source "src/*.h" "src/*.cpp")

# Cria o executável com todos os arquivos
add_executable(GraphicComputing ${source})

# Linka com as bibliotecas
target_link_libraries(GraphicComputing PRIVATE
  glfw
	GLEW::GLEW
	OpenGL::GL
  glm::glm
)

# Configura includes
target_include_directories(GraphicComputing PRIVATE
  ${glew_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
	${glfw_SOURCE_DIR}
)
