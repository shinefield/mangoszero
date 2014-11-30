# mangos-zero is a full featured server for World of Warcraft in its vanilla
# version, supporting clients for patch 1.12.x.
#
# Copyright (C) 2005-2014  MaNGOS project  <http://getmangos.com>
# Copyright (C) 2013-2014  CMaNGOS project <http://cmangos.net>
#
# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# ***** END GPL LICENSE BLOCK *****
#
# World of Warcraft, and all World of Warcraft or Warcraft art, images,
# and lore are copyrighted by Blizzard Entertainment, Inc.

#-----------------------------------------------------------------------------
# Initialize project.

project(mangos-zero C CXX)

set(APPLICATION_NAME            ${PROJECT_NAME})
set(APPLICATION_VERSION_MAJOR   "1")
set(APPLICATION_VERSION_MINOR   "1")
set(APPLICATION_VERSION_RELEASE "0")
set(APPLICATION_VERSION         "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_RELEASE}")

#-----------------------------------------------------------------------------
# We don't allow in-source builds. This causes no end of troubles because
# all out-of-source builds will use the CMakeCache.txt file there and even
# build the libs and objects in it.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    if(NOT DEFINED WITH_IN_SOURCE_BUILD)
        message(FATAL_ERROR
            "CMake generation for ${CMAKE_PROJECT_NAME} is not allowed within the source directory!"
            "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
            "\n "
            "\n rm CMakeCache.txt"
            "\n rm -r CMakeFiles"
            "\n mkdir _build"
            "\n cd _build"
            "\n cmake ../"
        )
    endif(NOT DEFINED WITH_IN_SOURCE_BUILD)
endif()

#-----------------------------------------------------------------------------
# We require CMake version 2.8 or newer. Older versions do not detect Visual
# Studio properly, especially the Express versions.
cmake_minimum_required(VERSION 2.8)

#-----------------------------------------------------------------------------
# Ensure we print configuration summary only on the initial CMake run
if(NOT EXECUTABLE_OUTPUT_PATH)
    set(FIRST_RUN "TRUE")
endif(NOT EXECUTABLE_OUTPUT_PATH)

#-----------------------------------------------------------------------------
# We provide a few modules for our own needs to find and configure uncommon
# software on which we depend.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#-----------------------------------------------------------------------------
# We do build in Release mode by default to provide the fastest build
# experience.
set(CMAKE_BUILD_TYPE_INIT "Release")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#-----------------------------------------------------------------------------
# If quiet output for Makefiles is desired, set this to ON. 'make -s' is an
# alternative.
# set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

#-----------------------------------------------------------------------------
# Global compile definitions since add_definitions() adds for all.
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:_DEBUG>)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>:NDEBUG>)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:RelWithDebInfo>:NDEBUG>)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:MinSizeRel>:NDEBUG>)

#-----------------------------------------------------------------------------
# Set policies

# see "cmake --help-policy CMP0003"
# So library linking is more sane
cmake_policy(SET CMP0003 NEW)

# see "cmake --help-policy CMP0005"
# So BUILDINFO and MANGOSPATH strings are automatically quoted
cmake_policy(SET CMP0005 NEW)

# see "cmake --help-policy CMP0010"
# So syntax problems are errors
cmake_policy(SET CMP0010 NEW)

# see "cmake --help-policy CMP0014"
# Input directories must have CMakeLists.txt
cmake_policy(SET CMP0014 NEW)

#-----------------------------------------------------------------------------
# Redirect output files
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

#-----------------------------------------------------------------------------
# Set default configuration options

# mangos-zero internal features
option(WITH_DEBUG_OUTPUT            "Enable ${CMAKE_PROJECT_NAME} debug output"             OFF)

# mangos-zero script extensions
option(WITH_MZ_SCRIPTS              "Enable ${CMAKE_PROJECT_NAME} script extensions"        OFF)
mark_as_advanced(WITH_MZ_SCRIPTS)

# Modifiers
option(WITH_RECAST_DEMO             "Build the Recast movement map viewer application"      OFF)
mark_as_advanced(WITH_RECAST_DEMO)

#-----------------------------------------------------------------------------
# Check for conflicting/unsupported configurations
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND WITH_DEBUG_OUTPUT)
    message(FATAL_ERROR "It is not recommeded to build ${CMAKE_PROJECT_NAME} with debug output in a non-development release.")
endif()

