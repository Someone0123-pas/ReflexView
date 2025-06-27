qt_add_qml_module(reflexview
    URI mainwindow
    RESOURCE_PREFIX "/"
    SOURCES
        init.cpp
        debug.cpp
    QML_FILES
        init.qml
)