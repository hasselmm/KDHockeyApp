add_library(GoogleBreakpadCommon STATIC) # =============================================================================

target_compile_definitions(GoogleBreakpadCommon PRIVATE -D__STDC_LIMIT_MACROS)
target_compile_definitions(GoogleBreakpadCommon PRIVATE -DUNICODE)

target_include_directories(GoogleBreakpadCommon PUBLIC src/src)

target_sources(GoogleBreakpadCommon PRIVATE src/src/common/convert_UTF.c)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/convert_UTF.h)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/md5.cc)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/md5.h)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/string_conversion.cc)
target_sources(GoogleBreakpadCommon PRIVATE src/src/common/string_conversion.h)

if (CMAKE_SYSTEM_NAME MATCHES "Android|Linux")
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/elfutils.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/elfutils.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/file_id.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/file_id.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/guid_creator.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/guid_creator.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/linux_libc_support.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/linux_libc_support.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/memory_mapped_file.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/memory_mapped_file.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/safe_readlink.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/linux/safe_readlink.h)

    if (CMAKE_SYSTEM_NAME MATCHES "Android")
        target_include_directories(GoogleBreakpadCommon PUBLIC src/src/common/android/include)
        target_sources(GoogleBreakpadCommon PRIVATE src/src/common/android/breakpad_getcontext.S)
        target_link_libraries(GoogleBreakpadCommon PUBLIC -llog)
    endif()
elseif (CMAKE_SYSTEM_NAME MATCHES "iOS")
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/file_id.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/file_id.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/macho_id.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/macho_id.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/macho_utilities.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/macho_utilities.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/macho_walker.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/macho_walker.h)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/string_utilities.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/mac/string_utilities.h)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/windows/guid_string.cc)
    target_sources(GoogleBreakpadCommon PRIVATE src/src/common/windows/guid_string.h)
endif()

add_library(GoogleBreakpadClient STATIC) # =============================================================================

target_compile_definitions(GoogleBreakpadClient PRIVATE -D__STDC_LIMIT_MACROS)
target_compile_definitions(GoogleBreakpadClient PRIVATE -DUNICODE)

target_link_libraries(GoogleBreakpadClient PUBLIC GoogleBreakpadCommon)

if (CMAKE_SYSTEM_NAME MATCHES "Android|Linux")
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/minidump_file_writer.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/minidump_file_writer.h)

    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/crash_generation/crash_generation_client.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/crash_generation/crash_generation_client.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/dump_writer_common/thread_info.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/dump_writer_common/thread_info.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/dump_writer_common/ucontext_reader.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/dump_writer_common/ucontext_reader.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/handler/exception_handler.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/handler/exception_handler.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/handler/minidump_descriptor.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/handler/minidump_descriptor.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/log/log.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/log/log.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/microdump_writer/microdump_writer.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/microdump_writer/microdump_writer.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/linux_dumper.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/linux_dumper.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/linux_ptrace_dumper.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/linux_ptrace_dumper.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/minidump_writer.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/linux/minidump_writer/minidump_writer.h)
elseif (CMAKE_SYSTEM_NAME MATCHES "iOS")
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/minidump_file_writer.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/minidump_file_writer.h)

    target_sources(GoogleBreakpadClient PRIVATE src/src/client/ios/exception_handler_no_mach.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/ios/exception_handler_no_mach.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/mac/handler/breakpad_nlist_64.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/mac/handler/breakpad_nlist_64.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/mac/handler/dynamic_images.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/mac/handler/dynamic_images.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/mac/handler/minidump_generator.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/mac/handler/minidump_generator.h)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    if (MINGW)
        target_compile_options(GoogleBreakpadClient PRIVATE -Wno-conversion-null)
    endif()

    target_sources(GoogleBreakpadClient PRIVATE src/src/client/windows/crash_generation/crash_generation_client.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/windows/crash_generation/crash_generation_client.h)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/windows/handler/exception_handler.cc)
    target_sources(GoogleBreakpadClient PRIVATE src/src/client/windows/handler/exception_handler.h)
endif()


#[[
linux {
    QMAKE_CXXFLAGS_WARN_ON += \
        -Wno-missing-field-initializers \
        -Wno-unused-parameters \
        -Wno-tautological-compare \
        -Wno-tautological-constant-out-of-range-compare \
        -Wno-unused-value
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