#-----------------------------------------------------------------------------
# Initialize uncached variables, avoid unused warning
# Linux only, not cached
set(WITH_BINRELOC OFF)

# MAXOSX only, set to avoid uninitialized
set(EXETYPE "")

# C/C++ flags
set(PLATFORM_CFLAGS)

# these are added to later on.
set(C_WARNINGS)
set(CXX_WARNINGS)

# for gcc -Wno-blah-blah
set(CC_REMOVE_STRICT_FLAGS)

# libraries to link the binary with passed to target_link_libraries()
set(PLATFORM_LINKLIBS "")

# Added to linker flags in setup_liblinks
# - CMAKE_EXE_LINKER_FLAGS
# - CMAKE_EXE_LINKER_FLAGS_DEBUG
set(PLATFORM_LINKFLAGS "")
set(PLATFORM_LINKFLAGS_DEBUG "")

# Configure target directories
set(BIN_DIR       "${CMAKE_INSTALL_PREFIX}/bin")
set(CONF_DIR      "${CMAKE_INSTALL_PREFIX}/etc")
set(INCL_DIR      "${CMAKE_INSTALL_PREFIX}/include")
set(LIBS_DIR      "${CMAKE_INSTALL_PREFIX}/lib")
set(DATA_DIR      "${CMAKE_INSTALL_PREFIX}/share/${APPLICATION_NAME}")

# For Unix systems set the rpath so that libraries are found
set(CMAKE_INSTALL_RPATH             ${LIBS_DIR})
set(CMAKE_INSTALL_NAME_DIR          ${LIBS_DIR})

# Run out of build tree
set(CMAKE_BUILD_WITH_INSTALL_RPATH  OFF)

#-----------------------------------------------------------------------------
# Platform specifics

if(UNIX AND NOT APPLE)
    find_package(Readline)

    # OpenSuse needs lutil, ArchLinux not, for now keep, can avoid by using --as-needed
    set(PLATFORM_LINKLIBS "-lutil -lc -lm -lpthread -lstdc++")

    set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} -pthread")

    # lfs on glibc, all compilers should use
    add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE)

    # GNU Compiler
    if(CMAKE_COMPILER_IS_GNUCC)
        set(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing")
    # CLang is the same as GCC for now.
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing")
    # Solaris CC
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "SunPro")
        set(PLATFORM_CFLAGS "-pipe -features=extensions -fPIC -D__FUNCTION__=__func__")

    # Intel C++ Compiler
    elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
        # think these next two are broken
        find_program(XIAR xiar)
        if(XIAR)
            set(CMAKE_AR "${XIAR}")
        endif()
        mark_as_advanced(XIAR)

        find_program(XILD xild)
        if(XILD)
            set(CMAKE_LINKER "${XILD}")
        endif()
        mark_as_advanced(XILD)

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fp-model precise -prec_div -parallel")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fp-model precise -prec_div -parallel")

        # set(PLATFORM_CFLAGS "${PLATFORM_CFLAGS} -diag-enable sc3")
        set(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing")
        set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} -static-intel")
    endif()

