cmake_minimum_required (VERSION 3.11)

SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

project(nerv_pathtracer)

add_executable(
	nerv_pathtracer
	main.cpp "src/config.h" "src/config.cpp" "src/window.h" "src/window.cpp" "src/shader.cpp" "src/shader.h" "src/commun.h" "src/commun.cpp" "src/data.h")

add_subdirectory(dependencies/openGL)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(dependencies/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

set(GLM_DIR dependencies/glm)
include_directories(${GLM_DIR} ..)

set(IMGUI_DIR dependencies/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

include_directories("dependencies/thirdParty" ..)

#target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})

add_custom_command(TARGET nerv_pathtracer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:nerv_pathtracer> ${CMAKE_CURRENT_SOURCE_DIR})