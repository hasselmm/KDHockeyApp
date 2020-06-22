cmake_minimum_required(VERSION 3.14)

if (NOT BREAKPAD_TARGET_NAME)
    set(BREAKPAD_TARGET_NAME ${CMAKE_SYSTEM_NAME})
endif()

if (CMAKE_CROSSCOMPILING) # ============================================================================================
    message(STATUS "Building native toolchain in ${CMAKE_CURRENT_BINARY_DIR}/toolchain")

    add_custom_command(
        COMMENT Configuring native toolchain for Google Breakpad
        OUTPUT toolchain/CMakeCache.txt
        DEPENDS CMakeLists.txt

        COMMAND ${CMAKE_COMMAND}
                -G${CMAKE_GENERATOR}
                -S${CMAKE_CURRENT_SOURCE_DIR}
                -B${CMAKE_CURRENT_BINARY_DIR}/toolchain
                -DBREAKPAD_TARGET_NAME=${CMAKE_SYSTEM_NAME}
    )

    add_custom_target(
        GoogleBreakpadToolchain ALL
        COMMENT Building native toolchain
        DEPENDS toolchain/CMakeCache.txt
        COMMAND ${CMAKE_COMMAND} --build toolchain --target GoogleBreakpadToolchain
    )
elseif (NOT BREAKPAD_TARGET_NAME MATCHES "Windows")
    add_executable(GoogleBreakpadDumpSyms) # ===========================================================================
    set_property(TARGET GoogleBreakpadDumpSyms PROPERTY OUTPUT_NAME dump_syms)
    target_link_libraries(GoogleBreakpadDumpSyms PUBLIC GoogleBreakpadCommon)

    if (CMAKE_CROSSCOMPILING)
        set_property(TARGET GoogleBreakpadDumpSyms PROPERTY EXCLUDE_FROM_ALL ON)
    endif()

    if (BREAKPAD_TARGET_NAME MATCHES "Android|Linux")
        target_sources(
            GoogleBreakpadDumpSyms PRIVATE
            src/src/tools/linux/dump_syms/dump_syms.cc
        )
    elseif (BREAKPAD_TARGET_NAME MATCHES "iOS|Darwin")
        target_sources(
            GoogleBreakpadDumpSyms PRIVATE
            src/src/tools/mac/dump_syms/dump_syms_tool.cc
        )
#    elseif (BREAKPAD_TARGET_NAME MATCHES "Windows")
#        target_sources(
#            GoogleBreakpadDumpSyms PRIVATE
#            src/src/tools/windows/dump_syms/dump_syms.cc
#        )
    endif()

    add_custom_target(GoogleBreakpadToolchain) # =======================================================================
    add_dependencies(GoogleBreakpadToolchain GoogleBreakpadDumpSyms)
endif()

add_library(GoogleBreakpadCommon STATIC) # =========================================================================

target_compile_definitions(GoogleBreakpadCommon PUBLIC -D__STDC_LIMIT_MACROS)
target_compile_definitions(GoogleBreakpadCommon PUBLIC -DUNICODE)
target_compile_features(GoogleBreakpadCommon PUBLIC cxx_std_14)
target_include_directories(GoogleBreakpadCommon PUBLIC src/src)

target_sources(
    GoogleBreakpadCommon PRIVATE
    src/src/common/convert_UTF.c
    src/src/common/convert_UTF.h
    src/src/common/dwarf/bytereader.cc
    src/src/common/dwarf/bytereader.h
    src/src/common/dwarf/dwarf2diehandler.cc
    src/src/common/dwarf/dwarf2diehandler.h
    src/src/common/dwarf/dwarf2reader.cc
    src/src/common/dwarf/dwarf2reader.h
    src/src/common/dwarf_cfi_to_module.cc
    src/src/common/dwarf_cfi_to_module.h
    src/src/common/dwarf_cu_to_module.cc
    src/src/common/dwarf_cu_to_module.h
    src/src/common/dwarf_line_to_module.cc
    src/src/common/dwarf_line_to_module.h
    src/src/common/language.cc
    src/src/common/language.h
    src/src/common/md5.cc
    src/src/common/md5.h
    src/src/common/module.cc
    src/src/common/module.h
    src/src/common/path_helper.cc
    src/src/common/path_helper.h
    src/src/common/string_conversion.cc
    src/src/common/string_conversion.h
)


