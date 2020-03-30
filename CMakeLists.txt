cmake_minimum_required(VERSION 3.5)

project(QtReversi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
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

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt5Multimedia)

set(TS_FILES
    QtReversi_en_CA.ts
    QtReversi_fr_CA.ts
    )

set(FILES
    main.cpp

    reversi.cpp reversi.h
    reversiview.cpp reversiview.h
    case.cpp case.h
    playertile.cpp playertile.h
    )

set(RESOURCES
    resources.qrc
    )

if(ANDROID)
    add_library(QtReversi SHARED
        ${RESOURCES}
        ${FILES}
        ${TS_FILES}
        )
else()
    add_executable(QtReversi
        ${RESOURCES}
        ${FILES}
        ${TS_FILES}
        )
endif()

target_link_libraries(QtReversi PRIVATE Qt5::Widgets Qt5::Multimedia)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
include(CPack)

#CMAKE_POLICY(SET CMP0020 NEW)
# remove console from release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET QtReversi PROPERTY WIN32_EXECUTABLE true)
endif()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
