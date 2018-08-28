add_library(GoogleBreakpadCommon STATIC) # ===================================================================================

target_compile_definitions(GoogleBreakpadCommon PRIVATE -D__STDC_LIMIT_MACROS)
target_include_directories(GoogleBreakpadCommon PUBLIC src/src)

target_sources(GoogleBreakpadCommon PRIVATE src/src/common/convert_UTF.c)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/md5.cc)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/string_conversion.cc)

if(CMAKE_SYSTEM_NAME MATCHES "Android|Linux")
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/elfutils.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/file_id.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/guid_creator.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/linux_libc_support.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/memory_mapped_file.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/safe_readlink.cc)
endif()

add_library(GoogleBreakpadClient STATIC) # ===================================================================================

target_compile_definitions(GoogleBreakpadClient PRIVATE -D__STDC_LIMIT_MACROS)
target_link_libraries(GoogleBreakpadClient PUBLIC GoogleBreakpadCommon)

if(CMAKE_SYSTEM_NAME MATCHES "Android|Linux")
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/elfutils.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/file_id.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/guid_creator.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/linux_libc_support.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/memory_mapped_file.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/safe_readlink.cc)

    target_sources(GoogleBreakpadClient PRIVATE src/src/client/minidump_file_writer.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/crash_generation/crash_generation_client.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/dump_writer_common/thread_info.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/dump_writer_common/ucontext_reader.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/handler/exception_handler.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/handler/minidump_descriptor.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/log/log.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/microdump_writer/microdump_writer.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/linux_dumper.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/linux_ptrace_dumper.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/minidump_writer.cc)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_include_directories(GoogleBreakpadCommon PRIVATE src/src/common/android/include)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/android/breakpad_getcontext.S)
    target_link_libraries(GoogleBreakpadCommon PUBLIC -llog)
endif()

#[[
OTHER_FILES += \
    breakpad.pri \
    breakpad-config.pri

linux {
    QMAKE_CXXFLAGS_WARN_ON += \
        -Wno-missing-field-initializers \
        -Wno-unused-parameters \
        -Wno-tautological-compare \
        -Wno-tautological-constant-out-of-range-compare \
        -Wno-unused-value
} else: ios: CONFIG(device, device|simulator) {
    SOURCES += \
        src/src/client/ios/exception_handler_no_mach.cc \
        src/src/client/mac/handler/breakpad_nlist_64.cc \
        src/src/client/mac/handler/dynamic_images.cc \
        src/src/client/mac/handler/minidump_generator.cc \
        src/src/common/mac/file_id.cc \
        src/src/common/mac/macho_id.cc \
        src/src/common/mac/macho_utilities.cc \
        src/src/common/mac/macho_walker.cc \
        src/src/common/mac/string_utilities.cc
} else {
    warning("Skipping breakpad, which is not supported for this platform")
    SOURCES = breakpad_dummy.cpp # building a dummy to make XCode happy
}

# Custom targets to build breakpad's dump_syms tool ------------------------------------------------

equals(QMAKE_HOST.os, Linux) {
    dumpsyms_configure.depends = $$_PRO_FILE_
    dumpsyms_configure.target = $$BREAKPAD_HOSTDIR/Makefile
    dumpsyms_configure.commands = \
        mkdir -p $$BREAKPAD_HOSTDIR && \
        cd $$BREAKPAD_HOSTDIR && \
        $$PWD/src/configure

    dumpsyms_build.depends = dumpsyms_configure
    dumpsyms_build.target = $$BREAKPAD_HOSTDIR/src/tools/linux/dump_syms/dump_syms
    dumpsyms_build.commands = +\$(MAKE) -C $$BREAKPAD_HOSTDIR

    !equals(BREAKPAD_BUILDDIR, $$BREAKPAD_HOSTDIR) {
        QMAKE_EXTRA_TARGETS += dumpsyms_build dumpsyms_configure
        PRE_TARGETDEPS += $$dumpsyms_build.target
    }
}

# Install targets ----------------------------------------------------------------------------------

target.path = $$LIBDIR
target.extra = \
    $$QMAKE_COPY $$shell_path($$BREAKPAD_LIBRARY) \
    $$shell_path($${QMAKE_PREFIX_STATICLIB}KDHockeyAppBreakpad.$${QMAKE_EXTENSION_STATICLIB})
target.files += $${QMAKE_PREFIX_STATICLIB}KDHockeyAppBreakpad.$${QMAKE_EXTENSION_STATICLIB}

INSTALLS += target
]]
