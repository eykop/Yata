cmake_minimum_required(VERSION 3.5)

project(Yata LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Core Quick Widgets Xml QuickControls2 REQUIRED)

if(ANDROID)
    add_library(Yata SHARED
            src/main.cpp
            src/qml.qrc
        )
else()
    add_executable(Yata
            src/main.cpp
            src/models/TasksListModel.h
            src/models/TasksListModel.cpp
            src/models/Task.h
            src/models/Task.cpp
            src/utils/FileReader.h
            src/utils/FileReader.cpp
            src/utils/XmlSection.h
            src/utils/XmlParser.h
            src/utils/XmlParser.cpp
            src/qml.qrc
        )
endif()


if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    # sudo dnf install gtkmm24 gtkmm24-devel
    pkg_check_modules(GTKMM gtkmm-2.4)
    # sudo dnf install libnotifymm libnotifymm-devel
    target_include_directories(Yata SYSTEM PRIVATE /usr/include/libnotifymm-1.0/)
    target_include_directories(Yata SYSTEM PRIVATE ${GTKMM_INCLUDE_DIRS})
    target_link_directories(Yata PRIVATE ${GTKMM_LIBRARY_DIRS})
    target_link_libraries(Yata PRIVATE  ${GTKMM_LIBRARIES} notifymm-1.0)
endif()

configure_file(data/data.xml ${CMAKE_CURRENT_BINARY_DIR}/data.xml COPYONLY)
configure_file(data/schema.xsd ${CMAKE_CURRENT_BINARY_DIR}/schema.xsd COPYONLY)


target_compile_definitions(Yata
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Yata
    PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets Qt5::QuickControls2 Qt5::Xml)