elseif(WIN32)

    # this file is included anyway when building under Windows with cl.exe
    #  include(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)

    if(CMAKE_COMPILER_IS_GNUCC)
        include(CheckCSourceCompiles)
        # Setup 64bit and 64bit windows systems
        CHECK_C_SOURCE_COMPILES("
            #ifndef __MINGW64__
            #error
            #endif
            int main(void) { return 0; }
            "
            WITH_MINGW64)

        if(WITH_MINGW64)
            message(STATUS "Compiling for 64 bit with MinGW-w64.")
        else()
            message(STATUS "Compiling for 32 bit with MinGW-w32.")
        endif()
    else()
        # Setup 64bit and 64bit windows systems
        if(CMAKE_CL_64)
            message(STATUS "64 bit compiler detected.")
            if(MSVC11)
                message(STATUS "Visual C++ 2012 detected.")
            else()
                message(STATUS "Visual C++ detected.")
            endif()
        else()
        # Setup 32bit windows systems
            if(MSVC11)
                message(STATUS "Visual C++ 2012 detected.")
            else()
                message(STATUS "Visual C++ detected.")
            endif()
        endif()
    endif()

    add_definitions(-DWIN32)

    if(MSVC)
        set(PLATFORM_LINKLIBS ws2_32 vfw32 winmm kernel32 user32 gdi32 comdlg32 advapi32 shfolder shell32 ole32 oleaut32 uuid psapi)

        add_definitions(/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_DEPRECATE /D_CONSOLE /D_LIB)

        # MSVC11 needs _ALLOW_KEYWORD_MACROS to build
        if(MSVC11)
            add_definitions(/D_ALLOW_KEYWORD_MACROS)
        endif()

        set(CMAKE_CXX_FLAGS "/nologo /J /Gd /EHsc /MP" CACHE STRING "MSVC MT C++ flags " FORCE)
        set(CMAKE_C_FLAGS   "/nologo /J /Gd"           CACHE STRING "MSVC MT C++ flags " FORCE)

        if(CMAKE_CL_64)
            set(CMAKE_CXX_FLAGS_DEBUG "/Od /RTC1 /MDd /Zi /bigobj" CACHE STRING "MSVC MT flags " FORCE)
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Ob1 /MD /Zi /bigobj" CACHE STRING "MSVC MT flags " FORCE)
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "/Od /RTC1 /MDd /ZI" CACHE STRING "MSVC MT flags " FORCE)
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Ob1 /MD /Zi" CACHE STRING "MSVC MT flags " FORCE)
        endif()
        set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /MD" CACHE STRING "MSVC MT flags " FORCE)
        set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /Ob1 /MD" CACHE STRING "MSVC MT flags " FORCE)
        if(CMAKE_CL_64)
            set(CMAKE_C_FLAGS_DEBUG "/Od /RTC1 /MDd /Zi" CACHE STRING "MSVC MT flags " FORCE)
        else()
            set(CMAKE_C_FLAGS_DEBUG "/Od /RTC1 /MDd /ZI" CACHE STRING "MSVC MT flags " FORCE)
        endif()
        set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob2 /MD" CACHE STRING "MSVC MT flags " FORCE)
        set(CMAKE_C_FLAGS_MINSIZEREL "/O1 /Ob1 /MD" CACHE STRING "MSVC MT flags " FORCE)
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "/O2 /Ob1 /MD /Zi" CACHE STRING "MSVC MT flags " FORCE)

        # most msvc warnings are C & C++
        set(_WARNINGS "/W3 /wd4018 /wd4244 /wd4305 /wd4800 /wd4181 /wd4065 /wd4267 /we4013 /wd4200")
        set(C_WARNINGS "${_WARNINGS}")
        set(CXX_WARNINGS "${_WARNINGS}")
        unset(_WARNINGS)

        set(PLATFORM_LINKFLAGS "/SUBSYSTEM:CONSOLE /STACK:2097152 /INCREMENTAL:NO /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:msvcmrt.lib /NODEFAULTLIB:msvcurt.lib /NODEFAULTLIB:msvcrtd.lib")

        # MSVC only, Mingw doesnt need
        if(CMAKE_CL_64)
            set(PLATFORM_LINKFLAGS "/MACHINE:X64 /OPT:NOREF ${PLATFORM_LINKFLAGS}")
        else()
            set(PLATFORM_LINKFLAGS "/MACHINE:IX86 /LARGEADDRESSAWARE ${PLATFORM_LINKFLAGS}")
        endif()

        set(PLATFORM_LINKFLAGS_DEBUG "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libc.lib")

    elseif(CMAKE_COMPILER_IS_GNUCC)
    # keep GCC specific stuff here
        set(PLATFORM_LINKLIBS "-lshell32 -lshfolder -lgdi32 -lmsvcrt -lwinmm -lmingw32 -lm -lws2_32 -lz -lstdc++ -lole32 -luuid -lwsock32 -lpsapi")
        set(PLATFORM_CFLAGS "-pipe -funsigned-char -fno-strict-aliasing")

        if(WITH_MINGW64)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
            set(PLATFORM_LINKLIBS "${PLATFORM_LINKLIBS} -lpthread")

            add_definitions(-DFREE_WINDOWS64 -DMS_WIN64)
            # Turn off OpenMP since it causes crashes on render for subsurfed/multiresolution meshes
            set(WITH_OPENMP OFF)
        endif()

        add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE)

        add_definitions(-DFREE_WINDOWS)

        set(PLATFORM_LINKFLAGS "-Xlinker --stack=2097152")

        ## DISABLE - causes linking errors
        ## for re-distribution, so users don't need mingw installed
        # set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} -static-libgcc -static-libstdc++")

    endif()

