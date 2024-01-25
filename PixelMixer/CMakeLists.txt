cmake_minimum_required(VERSION 3.0)

if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    # If no deployment target has been set default to the minimum supported
    # OS version (this has to be set before the first project() call)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0 CACHE STRING "iOS Deployment Target")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10 CACHE STRING "macOS Deployment Target")
    endif()
endif()

project(PixelMixer)
include_directories(include)


find_package(wxWidgets)
include(${wxWidgets_USE_FILE})

set(CMAKE_PREFIX_PATH "c:/libs/glew-cmake-master")
set(CMAKE_LIBRARY_PATH "c:/libs/glew-cmake-master/lib/Debug/x64/")
find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIRS})

if(WIN32)
    list(APPEND SRC_FILES res/rc.rc)
elseif(APPLE)
    list(APPEND SRC_FILES res/wxmac.icns)
endif()

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${GLEW_LIBRARIES})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "wxmac.icns"
        MACOSX_BUNDLE_ICON_FILE wxmac.icns
        MACOSX_BUNDLE_COPYRIGHT "PixelMixer"
        MACOSX_BUNDLE_GUI_IDENTIFIER "PixelMixer"
        )
endif()