if (NOT CMAKE_CROSSCOMPILING)
    target_sources(
        GoogleBreakpadCommon PRIVATE
        src/src/common/dwarf/elf_reader.cc
        src/src/common/dwarf/elf_reader.h
        src/src/common/stabs_reader.cc
        src/src/common/stabs_reader.h
        src/src/common/stabs_to_module.cc
        src/src/common/stabs_to_module.h
    )
endif()

if (MINGW)
    target_include_directories(GoogleBreakpadCommon PUBLIC mingw32glue)
    target_link_libraries(GoogleBreakpadCommon PUBLIC -lWS2_32)

    target_sources(
        GoogleBreakpadCommon PRIVATE
        mingw32glue/stdlib.c
        mingw32glue/stdlib.h
        mingw32glue/string.c
        mingw32glue/string.h
        mingw32glue/sys/mman.c
        mingw32glue/sys/mman.h
        mingw32glue/unistd.c
        mingw32glue/unistd.h
    )
endif()

if (BREAKPAD_TARGET_NAME MATCHES "Android|Linux") # --------------------------------------------------------------------
    if (UNIX)
        target_compile_definitions(GoogleBreakpadCommon PUBLIC -DHAVE_A_OUT_H=1)
    endif()

    target_sources(
        GoogleBreakpadCommon PRIVATE
        src/src/common/linux/crc32.cc
        src/src/common/linux/crc32.h
        src/src/common/linux/elf_symbols_to_module.cc
        src/src/common/linux/elf_symbols_to_module.h
        src/src/common/linux/elfutils.cc
        src/src/common/linux/elfutils.h
        src/src/common/linux/file_id.cc
        src/src/common/linux/file_id.h
        src/src/common/linux/linux_libc_support.cc
        src/src/common/linux/linux_libc_support.h
        src/src/common/linux/memory_mapped_file.cc
        src/src/common/linux/memory_mapped_file.h
    )

    if (CMAKE_CROSSCOMPILING OR UNIX)
        target_sources(
            GoogleBreakpadCommon PRIVATE
            src/src/common/linux/guid_creator.cc
            src/src/common/linux/guid_creator.h
            src/src/common/linux/safe_readlink.cc
            src/src/common/linux/safe_readlink.h
        )
    endif()

    if (NOT CMAKE_CROSSCOMPILING)
        target_sources(
            GoogleBreakpadCommon PRIVATE
            src/src/common/linux/dump_symbols.cc
            src/src/common/linux/dump_symbols.h
        )
    endif()
endif()

if (ANDROID) # ---------------------------------------------------------------------------------------------------------
    target_include_directories(GoogleBreakpadCommon PUBLIC src/src/common/android/include)
    target_link_libraries(GoogleBreakpadCommon PUBLIC -llog)

    target_sources(
        GoogleBreakpadCommon PRIVATE
        src/src/common/android/breakpad_getcontext.S
    )
endif()