elseif(APPLE)

    if(${CMAKE_VERSION} VERSION_LESS 3.0) # else breaks setting CMP0043 policy
        cmake_minimum_required(VERSION 2.8.8)
        cmake_policy(VERSION 2.8.8)
    endif()

    if(NOT CMAKE_OSX_ARCHITECTURES)
        set(CMAKE_OSX_ARCHITECTURES x86_64 CACHE STRING
        "Choose the architecture you want to build Blender for: i386, x86_64 or ppc"
        FORCE)
    endif()

    execute_process(COMMAND uname -r OUTPUT_VARIABLE MAC_SYS) # check for actual system-version
    if(${MAC_SYS} MATCHES 14)
        set(OSX_SYSTEM 10.10)
        # throw an error here, older cmake cannot handle 2 digit subversion!
        cmake_minimum_required(VERSION 3.0.0)
    elseif(${MAC_SYS} MATCHES 13)
        set(OSX_SYSTEM 10.9)
    elseif(${MAC_SYS} MATCHES 12)
        set(OSX_SYSTEM 10.8)
    elseif(${MAC_SYS} MATCHES 11)
        set(OSX_SYSTEM 10.7)
    elseif(${MAC_SYS} MATCHES 10)
        set(OSX_SYSTEM 10.6)
    elseif(${MAC_SYS} MATCHES 9)
        set(OSX_SYSTEM 10.5)
    else()
        set(OSX_SYSTEM unsupported)
    endif()
    message(STATUS "Detected system-version: " ${OSX_SYSTEM})

    # workaround for incorrect cmake xcode lookup for developer previews - XCODE_VERSION does not take xcode-select path into accout
    # but would always look into /Applications/Xcode.app while dev versions are named Xcode<version>-DP<preview_number>
    execute_process(COMMAND xcode-select --print-path  OUTPUT_VARIABLE XCODE_CHECK OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REPLACE "/Contents/Developer" "" XCODE_BUNDLE ${XCODE_CHECK}) # truncate to bundlepath in any case
    message(STATUS "Xcode-bundle : " ${XCODE_BUNDLE})

    if(${CMAKE_GENERATOR} MATCHES "Xcode")

        if(${XCODE_VERSION} VERSION_GREATER 4.2) # earlier xcode has no bundled developer dir, no sense in getting xcode path from
            string(SUBSTRING "${XCODE_CHECK}" 14 6 DP_NAME) # reduce to XCode name without dp extension
            if(${DP_NAME} MATCHES Xcode5)
                set(XCODE_VERSION 5)
            endif()
        endif()

        ##### cmake incompatibility with xcode  4.3 and higher #####
        if(${XCODE_VERSION} MATCHES '') # cmake fails due looking for xcode in the wrong path, thus will be empty var
            message(FATAL_ERROR "Xcode 4.3 and higher must be used with cmake 2.8-8 or higher")
        endif()
        ### end cmake incompatibility with xcode 4.3 and higher ###

        if(${XCODE_VERSION} VERSION_EQUAL 4 OR ${XCODE_VERSION} VERSION_GREATER 4 AND ${XCODE_VERSION} VERSION_LESS 4.3)
            # Xcode 4 defaults to the Apple LLVM Compiler.
            # Override the default compiler selection because Blender only compiles with gcc up to xcode 4.2
            set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvmgcc42")
            message(STATUS "Setting compiler to: " ${CMAKE_XCODE_ATTRIBUTE_GCC_VERSION})
        endif()
    else() # unix makefile generator does not fill XCODE_VERSION var, so we get it with a command
        execute_process(COMMAND xcodebuild -version OUTPUT_VARIABLE XCODE_VERS_BUILD_NR)
        string(SUBSTRING "${XCODE_VERS_BUILD_NR}" 6 3 XCODE_VERSION) # truncate away build-nr
        unset(XCODE_VERS_BUILD_NR)
    endif()

    message(STATUS "Detected Xcode-version: " ${XCODE_VERSION})

    if(${XCODE_VERSION} VERSION_LESS 4.3)
        set(CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX${OSX_SYSTEM}.sdk CACHE PATH "" FORCE)  # use guaranteed existing sdk
    else()
        # note: xcode-select path could be ambigous, cause /Applications/Xcode.app/Contents/Developer or /Applications/Xcode.app would be allowed
        # so i use a selfcomposed bundlepath here
        set(OSX_SYSROOT_PREFIX ${XCODE_BUNDLE}/Contents/Developer/Platforms/MacOSX.platform)
        message(STATUS "OSX_SYSROOT_PREFIX: " ${OSX_SYSROOT_PREFIX})
        set(OSX_DEVELOPER_PREFIX /Developer/SDKs/MacOSX${OSX_SYSTEM}.sdk) # use guaranteed existing sdk
        set(CMAKE_OSX_SYSROOT ${OSX_SYSROOT_PREFIX}/${OSX_DEVELOPER_PREFIX} CACHE PATH "" FORCE)
        if(${CMAKE_GENERATOR} MATCHES "Xcode")
            set(CMAKE_XCODE_ATTRIBUTE_SDKROOT macosx${OSX_SYSTEM}) # to silence sdk not found warning, just overrides CMAKE_OSX_SYSROOT
        endif()
    endif()

    if(OSX_SYSTEM MATCHES 10.9)
        set(CMAKE_FIND_ROOT_PATH ${CMAKE_OSX_SYSROOT}) # make sure syslibs and headers are looked up in sdk ( expecially for 10.9 openGL atm. )
    endif()

    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6" CACHE STRING "" FORCE) # 10.6 is our min. target, if you use higher sdk, weak linking happens
    endif()

    if(NOT ${CMAKE_GENERATOR} MATCHES "Xcode")
        # force CMAKE_OSX_DEPLOYMENT_TARGET for makefiles, will not work else ( cmake bug ? )
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
        add_definitions("-DMACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}")
    endif()

