# $Id: CMakeLists.txt 2012-05-05 18:42:09Z fgriberi $

cmake_minimum_required( VERSION 2.6 )

project( lib_fideo CXX )

add_definitions ( -Wall -Wextra -pedantic -ansi )

option( weff "Compile with warnings violations of the Effective C++ book." OFF )

if ( weff )
    message ( STATUS "Compiling with warnings violations of the Effective C++ book." )
    add_definitions (-Weffc++)
    endif ( weff )

    find_file ( MILI mili/mili.h )
    if (NOT MILI )
        message ( FATAL_ERROR "You must install MiLi: mili.googlecode.com" )
        endif (NOT MILI )

        find_file ( BIOPP biopp/biopp.h )
        if (NOT BIOPP )
            message ( FATAL_ERROR "You must install Biopp: biopp.googlecode.com" )
            endif (NOT BIOPP )

            if ( CMAKE_BUILD_TYPE MATCHES Debug )
                message ( STATUS "Compiling with debbug support." )
                add_definitions( -ggdb3 )
                endif ( CMAKE_BUILD_TYPE MATCHES Debug )

                include_directories( . fideo src )

                set ( lib_srcs     src/RNABackendProxy.cpp
                      src/RNAFold.cpp
                      src/SecStructure.cpp
                      src/RNABackendsFactory.cpp
                    )

                set ( fideo_header fideo.h )

                set ( fideo_private_headers    fideo/IFold.h
                      fideo/RNAFold.h
                      fideo/RNABackendsConfig.h
                      fideo/RNABackendProxy.h
                      fideo/rna_backends_exceptions.h
                      fideo/rna_backends_types.h
                      fideo/RNABackendsFactory.h
                    )

                install ( FILES  ${fideo_header} DESTINATION include/fideo )
                install ( FILES  ${fideo_private_headers}  DESTINATION include/fideo/.private )

                add_library ( fideo SHARED ${lib_srcs} )

                set ( CMAKE_INSTALL_PREFIX /usr/local )

                install ( TARGETS fideo LIBRARY DESTINATION lib )
