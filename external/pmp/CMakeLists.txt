file(GLOB_RECURSE SRCS ./*.cpp)
file(GLOB_RECURSE HDRS ./*.h)

if (EMSCRIPTEN)

    add_library(pmp STATIC ${SRCS} ${HDRS})
    target_link_libraries(pmp imgui stb_image rply)

else()

    find_package(OpenGL REQUIRED)

    if (OpenGL_FOUND)
        add_library(pmp STATIC ${SRCS} ${HDRS})
        target_link_libraries(pmp imgui stb_image glfw glew rply ${OPENGL_LIBRARIES})
    endif()

endif()
