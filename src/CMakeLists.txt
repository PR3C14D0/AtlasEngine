# CMakeList.txt: proyecto de CMake para AtlasEngine, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Agregue un origen al ejecutable de este proyecto.
add_executable (AtlasEngine  "main.cpp" "public/Engine/Core.h" "private/Engine/Core.cpp" "public/Engine/Window.h" "private/Engine/Window.cpp" "public/Engine/Error.h" "private/Engine/Error.cpp" "public/Math/Vector3.h" "private/Math/Vector3.cpp" "private/Math/Transform.cpp" "private/Math/Transform.cpp" "public/Engine/GameObject/GameObject.h" "private/Engine/GameObject/GameObject.cpp" "private/Engine/GameObject/Component.cpp" "private/Engine/GameObject/Component.cpp" "public/Engine/GameObject/Components/Mesh.h" "public/Engine/Scene/Scene.h" "private/Engine/Scene/Scene.cpp" "public/Engine/Scene/SceneManager.h" "private/Engine/Scene/SceneManager.cpp" "private/Engine/GameObject/Components/Mesh.cpp" "public/Engine/Vertex.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AtlasEngine PROPERTY CXX_STANDARD 20)
  set_property(TARGET AtlasEngine PROPERTY WIN32_EXECUTABLE TRUE)
endif()

set(PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/public")
set(GLOBAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib")
set(D3D_LIB "${LIB}/DirectX/D3D11.lib;${LIB}/DirectX/D3DX11.lib;${LIB}/DirectX/DXGI.lib")

add_custom_command(TARGET AtlasEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Shader/shader.fx ${CMAKE_CURRENT_BINARY_DIR}/shader.fx)

target_link_libraries(AtlasEngine "${D3D_LIB}")

include_directories("${PRIVATE_INCLUDES}")
include_directories("${GLOBAL_INCLUDES}")