file(GLOB STB_SOURCE *.cpp)
file(GLOB STB_HEADER *.h)

add_library(stb_image STATIC ${STB_SOURCE} ${STB_HEADER})
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(stb_image::stb_image ALIAS stb_image)
