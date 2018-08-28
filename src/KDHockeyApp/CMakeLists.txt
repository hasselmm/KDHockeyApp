configure_file(KDHockeyAppConfig.h.cmake include/KDHockeyAppConfig.h)
configure_file(KDHockeyAppManager.h include/ COPYONLY)

add_library(KDHockeyApp STATIC)

target_compile_definitions(KDHockeyApp PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE)
target_compile_definitions(KDHockeyApp PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(KDHockeyApp PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(KDHockeyApp PUBLIC GoogleBreakpadClient Qt5::Network)

target_sources(KDHockeyApp PRIVATE KDHockeyAppLiterals.cpp KDHockeyAppLiterals_p.h)
target_sources(KDHockeyApp PRIVATE KDHockeyAppManager.cpp KDHockeyAppManager.h KDHockeyAppManager_p.h)
target_sources(KDHockeyApp PRIVATE KDHockeyAppSoftAssert.cpp KDHockeyAppSoftAssert_p.h)

if(KDHOCKEYAPP_QMLSUPPORT_ENABLED)
    target_link_libraries(KDHockeyApp PUBLIC Qt5::QmlPrivate)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android") # =============================================================================
    target_link_libraries(KDHockeyApp PRIVATE Qt5::AndroidExtras)
    target_sources(KDHockeyApp PRIVATE KDHockeyAppManager_android.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS") # =============================================================================
    target_sources(KDHockeyApp PRIVATE KDHockeyAppManager_ios.mm)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux") # ===========================================================================
    target_sources(KDHockeyApp PRIVATE KDHockeyAppManager_linux.cpp)
else() # ===============================================================================================================
    target_sources(KDHockeyApp PRIVATE KDHockeyAppManager_generic.cpp)
endif() # ==============================================================================================================

#[[
target.path = $$LIBDIR
INSTALLS += target

headers.path = $$INCLUDEDIR/KDHockeyApp
headers.files = $$INSTALLHEADERS
INSTALLS += headers
]]
