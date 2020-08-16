set(PLUGIN "control-center")
project(${PLUGIN})

set(QT Core Gui Widgets DBus)
find_package(Qt5 REQUIRED ${QT})

set(SRCS
    plugin.h
    controlwidget.h
    controlwidget.cpp
    popupwindow.h
    popupwindow.cpp
    volumewidget.h
    volumewidget.cpp
)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN} SHARED ${SRCS})

target_link_libraries(${PLUGIN} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::DBus
)
target_include_directories(${PLUGIN} PUBLIC ../interfaces)

install(TARGETS ${PLUGIN} LIBRARY DESTINATION /usr/lib/panda-menubar/plugins)
