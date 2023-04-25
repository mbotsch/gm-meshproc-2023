file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.h)

add_executable(mesh-processing ${SOURCES} ${HEADERS})
target_link_libraries(mesh-processing pmp poisson)

if (EMSCRIPTEN)
    set_target_properties(mesh-processing PROPERTIES LINK_FLAGS "--shell-file ${PROJECT_SOURCE_DIR}/external/pmp/shell.html --preload-file ${PROJECT_SOURCE_DIR}/data@./data")
endif()
