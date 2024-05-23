cmake_minimum_required(VERSION 3.16)

project(UAVRoute VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


configure_file(${CMAKE_SOURCE_DIR}/resources/graph.json ${CMAKE_CURRENT_BINARY_DIR}/graph.json COPYONLY)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(nlohmann_json 3.2.0 REQUIRED)



qt_standard_project_setup()

qt_add_executable(appUAVRoute
    main.cpp
)

qt_add_qml_module(appUAVRoute
    URI UAVRoute
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES resources/graph.json
    SOURCES graph.h graph.cpp
    SOURCES
    SOURCES antcolonyoptimization.h antcolonyoptimization.cpp

)


target_link_libraries(appUAVRoute
    PRIVATE Qt6::Quick
    PUBLIC nlohmann_json::nlohmann_json
)

include(GNUInstallDirs)
install(TARGETS appUAVRoute
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
