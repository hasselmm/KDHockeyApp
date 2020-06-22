cmake_minimum_required(VERSION 3.14)

if (CMAKE_CROSSCOMPILING)
    message(STATUS "Building native toolchain in ${CMAKE_CURRENT_BINARY_DIR}/toolchain")

    if (NOT QT_ROOT_NATIVE)
        if (CMAKE_HOST_APPLE)
            set(QT_ROOT_NATIVE "$ENV{QT_ROOT_MACOS}"
                CACHE PATH "Path of the native Qt version to use")
        elseif (CMAKE_HOST_WIN32)
            set(QT_ROOT_NATIVE "$ENV{QT_ROOT_WINDOWS}"
                CACHE PATH "Path of the native Qt version to use")
        else()
            set(QT_ROOT_NATIVE "$ENV{QT_ROOT_LINUX}"
                CACHE PATH "Path of the native Qt version to use")
        endif()
    endif()

    add_custom_command(
        COMMENT Configuring native toolchain for KDHockeyApp
        OUTPUT toolchain/CMakeCache.txt
        DEPENDS CMakeLists.txt

        COMMAND ${CMAKE_COMMAND}
                -G${CMAKE_GENERATOR}
                -S${CMAKE_CURRENT_SOURCE_DIR}
                -B${CMAKE_CURRENT_BINARY_DIR}/toolchain
                -DCMAKE_PREFIX_PATH="${QT_ROOT_NATIVE}"
                -DBREAKPAD_TARGET_NAME="${CMAKE_SYSTEM_NAME}"
    )

    add_custom_target(
        KDHockeyAppToolchain ALL
        DEPENDS toolchain/CMakeCache.txt GoogleBreakpadToolchain
        COMMENT Building native toolchain
        COMMAND ${CMAKE_COMMAND} --build toolchain --target KDHockeyAppToolchain
    )
elseif (NOT IOS)
    if (NOT TARGET Qt5::Core)
        find_package(Qt5 COMPONENTS Gui REQUIRED)
    endif()

    add_executable(KDHockeyAppCollectSymbols)
    set_property(TARGET KDHockeyAppCollectSymbols PROPERTY OUTPUT_NAME collectsymbols)
    target_compile_features(KDHockeyAppCollectSymbols PUBLIC cxx_std_14)
    target_link_libraries(KDHockeyAppCollectSymbols PRIVATE Qt5::GuiPrivate)
    target_sources(KDHockeyAppCollectSymbols PRIVATE collectsymbols.cpp)

    add_custom_target(KDHockeyAppToolchain DEPENDS KDHockeyAppCollectSymbols)
endif()
