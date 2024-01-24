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

find_package(wxWidgets)
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

find_package(GLEW)
if(GLEW_FOUND)
    include(${GLEW_INCLUDE_DIRS})
endif()

include_directories(include)

set(SRC_FILES
    src/App.cpp
    src/Button.cpp
    src/ConfigPanel.cpp
    src/ConfigTabs.cpp
    src/FrameBuffer.cpp
    src/HeaderPanel.cpp
    src/IndexBuffer.cpp
    src/InputPanel.cpp
    src/MainFrame.cpp
    src/OutputPanel.cpp
    src/Renderer.cpp
    src/SettingsPanel.cpp
    src/StatsPanel.cpp
    src/TextSlider.cpp
    src/Texture.cpp
    src/VertexArray.cpp
    src/VertexBuffer.cpp
    src/ViewportPanel.cpp
    )

if(WIN32)
    list(APPEND SRC_FILES res/rc.rc)
elseif(APPLE)
    list(APPEND SRC_FILES res/wxmac.icns)
endif()

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "wxmac.icns"
        MACOSX_BUNDLE_ICON_FILE wxmac.icns
        MACOSX_BUNDLE_COPYRIGHT "PixelMixer"
        MACOSX_BUNDLE_GUI_IDENTIFIER "PixelMixer"
        )
endif()
