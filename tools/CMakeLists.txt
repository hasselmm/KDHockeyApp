cmake_minimum_required(VERSION 3.14)

if (CMAKE_CROSSCOMPILING)
    message(STATUS "Building native toolchain in ${CMAKE_CURRENT_BINARY_DIR}/toolchain")

    get_filename_component(QT_ROOT_COMMON "${CMAKE_PREFIX_PATH}" DIRECTORY)

    if (APPLE)
        set(QT_ROOT_NATIVE "${QT_ROOT_COMMON}/clang_64")
    else()
        set(QT_ROOT_NATIVE "${QT_ROOT_COMMON}/mingw53_32")
    endif()

    add_custom_target(
        KDHockeyAppToolchain ALL
        DEPENDS GoogleBreakpadToolchain

        COMMAND ${CMAKE_COMMAND}
                -G${CMAKE_GENERATOR}
                -S${CMAKE_CURRENT_SOURCE_DIR}
                -B${CMAKE_CURRENT_BINARY_DIR}/toolchain
                -DCMAKE_PREFIX_PATH="${QT_ROOT_NATIVE}"
                -DBREAKPAD_TARGET_NAME="${CMAKE_SYSTEM_NAME}"

        COMMAND ${CMAKE_COMMAND}
                --build toolchain --target KDHockeyAppToolchain)
elseif (NOT IOS)
    if (NOT TARGET Qt5::Core)
        find_package(Qt5 COMPONENTS Gui REQUIRED)
    endif()

    add_executable(KDHockeyAppCollectSymbols EXCLUDE_FROM_ALL)
    set_property(TARGET KDHockeyAppCollectSymbols PROPERTY OUTPUT_NAME collectsymbols)
    target_compile_features(KDHockeyAppCollectSymbols PUBLIC cxx_std_14)
    target_link_libraries(KDHockeyAppCollectSymbols PRIVATE Qt5::GuiPrivate)
    target_sources(KDHockeyAppCollectSymbols PRIVATE collectsymbols.cpp)

    add_custom_target(KDHockeyAppToolchain DEPENDS KDHockeyAppCollectSymbols)
endif()
