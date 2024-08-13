cmake_minimum_required(VERSION 3.24)
project(Milody)

option(MILODY_BUILD_SHARED_LIBS "Build shared library" ON)
option(MILODY_BUILD_FRAMEWORK_LIBS "Build framework library" OFF)
option(MILODY_ENABLE_CLI "Enable cli" ON)
option(MILODY_ENABLE_TESTS "Enable tests" ON)
option(MILODY_WITH_ADDRESS_SANITIZER "Build with clang address sanitizer" OFF)
option(MILODY_WITH_JAVASCRIPTCORE "Build with JavaScript Core on the APPLE platform" ON)

set(CMAKE_CXX_STANDARD 20)

# Need to explicitly disable scanning
# Reference: https://www.kitware.com/import-cmake-the-experiment-is-over/
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

#cmake_policy(SET CMP0063 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

# Keep this option OFF
# The third-party libraries might decide to build dynamic libraries based on this option,
# but we want to only generate static libraries in all cases.
set(BUILD_SHARED_LIBS OFF)

# Because the current Windows build uniformly uses MSYS2 Clang, it is necessary to use statically link for all targets.
# e.g. googletest
if (WIN32)
    if (MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    endif ()
endif ()

if (MILODY_WITH_ADDRESS_SANITIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-recover=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address -fsanitize-recover=address")
endif ()

if (BUILD_SHARED_LIBS)
    message(FATAL_ERROR "Please do not set BUILD_SHARED_LIBS")
endif ()

if (MILODY_BUILD_SHARED_LIBS)
    add_compile_definitions(MILODY_DLL MILODY_BUILDING_SHARED_LIBS)
    set(PROJECT_LIBRARY_PREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}")
    set(PROJECT_LIBRARY_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
else ()
    set(PROJECT_LIBRARY_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
    set(PROJECT_LIBRARY_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif ()


#############################################################
# Setup project
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/../lib/release:$ORIGIN/../lib/debug")
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# Read the version information from the VERSION file
file(STRINGS "${PROJECT_SOURCE_DIR}/VERSION" PACKAGE_VERSION)
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION})

set(PROJECT_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(PROJECT_MODULE_EXPORT_NAMESPACE "${PROJECT_NAME}::")
add_compile_definitions(MILODY_VERSION="${PROJECT_VERSION}")
add_compile_definitions(MILODY_VERSION_MAJOR=${CPACK_PACKAGE_VERSION_MAJOR})
add_compile_definitions(MILODY_VERSION_MINOR=${CPACK_PACKAGE_VERSION_MINOR})
add_compile_definitions(MILODY_VERSION_PATCH=${CPACK_PACKAGE_VERSION_PATCH})

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf
# directories. This simplifies manual testing and the use of the build
# tree rather than installed Boost libraries.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Windows DLLs are "runtime" for CMake. Output them to "bin" like the Visual Studio projects do.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Reset output dirs for multi-config builds
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach (OUTPUTCONFIG)

# Append our module directory to CMake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

include(MilodyMacros)

if (ANDROID)
    # 检查 ANDROID_ABI 变量的值，并根据其值设置架构变量
    if (ANDROID_ABI STREQUAL "arm64-v8a")
        set(MILODY_TARGET_ARCH "aarch64")
    else ()
        message(FATAL_ERROR "Unsupported ANDROID_ABI: ${ANDROID_ABI}")
    endif ()

    # 输出设置的架构
    message(STATUS "MILODY_TARGET_ARCH: ${MILODY_TARGET_ARCH}")

    # 提取版本号
    if (NOT DEFINED ANDROID_PLATFORM)
        set(ANDROID_PLATFORM "android-23")
    endif ()
    message(STATUS "ANDROID_PLATFORM: ${ANDROID_PLATFORM}")
    string(REGEX REPLACE ".*android-([0-9]+).*" "\\1" ANDROID_SDK_VERSION ${ANDROID_PLATFORM})
    message(STATUS "ANDROID_SDK_VERSION: ${ANDROID_SDK_VERSION}")

    if (CMAKE_HOST_WIN32)
        set(NDK_SCRIPT_SUFFIX ".cmd")
        set(NDK_SCRIPT_SUFFIX_EXE ".exe")
    else ()
        set(NDK_SCRIPT_SUFFIX "")
        set(NDK_SCRIPT_SUFFIX_EXE "")
    endif ()
    message(STATUS "NDK_SCRIPT_SUFFIX: ${NDK_SCRIPT_SUFFIX}")
endif ()

if (NOT APPLE)
    set(MILODY_WITH_JAVASCRIPTCORE OFF)
endif ()

#############################################################
# Setup external libraries
include(ExternalProject)

add_compile_definitions(JUCE_USE_FLAC)
add_compile_definitions(JUCE_USE_OGGVORBIS)
add_compile_definitions(JUCE_USE_MP3AUDIOFORMAT)
add_compile_definitions(JUCE_USE_WINDOWS_MEDIA_FORMAT)
add_compile_definitions(JUCE_USE_CURL=0)
add_compile_definitions(JUCE_USE_WINRT_MIDI=0)
add_compile_definitions(JUCE_WEB_BROWSER=0)
IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(JUCE_FORCE_DEBUG=1)
    add_compile_definitions(JUCE_LOG_ASSERTIONS=1)
    add_compile_definitions(JUCE_CHECK_MEMORY_LEAKS=1)
else ()
    add_compile_definitions(JUCE_FORCE_DEBUG=0)
    add_compile_definitions(JUCE_LOG_ASSERTIONS=0)
    add_compile_definitions(JUCE_CHECK_MEMORY_LEAKS=0)
endif ()
if (DEFINED APPLE)
    file(GLOB_RECURSE OBJC_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/ext/juce/*.m"
            "${CMAKE_CURRENT_SOURCE_DIR}/ext/juce/*.mm")
    foreach (SRC IN LISTS OBJC_SOURCES)
        set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "-fno-objc-arc")
    endforeach ()
endif ()
# The _NO_RESOURCERC option is private, and is only intended for use when building juceaide.
# We can't add a resources.rc to juceaide because we need juceaide to generate the resources.rc!
set(JUCE_ARG__NO_RESOURCERC ON)
add_compile_definitions(JUCE_STANDALONE_APPLICATION=1)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(_CONSOLE=1)
endif ()
add_subdirectory(ext/juce EXCLUDE_FROM_ALL)

set(EXT_PKG_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/ext/pkginstall")

add_subdirectory("ext/json" EXCLUDE_FROM_ALL)

set(CLI11_PRECOMPILED ON)
add_subdirectory("ext/cli11" EXCLUDE_FROM_ALL)

set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_SHARED_LIB OFF)
set(SPDLOG_BUILD_PIC ON)
set(SPDLOG_BUILD_TESTS OFF)
add_subdirectory("ext/spdlog" EXCLUDE_FROM_ALL)

add_subdirectory("ext/PluginAPI" EXCLUDE_FROM_ALL)

#############################################################
# Setup Milody
set(MILODY_DEFINITIONS "MILODY_BUILDING_ENV")
set(MILODY_CLI_DEFINITIONS "MILODY_BUILDING_ENV")

set(MILODY_INCLUDES ${PROJECT_PLATFORM_INCLUDES} ${MILODY_SKIA_INCLUDE_PATH})
set(MILODY_CLI_INCLUDES ${PROJECT_PLATFORM_INCLUDES} ${MILODY_SKIA_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/ext/json/include")

set(MILODY_LIBRARIES ${PROJECT_PLATFORM_LIBRARIES})
set(MILODY_CLI_LIBRARIES ${PROJECT_PLATFORM_LIBRARIES})

if (MILODY_ENABLE_CLI)
    set(MILODY_DEFINITIONS ${MILODY_DEFINITIONS} "MILODY_USE_CLI")
    set(MILODY_CLI_DEFINITIONS ${MILODY_CLI_DEFINITIONS} "MILODY_USE_CLI")
endif ()

set(
        MILODY_CORE_FILE

        src/audio/milody_audio_type.cpp
        src/audio/milody_juce_audio_buffer.cpp
        src/audio/milody_juce_audio_device_manager.cpp
        src/audio/milody_juce_audio_source_player.cpp
        src/audio/milody_juce_audio_format_reader.cpp
        src/audio/milody_juce_iir_filter_audio_source.cpp
        src/audio/milody_juce_logger_proxy.cpp
        src/audio/milody_juce_sound_player_wrapper.cpp
        src/audio/milody_juce_sound_player.cpp
        src/audio/milody_juce_time_slice_thread.cpp
        src/audio/milody_juce_audio_format_manager.cpp
        src/io/milody_io.cpp
        src/log/log.cpp
        src/log/log_unitylog.cpp
        src/util/milody_binaryreader.cpp
        src/util/milody_binarywriter.cpp
        src/platform/milody_windows.cpp
        src/context/context.cpp
)

set(
        MILODY_APPLICATION_FILE
        src/game/milody_game_audio_audio_buffer.cpp
        src/game/milody_game_audio_format_reader.cpp
        src/game/milody_game_audio_juce_audio_source_player.cpp
        src/game/milody_game_audio_audio_device_manager.cpp
        src/game/milody_game_audio_sound_player.cpp
        src/game/milody_game_plugin_main.cpp
        src/game/milody_game_context.cpp
)

set(
        MILODY_CMD_FILE
        apps/cmd/biz/Audio.cpp
        apps/cmd/biz/Audio.h
        apps/cmd/config/Cmd.cpp
        apps/cmd/config/Cmd.h
        apps/cmd/config/CmdFlags.cpp
        apps/cmd/config/CmdFlags.h
        apps/cmd/config/Config.cpp
        apps/cmd/config/Config.h
        apps/cmd/utils/CliValidator.cpp
        apps/cmd/utils/CliValidator.h
        apps/cmd/utils/CollectionsHelper.cpp
        apps/cmd/utils/CollectionsHelper.h
        apps/cmd/cli.cpp
)

set(
        PUBLIC_GAME_INTERFACE
        include/Milody/common/milody_export_macros.h
        include/Milody/game/milody_game_types.h
        include/Milody/game/milody_game_interface.h
)

set(MILODY_CORE_FILE ${MILODY_CORE_FILE} ${PUBLIC_GAME_INTERFACE})

if (DEFINED APPLE)
    enable_language(Swift)
    enable_language(OBJC)
    enable_language(OBJCXX)

    if ("${CMAKE_Swift_COMPILER_VERSION}" VERSION_LESS 5.9)
        message(FATAL_ERROR "Bidirectional C++ Interop requires Swift 5.9 or greater. Have ${CMAKE_Swift_COMPILER_VERSION}")
    endif ()

    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        message(FATAL_ERROR "Project requires building with Clang. Have ${CMAKE_CXX_COMPILER_ID}")
    endif ()

    include(InitializeSwift)
    include(AddSwift)

    find_library(FOUNDATION Foundation)
    set(PROJECT_PLATFORM_LIBRARIES ${PROJECT_PLATFORM_LIBRARIES} "${FOUNDATION}")
endif ()

if (DEFINED APPLE AND DEFINED IOS)
    find_library(UIKIT UIKit)
    set(PROJECT_PLATFORM_LIBRARIES ${PROJECT_PLATFORM_LIBRARIES} "${UIKIT}")
endif ()


# We will not use BUILD_SHARED_LIBS, and you can see the above comment for reasons.
if (MILODY_BUILD_SHARED_LIBS)
    add_library(Milody SHARED ${MILODY_CORE_FILE} ${MILODY_APPLICATION_FILE})
else ()
    add_library(Milody STATIC ${MILODY_CORE_FILE} ${MILODY_APPLICATION_FILE})
endif ()

if (MSVC)
    set_target_properties(Milody PROPERTIES RUNTIME_OUTPUT_NAME "libMilody")
endif ()

# Only Apple Can Do
if (DEFINED APPLE AND DEFINED IOS AND MILODY_BUILD_FRAMEWORK_LIBS)
    set_target_properties(
            Milody PROPERTIES
            FRAMEWORK TRUE
            FRAMEWORK_VERSION C
            MACOSX_FRAMEWORK_INFO_PLIST "${PROJECT_SOURCE_DIR}/Info.plist"
            PUBLIC_HEADER "${PUBLIC_GAME_INTERFACE}"
    )
endif ()

_juce_initialise_target(Milody)
set_target_properties(Milody PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(PROJECT_PLATFORM_INCLUDES ${PROJECT_PLATFORM_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}/include)

if (DEFINED APPLE AND NOT DEFINED IOS)
    target_compile_options(Milody PUBLIC "$<$<COMPILE_LANGUAGE:Swift>:-cxx-interoperability-mode=default>")
endif ()

target_compile_definitions(Milody PUBLIC ${MILODY_DEFINITIONS})
target_link_libraries(Milody
        PRIVATE
        ${PROJECT_CODEC_LIBRARIES}
        ${PROJECT_PLATFORM_LIBRARIES}
        nlohmann_json::nlohmann_json
        UnityPluginAPI
        PUBLIC
        juce::juce_core
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_dsp
        spdlog::spdlog
)
if (LINUX)
    target_link_libraries(Milody
            PRIVATE
            dl
    )
endif ()

target_include_directories(Milody
        PUBLIC
        include
        ${PROJECT_CODEC_INCLUDES}
        PRIVATE
        src
)

if (NOT DEFINED IOS)
    # Install the lib (without header files)
    install(TARGETS Milody
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif ()


# Make XCode Happy
if (DEFINED DEVELOPMENT_TEAM_ID)
    SET_XCODE_PROPERTY(Milody DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID} "All")
endif ()
if (DEFINED CODESIGNIDENTITY)
    SET_XCODE_PROPERTY(Milody CODE_SIGN_IDENTITY ${CODESIGNIDENTITY} "All")
endif ()
if (DEFINED PROVISIONING_PROFILE_NAME)
    SET_XCODE_PROPERTY(Milody PROVISIONING_PROFILE_SPECIFIER ${PROVISIONING_PROFILE_NAME} "All")
endif ()
#if (MSVC)
#    target_compile_options(Milody BEFORE PRIVATE "/EHsc")
#else ()
#    target_compile_options(Milody BEFORE PRIVATE "-fno-exceptions")
#endif ()
if (WIN32)
    if (MINGW)
        target_link_options(Milody BEFORE PRIVATE "-static")
    endif ()
elseif (LINUX)
    if (NOT ANDROID)
        # Excluding Android
        # Android defaults to static linking with libc++, no tinkering needed.
        target_link_options(Milody BEFORE PRIVATE "-static-libstdc++")
    endif ()
    #        elseif (APPLE)
    #            target_link_options(MilodyCli BEFORE PRIVATE "-static-libgcc -static-libstdc++")
endif ()

if (MILODY_ENABLE_TESTS)
    include(CTest)
    enable_testing()

    add_subdirectory(ext/googletest EXCLUDE_FROM_ALL)
    add_subdirectory(tests)
    message(STATUS "Building with tests")
else ()
    message(STATUS "Building without tests")
endif ()


if (MILODY_ENABLE_CLI)
    add_executable(MilodyCli ${MILODY_CMD_FILE})
    target_compile_definitions(MilodyCli PRIVATE Milody ${MILODY_CLI_DEFINITIONS})
    target_link_libraries(MilodyCli
            PRIVATE
            Milody
            ${PROJECT_CLI_LIBRARIES}
            nlohmann_json::nlohmann_json
            CLI11::CLI11
    )
    target_include_directories(MilodyCli PRIVATE ${MILODY_CLI_INCLUDES})

    if (WIN32)
        if (MINGW)
            target_link_options(MilodyCli BEFORE PRIVATE "-static")
        endif ()
    elseif (LINUX)
        if (NOT ANDROID)
            # Excluding Android
            # Android defaults to static linking with libc++, no tinkering needed.
            target_link_options(MilodyCli BEFORE PRIVATE "-static-libstdc++")
        endif ()
        #        elseif (APPLE)
        #            target_link_options(MilodyCli BEFORE PRIVATE "-static-libgcc -static-libstdc++")
    endif ()

    install(TARGETS MilodyCli DESTINATION bin)
endif ()

if (CMAKE_HOST_WIN32)
    add_custom_target(H2CS
            COMMAND cd ${PROJECT_SOURCE_DIR} & cmd /c gradlew.bat h2cs -DsourceFilePath="${PROJECT_SOURCE_DIR}/include/Milody/game/milody_game_interface.h" -DcsharpBindingOutputPath="${CMAKE_CURRENT_BINARY_DIR}/BindingC.cs" -DcppFrameworkBindingOutputPath="${CMAKE_CURRENT_BINARY_DIR}/FrameworkBinding.cpp"
    )
else ()
    add_custom_target(H2CS
            COMMAND cd ${PROJECT_SOURCE_DIR} && ./gradlew h2cs -DsourceFilePath="${PROJECT_SOURCE_DIR}/include/Milody/game/milody_game_interface.h" -DcsharpBindingOutputPath="${CMAKE_CURRENT_BINARY_DIR}/BindingC.cs" -DcppFrameworkBindingOutputPath="${CMAKE_CURRENT_BINARY_DIR}/FrameworkBinding.cpp"
    )
endif ()

#############################################################
# Show CMake Config
MILODY_SHOW_CONFIG()
