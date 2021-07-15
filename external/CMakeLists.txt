# The list of files is drived from: breakpad/Makefile.am
cmake_minimum_required(VERSION 3.12)
project(sentry_breakpad)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX TRUE)
endif()

# set static runtime, if enabled
if(SENTRY_BUILD_RUNTIMESTATIC AND MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(LINUX)
    option(BREAKPAD_PIC "Build breakpad as position independent code" ON)
else()
    set(BREAKPAD_PIC ON)
endif()
if(BREAKPAD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(BREAKPAD_SOURCES_COMMON
    breakpad/src/common/convert_UTF.cc
    breakpad/src/common/convert_UTF.h
    breakpad/src/common/md5.cc
    breakpad/src/common/md5.h
    breakpad/src/common/string_conversion.cc
    breakpad/src/common/string_conversion.h
)

set(BREAKPAD_SOURCES_COMMON_EXTRA
)

set(BREAKPAD_SOURCES_COMMON_EXTRA_UNIX
    breakpad/src/common/dwarf/bytereader.cc
    breakpad/src/common/dwarf/bytereader.h
    breakpad/src/common/dwarf/dwarf2diehandler.cc
    breakpad/src/common/dwarf/dwarf2diehandler.h
    breakpad/src/common/dwarf/dwarf2reader.cc
    breakpad/src/common/dwarf/dwarf2reader.h
    breakpad/src/common/dwarf/elf_reader.cc
    breakpad/src/common/dwarf/elf_reader.h
    breakpad/src/common/dwarf_cfi_to_module.cc
    breakpad/src/common/dwarf_cfi_to_module.h
    breakpad/src/common/dwarf_cu_to_module.cc
    breakpad/src/common/dwarf_cu_to_module.h
    breakpad/src/common/dwarf_line_to_module.cc
    breakpad/src/common/dwarf_line_to_module.h
    breakpad/src/common/dwarf_range_list_handler.cc
    breakpad/src/common/dwarf_range_list_handler.h
    breakpad/src/common/path_helper.cc
    breakpad/src/common/path_helper.h
    breakpad/src/common/language.cc
    breakpad/src/common/language.h
    breakpad/src/common/module.cc
    breakpad/src/common/module.h
    breakpad/src/common/stabs_to_module.cc
    breakpad/src/common/stabs_to_module.h
    breakpad/src/common/stabs_reader.cc
    breakpad/src/common/stabs_reader.h
)

set(BREAKPAD_SOURCES_COMMON_LINUX
    breakpad/src/common/linux/elf_core_dump.cc
    breakpad/src/common/linux/elfutils.cc
    breakpad/src/common/linux/elfutils.h
    breakpad/src/common/linux/file_id.cc
    breakpad/src/common/linux/file_id.h
    breakpad/src/common/linux/guid_creator.cc
    breakpad/src/common/linux/guid_creator.h
    breakpad/src/common/linux/linux_libc_support.cc
    breakpad/src/common/linux/memory_mapped_file.cc
    breakpad/src/common/linux/safe_readlink.cc
)

set(BREAKPAD_SOURCES_COMMON_EXTRA_LINUX
    breakpad/src/common/linux/crc32.h
    breakpad/src/common/linux/crc32.cc
    breakpad/src/common/linux/dump_symbols.h
    breakpad/src/common/linux/dump_symbols.cc
    breakpad/src/common/linux/elf_symbols_to_module.cc
    breakpad/src/common/linux/elf_symbols_to_module.h
)

set(BREAKPAD_SOURCES_COMMON_LINUX_GETCONTEXT
    breakpad/src/common/linux/breakpad_getcontext.S
)

set(BREAKPAD_SOURCES_COMMON_ANDROID
    breakpad/src/common/android/include/sys/procfs.h
)

set(BREAKPAD_SOURCES_COMMON_WINDOWS
    breakpad/src/common/windows/guid_string.cc
    breakpad/src/common/windows/guid_string.h
)

set(BREAKPAD_SOURCES_COMMON_EXTRA_WINDOWS
    breakpad/src/common/windows/dia_util.cc
    breakpad/src/common/windows/dia_util.h
    breakpad/src/common/windows/omap.cc
    breakpad/src/common/windows/omap.h
    breakpad/src/common/windows/pdb_source_line_writer.cc
    breakpad/src/common/windows/pdb_source_line_writer.h
    breakpad/src/common/windows/pe_source_line_writer.cc
    breakpad/src/common/windows/pe_source_line_writer.h
    breakpad/src/common/windows/pe_util.cc
    breakpad/src/common/windows/pe_util.h
    breakpad/src/common/windows/string_utils.cc
    breakpad/src/common/windows/string_utils-inl.h
)

set(BREAKPAD_SOURCES_COMMON_APPLE
    breakpad/src/common/mac/file_id.cc
    breakpad/src/common/mac/file_id.h
    breakpad/src/common/mac/macho_id.cc
    breakpad/src/common/mac/macho_id.h
    breakpad/src/common/mac/macho_utilities.cc
    breakpad/src/common/mac/macho_utilities.h
    breakpad/src/common/mac/macho_walker.cc
    breakpad/src/common/mac/macho_walker.h
    breakpad/src/common/mac/string_utilities.cc
    breakpad/src/common/mac/string_utilities.h
)

set(BREAKPAD_SOURCES_COMMON_MAC
    breakpad/src/common/mac/MachIPC.mm
    breakpad/src/common/mac/bootstrap_compat.cc
    breakpad/src/common/mac/bootstrap_compat.h
)

set(BREAKPAD_SOURCES_COMMON_EXTRA_APPLE
    breakpad/src/common/mac/arch_utilities.cc
    breakpad/src/common/mac/arch_utilities.h
    breakpad/src/common/mac/dump_syms.cc
    breakpad/src/common/mac/dump_syms.h
    breakpad/src/common/mac/macho_reader.cc
    breakpad/src/common/mac/macho_reader.h
)

set(BREAKPAD_SOURCES_CLIENT_LINUX
    breakpad/src/client/minidump_file_writer-inl.h
    breakpad/src/client/minidump_file_writer.cc
    breakpad/src/client/minidump_file_writer.h
    breakpad/src/client/linux/crash_generation/crash_generation_client.cc
    breakpad/src/client/linux/crash_generation/crash_generation_server.cc
    breakpad/src/client/linux/dump_writer_common/thread_info.cc
    breakpad/src/client/linux/dump_writer_common/ucontext_reader.cc
    breakpad/src/client/linux/handler/exception_handler.cc
    breakpad/src/client/linux/handler/exception_handler.h
    breakpad/src/client/linux/handler/minidump_descriptor.cc
    breakpad/src/client/linux/handler/minidump_descriptor.h
    breakpad/src/client/linux/log/log.cc
    breakpad/src/client/linux/log/log.h
    breakpad/src/client/linux/microdump_writer/microdump_writer.cc
    breakpad/src/client/linux/microdump_writer/microdump_writer.h
    breakpad/src/client/linux/minidump_writer/linux_core_dumper.cc
    breakpad/src/client/linux/minidump_writer/linux_dumper.cc
    breakpad/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
    breakpad/src/client/linux/minidump_writer/minidump_writer.cc
)

set(BREAKPAD_SOURCES_CLIENT_WINDOWS
    breakpad/src/client/windows/crash_generation/crash_generation_client.cc
    breakpad/src/client/windows/crash_generation/crash_generation_client.h
    breakpad/src/client/windows/handler/exception_handler.cc
    breakpad/src/client/windows/handler/exception_handler.h
)

set(BREAKPAD_SOURCES_CLIENT_APPLE
    breakpad/src/client/minidump_file_writer-inl.h
    breakpad/src/client/minidump_file_writer.cc
    breakpad/src/client/minidump_file_writer.h
    breakpad/src/client/mac/handler/breakpad_nlist_64.cc
    breakpad/src/client/mac/handler/breakpad_nlist_64.h
    breakpad/src/client/mac/handler/dynamic_images.cc
    breakpad/src/client/mac/handler/dynamic_images.h
    breakpad/src/client/mac/handler/exception_handler.cc
    breakpad/src/client/mac/handler/exception_handler.h
    breakpad/src/client/mac/handler/minidump_generator.cc
    breakpad/src/client/mac/handler/minidump_generator.h
)

set(BREAKPAD_SOURCES_CLIENT_MAC
    breakpad/src/client/mac/crash_generation/crash_generation_client.cc
    breakpad/src/client/mac/crash_generation/crash_generation_client.h
)

set(BREAKPAD_SOURCES_CLIENT_IOS
    breakpad/src/client/ios/exception_handler_no_mach.cc
    breakpad/src/client/ios/exception_handler_no_mach.h
    breakpad/src/client/ios/handler/ios_exception_minidump_generator.h
    breakpad/src/client/ios/handler/ios_exception_minidump_generator.mm
    breakpad/src/client/mac/crash_generation/ConfigFile.h
    breakpad/src/client/mac/crash_generation/ConfigFile.mm
    breakpad/src/client/mac/handler/mach_vm_compat.h
    breakpad/src/client/mac/handler/protected_memory_allocator.cc
    breakpad/src/client/mac/handler/protected_memory_allocator.h
    breakpad/src/client/mac/handler/ucontext_compat.h
)

set(BREAKPAD_SOURCES_DUMPSYMS_LINUX
    breakpad/src/tools/linux/dump_syms/dump_syms.cc
)

set(BREAKPAD_SOURCES_DUMPSYMS_WINDOWS
    breakpad/src/tools/windows/dump_syms/dump_syms.cc
)

set(BREAKPAD_SOURCES_DUMPSYMS_MAC
    breakpad/src/tools/mac/dump_syms/dump_syms_tool.cc
)

set(BREAKPAD_SOURCES_MACHODUMP_MAC
    breakpad/src/tools/mac/dump_syms/macho_dump.cc
)

add_library(breakpad_common OBJECT)
add_library(breakpad_common_extra OBJECT EXCLUDE_FROM_ALL)
add_library(breakpad_client STATIC)
add_executable(dump_syms)

target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON})
target_link_libraries(breakpad_client PRIVATE $<BUILD_INTERFACE:breakpad_common>)