if (BREAKPAD_TARGET_NAME MATCHES "iOS|Darwin") # -----------------------------------------------------------------------
    target_compile_definitions(GoogleBreakpadCommon PUBLIC -DHAVE_MACH_O_NLIST_H=1)

    target_sources(
        GoogleBreakpadCommon PRIVATE
        src/src/common/mac/arch_utilities.cc
        src/src/common/mac/arch_utilities.h
        src/src/common/mac/dump_syms.cc
        src/src/common/mac/dump_syms.h
        src/src/common/mac/file_id.cc
        src/src/common/mac/file_id.h
        src/src/common/mac/macho_id.cc
        src/src/common/mac/macho_id.h
        src/src/common/mac/macho_reader.cc
        src/src/common/mac/macho_reader.h
        src/src/common/mac/macho_utilities.cc
        src/src/common/mac/macho_utilities.h
        src/src/common/mac/macho_walker.cc
        src/src/common/mac/macho_walker.h
        src/src/common/mac/string_utilities.cc
        src/src/common/mac/string_utilities.h
    )
endif()

if (BREAKPAD_TARGET_NAME MATCHES "Windows") # --------------------------------------------------------------------------
    target_sources(
        GoogleBreakpadCommon PRIVATE
        src/src/common/windows/guid_string.cc
        src/src/common/windows/guid_string.h
    )
endif()

add_library(GoogleBreakpadClient STATIC) # =============================================================================
target_link_libraries(GoogleBreakpadClient PUBLIC GoogleBreakpadCommon)

if (NOT BREAKPAD_TARGET_NAME MATCHES "Windows")
    target_sources(
        GoogleBreakpadClient PRIVATE
        src/src/client/minidump_file_writer.cc
        src/src/client/minidump_file_writer.h
    )
endif()

if (BREAKPAD_TARGET_NAME MATCHES "Android|Linux") # --------------------------------------------------------------------
    target_sources(
        GoogleBreakpadClient PRIVATE
        src/src/client/linux/crash_generation/crash_generation_client.cc
        src/src/client/linux/crash_generation/crash_generation_client.h
        src/src/client/linux/dump_writer_common/thread_info.cc
        src/src/client/linux/dump_writer_common/thread_info.h
        src/src/client/linux/dump_writer_common/ucontext_reader.cc
        src/src/client/linux/dump_writer_common/ucontext_reader.h
        src/src/client/linux/handler/exception_handler.cc
        src/src/client/linux/handler/exception_handler.h
        src/src/client/linux/handler/minidump_descriptor.cc
        src/src/client/linux/handler/minidump_descriptor.h
        src/src/client/linux/log/log.cc
        src/src/client/linux/log/log.h
        src/src/client/linux/microdump_writer/microdump_writer.cc
        src/src/client/linux/microdump_writer/microdump_writer.h
        src/src/client/linux/minidump_writer/linux_dumper.cc
        src/src/client/linux/minidump_writer/linux_dumper.h
        src/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
        src/src/client/linux/minidump_writer/linux_ptrace_dumper.h
        src/src/client/linux/minidump_writer/minidump_writer.cc
        src/src/client/linux/minidump_writer/minidump_writer.h
    )
elseif (BREAKPAD_TARGET_NAME MATCHES "iOS|Darwin") # -------------------------------------------------------------------
    target_sources(
        GoogleBreakpadClient PRIVATE
        src/src/client/ios/exception_handler_no_mach.cc
        src/src/client/ios/exception_handler_no_mach.h
        src/src/client/mac/handler/breakpad_nlist_64.cc
        src/src/client/mac/handler/breakpad_nlist_64.h
        src/src/client/mac/handler/dynamic_images.cc
        src/src/client/mac/handler/dynamic_images.h
        src/src/client/mac/handler/minidump_generator.cc
        src/src/client/mac/handler/minidump_generator.h
    )
elseif (BREAKPAD_TARGET_NAME MATCHES "Windows") # ----------------------------------------------------------------------
    if (MINGW)
        target_compile_options(GoogleBreakpadClient PRIVATE -Wno-conversion-null)
    endif()

    target_sources(
        GoogleBreakpadClient PRIVATE
        src/src/client/windows/crash_generation/crash_generation_client.cc
        src/src/client/windows/crash_generation/crash_generation_client.h
        src/src/client/windows/handler/exception_handler.cc
        src/src/client/windows/handler/exception_handler.h
    )
endif()
