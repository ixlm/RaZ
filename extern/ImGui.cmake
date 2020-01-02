#########
# ImGui #
#########

project(ImGui)
#added by xlm, add the imgui extentions
# set(LOG "log.txt")
# add_custom_command(OUTPUT ${LOG}
#     COMMAND copy ImGuizmo/*.h imgui/
#     COMMAND copy ImGuizmo/*.cpp imgui/
# )
# file(copy ImGuizmo/*.h imgui/)
# file(copy ImGuizmo/*.cpp imgui/)
# add_custom_target(copy_files ALL 
# COMMAND ${CMAKE_COMMAND} -E copy ImGuizmo/*.h imgui/
# COMMAND ${CMAKE_COMMAND} -E copy ImGuizmo/*.cpp imgui/
# )
# configure_file(ImGuizmo/*.h imgui/ COPYONLY)
# FILE(GLOB imgui_ext ImGuizmo/*.h ImGuizmo/*.cpp)

# aux_source_directory(ImGuizmo imgui_ext)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.h  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImGuizmo.h
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImGuizmo.cpp
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImSequencer.h  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImSequencer.h
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImSequencer.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImSequencer.cpp
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGradient.h  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImGradient.h
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGradient.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImGradient.cpp
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImCurveEdit.h  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImCurveEdit.h
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImCurveEdit.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/ImCurveEdit.cpp
    )
# message((STATUS ---------------${CMAKE_CURRENT_SOURCE_DIR}------ ))
# foreach(parttern_file ${imgui_ext} )
#    add_custom_command(OUTPUT ) 
# endforeach(parttern_file ${imgui_ext)

if (NOT RAZ_USE_GLEW)
    message(SEND_ERROR "Error: ImGui requires GLEW to be used.")
endif ()

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(imgui IMGUI_SRC)

if (NOT RAZ_USE_GLFW)
    list(
        REMOVE_ITEM
        IMGUI_SRC

        imgui/imgui_impl_glfw.h
        imgui/imgui_impl_glfw.cpp
     )
endif ()

# Building ImGui
add_library(ImGui OBJECT ${IMGUI_SRC})

target_include_directories(
    ImGui

    SYSTEM
    PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE

    glew/include
)

# Disabling all compilers warnings
if (MSVC)
    target_compile_options(ImGui PRIVATE /w)
else ()
    target_compile_options(ImGui PRIVATE -w)
endif ()

target_link_libraries(ImGui GLEW)

if (RAZ_USE_GLFW)
    target_link_libraries(ImGui GLFW)
endif ()