target_sources(breakpad_common_extra PRIVATE ${BREAKPAD_SOURCES_COMMON_EXTRA})
target_link_libraries(breakpad_common_extra PUBLIC breakpad_common)

target_link_libraries(dump_syms PRIVATE breakpad_common breakpad_common_extra)

if(LINUX OR ANDROID)
    target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON_LINUX})
    target_sources(breakpad_common_extra PRIVATE ${BREAKPAD_SOURCES_COMMON_EXTRA_UNIX} ${BREAKPAD_SOURCES_COMMON_EXTRA_LINUX})
    target_sources(breakpad_client PRIVATE ${BREAKPAD_SOURCES_CLIENT_LINUX})
    target_sources(dump_syms PRIVATE ${BREAKPAD_SOURCES_DUMPSYMS_LINUX})
    if(ANDROID)
        target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON_ANDROID})
        target_include_directories(breakpad_common PUBLIC src/common/android/include)
    endif()
        if(LINUX)
            find_package(Threads REQUIRED)
            target_link_libraries(breakpad_common_extra PRIVATE Threads::Threads)
        endif()

    include(CheckFunctionExists)
    check_function_exists(getcontext HAVE_GETCONTEXT)
    if(HAVE_GETCONTEXT)
        target_compile_definitions(breakpad_common PUBLIC HAVE_GETCONTEXT)
        target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON_LINUX_GETCONTEXT})
    endif()
