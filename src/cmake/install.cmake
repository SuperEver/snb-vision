# Used for pack inc, bin, lib to appropriate path

set ( INSTALL_BIN bin CACHE PATH "Where to install binaries to." )
set ( INSTALL_LIB lib CACHE PATH "Where to install libraries to." )
set ( INSTALL_INC include CACHE PATH "Where to install headers to." )
set ( INSTALL_SDK sdk CACHE PATH "Where to install sdk(inc, so) to." )

# Parser macro
macro ( parse_arguments prefix arg_names option_names)
    set ( DEFAULT_ARGS )
    foreach ( arg_name ${arg_names} )
        set ( ${prefix}_${arg_name} )
    endforeach ()
    foreach ( option ${option_names} )
        set ( ${prefix}_${option} FALSE )
    endforeach ()

    set ( current_arg_name DEFAULT_ARGS )
    set ( current_arg_list )
    foreach ( arg ${ARGN} )
        set ( larg_names ${arg_names} )
        list ( FIND larg_names "${arg}" is_arg_name )
        if ( is_arg_name GREATER -1 )
            set ( ${prefix}_${current_arg_name} ${current_arg_list} )
            set ( current_arg_name ${arg} )
            set ( current_arg_list )
        else ()
            set ( loption_names ${option_names} )
            list ( FIND loption_names "${arg}" is_option )
            if ( is_option GREATER -1 )
                set ( ${prefix}_${arg} TRUE )
            else ()
                set ( current_arg_list ${current_arg_list} ${arg} )
            endif ()
        endif ()
    endforeach ()
    set ( ${prefix}_${current_arg_name} ${current_arg_list} )
endmacro ()

# helper function for various install_* functions, for PATTERN/REGEX args.
macro ( _complete_install_args )
    if ( NOT("${_ARG_PATTERN}" STREQUAL "") )
        set ( _ARG_PATTERN PATTERN ${_ARG_PATTERN} )
    endif ()
    if ( NOT("${_ARG_REGEX}" STREQUAL "") )
        set ( _ARG_REGEX REGEX ${_ARG_REGEX} )
    endif ()
endmacro ()

macro ( install_header )
    parse_arguments ( _ARG "INTO;PATTERN;REGEX" "" ${ARGN} )
    _complete_install_args()
    foreach ( _file ${_ARG_DEFAULT_ARGS} )
        if ( IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_file}" )
            install ( DIRECTORY ${_file} DESTINATION ${INSTALL_INC}/${_ARG_INTO}
                    COMPONENT Header ${_ARG_PATTERN} ${_ARG_REGEX} )
        else ()
            install ( FILES ${_file} DESTINATION ${INSTALL_INC}/${_ARG_INTO}
                    COMPONENT Header )
        endif ()
    endforeach()
endmacro ()

macro ( install_library )
    foreach ( _file ${ARGN} )
        install ( TARGETS ${_file}
                RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT Runtime
                LIBRARY DESTINATION ${INSTALL_LIB} COMPONENT Runtime
                ARCHIVE DESTINATION ${INSTALL_LIB} COMPONENT Library )
    endforeach()
endmacro ()

macro ( install_executable )
    foreach ( _file ${ARGN} )
        install ( TARGETS ${_file} RUNTIME DESTINATION ${INSTALL_BIN}
                COMPONENT Runtime )
    endforeach()
endmacro ()

macro ( install_sdk )
    foreach ( _file ${ARGN} )
        install ( TARGETS ${_file}
                RUNTIME DESTINATION ${INSTALL_SDK} COMPONENT Runtime)
    endforeach()
endmacro ()