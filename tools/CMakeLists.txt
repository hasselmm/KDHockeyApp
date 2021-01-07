cmake_minimum_required(VERSION 3.14)

if (NOT CMAKE_CROSSCOMPILING)
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