endif()

if(APPLE)
    target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON_APPLE})
    target_sources(breakpad_common_extra PRIVATE ${BREAKPAD_SOURCES_COMMON_EXTRA_UNIX} ${BREAKPAD_SOURCES_COMMON_EXTRA_APPLE})
    target_sources(breakpad_client PRIVATE ${BREAKPAD_SOURCES_CLIENT_APPLE})
    target_sources(dump_syms PRIVATE ${BREAKPAD_SOURCES_DUMPSYMS_MAC})
    add_executable(macho_dump ${BREAKPAD_SOURCES_MACHODUMP_MAC})
    target_link_libraries(macho_dump PRIVATE breakpad_common breakpad_common_extra)
        target_compile_definitions(breakpad_common PRIVATE HAVE_MACH_O_NLIST_H)
        target_compile_definitions(breakpad_common_extra PRIVATE HAVE_MACH_O_NLIST_H)
    if(NOT IOS)
        target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON_MAC})
        target_sources(breakpad_client PRIVATE ${BREAKPAD_SOURCES_CLIENT_MAC})
    else()
        target_sources(breakpad_client PRIVATE ${BREAKPAD_SOURCES_CLIENT_IOS})
    endif()

    target_link_libraries(breakpad_common PRIVATE "-framework CoreFoundation")
endif()

if(WIN32)
    target_sources(breakpad_common PRIVATE ${BREAKPAD_SOURCES_COMMON_WINDOWS})
    target_sources(breakpad_common_extra PRIVATE ${BREAKPAD_SOURCES_COMMON_EXTRA_WINDOWS})
    target_sources(breakpad_client PRIVATE ${BREAKPAD_SOURCES_CLIENT_WINDOWS})
    target_sources(dump_syms PRIVATE ${BREAKPAD_SOURCES_DUMPSYMS_WINDOWS})

    target_compile_definitions(breakpad_common PUBLIC _UNICODE UNICODE)

    target_compile_options(breakpad_common_extra PUBLIC
        "-I$(VsInstallDir)/DIA SDK/include"
    )
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^([xX]86(_64)?|amd64|AMD64|[iI]?[3-7]86|intel|INTEL)$")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(diaguids_libsubdir "/amd64")
        else()
            set(diaguids_libsubdir "")
        endif()
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|AARCH64|arm.*)$")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(diaguids_libsubdir "/arm64")
        else()
            set(diaguids_libsubdir "/arm")
        endif()
    else()
        message(FATAL_ERROR "Unknown CMAKE_SYSTEM_PROCESSOR (${CMAKE_SYSTEM_PROCESSOR}). Don't know where diaguids library lives.")
    endif()
    target_link_options(breakpad_common_extra PUBLIC
        "-LIBPATH:$(VsInstallDir)/DIA SDK/lib${diaguids_libsubdir}"
    )
    target_link_libraries(breakpad_common_extra PRIVATE dbghelp diaguids imagehlp)
endif()

# breakpad has includes directly to `third_party/lss/...`,
# which are being resolved correctly when we add the current directory to
# the include directories. A giant hack, yes, but it works
target_include_directories(breakpad_common
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src/>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_include_directories(breakpad_client
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
