file(GLOB SRCS ./*.cpp)
file(GLOB HDRS ./*.h)

add_library(poisson STATIC ${SRCS} ${HDRS})
