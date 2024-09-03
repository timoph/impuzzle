cmake_minimum_required(VERSION 3.16)
project(impuzzle2 VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Quick)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS QuickControls2)

qt_add_executable(impuzzle2 WIN32 MACOSX_BUNDLE
    defines.h
    engine.cpp engine.h
    imageimporter.cpp imageimporter.h
    main.cpp
    puzzleitem.cpp puzzleitem.h
)
target_include_directories(impuzzle2 PRIVATE
    .
)

target_compile_definitions(impuzzle2 PRIVATE
    APP_VERSION="2.0.0"
)

target_link_libraries(impuzzle2 PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
)


# Resources:
set(resources_resource_files
    "images/default.jpg"
)

qt_add_resources(impuzzle2 "resources"
    PREFIX
        "/"
    FILES
        ${resources_resource_files}
)
set(qml_resource_files
    "qml/FireWorks.qml"
    "qml/GameMenu.qml"
    "qml/GameOverBanner.qml"
    "qml/GameView.qml"
    "qml/ImagePicker.qml"
    "qml/LoadingScreen.qml"
    "qml/MenuButton.qml"
    "qml/SettingsCheckBox.qml"
    "qml/Style.qml"
    "qml/main.qml"
    "qml/qmldir"
)

qt_add_resources(impuzzle2 "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)
set(translantions_resource_files
    "languages/impuzzle2_en.qm"
    "languages/impuzzle2_fi.qm"
    "languages/impuzzle2_sv.qm"
)

qt_add_resources(impuzzle2 "translantions"
    PREFIX
        "/"
    FILES
        ${translantions_resource_files}
)

if((QT_VERSION_MAJOR EQUAL 6))
    target_link_libraries(impuzzle2 PRIVATE
        Qt::QuickControls2
    )
endif()

if(bigscreen)
    target_compile_definitions(impuzzle2 PRIVATE
        WINDOW_HEIGHT=800
        WINDOW_WIDTH=1280
    )
endif()

if(NOT bigscreen)
    target_compile_definitions(impuzzle2 PRIVATE
        WINDOW_HEIGHT=480
        WINDOW_WIDTH=800
    )
endif()

install(TARGETS impuzzle2
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