endif()

#-----------------------------------------------------------------------------
# Git and build information extraction
set(GIT_REFSPEC     "unknown branch")
set(GIT_REVISION    "unknown revision")
set(BUILD_DATE      "*")
set(BUILD_TIME      "*")

find_package(Git)

if(GIT_FOUND AND GIT_EXECUTABLE)
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_REVISION)
endif()

# BUILD_PLATFORM and BUILD_PLATFORM are taken from CMake but BUILD_DATE and
# BUILD_TIME are platform dependant.
if(UNIX)
    execute_process(COMMAND date "+%Y-%m-%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND date "+%H:%M:%S" OUTPUT_VARIABLE BUILD_TIME OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
if(WIN32)
    execute_process(COMMAND cmd /c date /t OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND cmd /c time /t OUTPUT_VARIABLE BUILD_TIME OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

#-----------------------------------------------------------------------------
# Common
find_package(OpenSSL REQUIRED)
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Lua52 REQUIRED)
find_package(ACE REQUIRED)

if(EXISTS ${ACE_INCLUDE_DIR}/ace/Stack_Trace.h)
    set(HAVE_ACE_STACK_TRACE_H ON)
endif()

find_package(MySQL REQUIRED)

#-----------------------------------------------------------------------------
# Extra compile flags

#-------------------------------------------------------------------------------
# Global Defines

#-----------------------------------------------------------------------------
# Dump generated headers and add custom target for version control information
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/revision.h.cmake ${CMAKE_BINARY_DIR}/revision.h)

add_custom_target("revision.h" ALL
    SOURCES "${CMAKE_BINARY_DIR}/revision.h"
)

#-----------------------------------------------------------------------------
# Build dependencies and our own applications
add_subdirectory(dep)
add_subdirectory(sql)
add_subdirectory(src)

#-----------------------------------------------------------------------------
# Print Final Configuration
if(FIRST_RUN)
    message(STATUS "")
    message(STATUS "${CMAKE_PROJECT_NAME} version ${APPLICATION_VERSION} (${CMAKE_BUILD_TYPE} mode) configured for ${CMAKE_SYSTEM_NAME}")

    message(STATUS "")
    message(STATUS "Scripting information:")

    if(WITH_MZ_SCRIPTS)
        message(STATUS "  script extensions enabled.")
    else()
        message(STATUS "  script extensions disabled.")
    endif()

    message(STATUS "")
    message(STATUS "Version information:")

    if(GIT_REFSPEC AND GIT_REVISION)
        message(STATUS "  Git branch:   ${GIT_REFSPEC}")
        message(STATUS "  Git revision: ${GIT_REVISION}")
    else()
        message(STATUS "  Git revision: not available")
    endif()

    message(STATUS "")
endif(FIRST_RUN)
