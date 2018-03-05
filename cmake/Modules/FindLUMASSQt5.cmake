# *******************************************************
# FindLUMASSQt5.cmake
# based on cmake 2.8.9
#
# finds main installation path of custom installed Qt5 (Qt-Project webiste download)
# ****************************************************

# CUSTQT5_FOUND

# ================================================
# define library names we're going to look for later
# ================================================

if(WIN32)
    set(QT5_CORE_LIB "lib/Qt5Core.lib")
    set(QT5SQLite_PLUGIN_LIBRARY "qsqlite.lib")
    set(QT5_PATHS
		C:/qt
		C:/
		C:/Qt
		C:/opt
		C:/opt/qt
		C:/opt/Qt
	)
    set(QT5_PATH_SUFFIXES
            Qt5.3.1/qtbase
            Qt5.3.1/5.3/qtbase
            Qt5.5.0/5.5/qtbase
            Qt5.5.0/qtbase
            5.3.1/qtbase
            5.4.2/qtbase
            5.4.2/5.4/qtbase
            5.3.1/5.3/qtbase
            5.5.0/5.5/qtbase
            5.5.0/qtbase
            5.5.1/qtbase
            5.6.0/qtbase
            5.6.1/qtbase
            5.6.1-1/qtbase
            5.6.2/qtbase
            5.6.3/qtbase
            5.7.0/qtbase
            5.7.1/qtbase
            5.8.0/qtbase
            5.9.0/qtbase
            5.9.1/qtbase
            5.9.2/qtbase
            5.9.3/qtbase
            5.10.0/qtbase
            5.11.0/qtbase
    )
else()
    set(QT5_CORE_LIB "libQt5Core.so")
    set(QT5SQLite_PLUGIN_LIBRARY "libqsqlite.so")
    set(QT5_PATHS
            /opt
            /opt/qt
            /opt/Qt
            /usr
            /usr/qt
            /usr/local
            /usr/lib
            /usr/lib/x86_64-linux-gnu
	)
    set(QT5_PATH_SUFFIXES
            x86_64-linux-gnu
            Qt5.0.0/5.0.0/gcc
            Qt5.0.0/5.0.0/gcc_64
            Qt5.0.1/5.0.1/gcc
            Qt5.0.1/5.0.1/gcc_64
            Qt5.0.2/5.0.2/gcc
            Qt5.0.2/5.0.2/gcc_64
            Qt5.1.0/5.1.0/gcc
            Qt5.1.0/5.1.0/gcc_64
            Qt5.1.1/5.1.1/gcc
            Qt5.1.1/5.1.1/gcc_64
            Qt5.2.0/5.2.0/gcc
            Qt5.2.0/5.2.0/gcc_64
            Qt5.2.1/5.2.1/gcc
            Qt5.2.1/5.2.1/gcc_64
            Qt5.3.0/5.3.0/gcc
            Qt5.3.0/5.3.0/gcc_64
            Qt5.3.1/5.3.1/gcc
            Qt5.3.1/5.3.1/gcc_64
            Qt5.4.2/5.4.2/gcc
            Qt5.4.2/5.4.2/gcc_64
            Qt5.5.0/5.5.0/gcc
            Qt5.5.0/5.5.0/gcc_64
            Qt5.5.1/5.5.1/gcc
            Qt5.5.1/5.5.1/gcc_64
            Qt5.6.0/5.6.0/gcc
            Qt5.6.0/5.6.0/gcc_64
            Qt5.6.1/5.6.1/gcc_64
            Qt5.6.1-1/5.6.1-1/gcc_64
            Qt5.6.2/5.6.2/gcc_64
            Qt5.6.3/5.6.3/gcc_64
            Qt5.7.0/5.7.0/gcc_64
            Qt5.7.1/5.7.1/gcc_64
            Qt5.8.0/5.8.0/gcc_64
            Qt5.9.0/5.9.0/gcc_64
            Qt5.9.1/5.9.1/gcc_64
            Qt5.9.2/5.9.2/gcc_64
            Qt5.9.3/5.9.3/gcc_64
            Qt5.10.0/5.10.0/gcc_64
            Qt5.11.0/5.11.0/gcc_64
            qt/5.3/gcc
            qt/5.3/gcc_64
            5.3/gcc
            5.3/gcc_64
            5.4/gcc_64
            5.4/gcc
            5.5/gcc_64
            5.5/gcc
            qt5
    )
endif()

# ==================================================
# brute force
# ==================================================
FIND_PATH(QT5_PREFIX_PATH ${QT5_CORE_LIB}
    PATH_SUFFIXES
        ${QT5_PATH_SUFFIXES}
    PATHS
        ${QT5_PATHS}
)
message(STATUS "QT5_PREFIX_PATH: ${QT5_PREFIX_PATH} ")


# ============================================================================
# determine Qt5 modules' library and include directories
# ============================================================================
if(NOT QT5_PREFIX_PATH)
    SET(QT5_PREFIX_PATH "${QT5_PREFIX_PATH}" CACHE PATH "Qt5 install dir")
else()

    SET(QT5_PREFIX_PATH ${QT5_PREFIX_PATH}/../ CACHE PATH "Qt5 install dir")
    SET(QT5_COMP_LIST
        "Qt5Core"
        "Qt5Widgets"
        "Qt5Xml"
        "Qt5Gui"
        "Qt5Concurrent"
        "Qt5Sql"
        #"Qt5Declarative"
        #"Qt5Qml"
        #"Qt5Quick"
    )
	
	foreach(QT5COMP ${QT5_COMP_LIST})
	    find_package(${QT5COMP})
	    if(${QT5COMP}_FOUND)
			if(WIN32)
				LIST(APPEND QT5_LIB_LIST "${QT5COMP}.lib")
			else(WIN32)
				LIST(APPEND QT5_LIB_LIST "${QT5COMP}_LIBRARIES")
		    endif(WIN32)
                    LIST(APPEND QT5_INCLUDE_DIRS ${${QT5COMP}_INCLUDE_DIRS})

            #			string(COMPARE EQUAL "${QT5COMP}" Qt5Sql _cmp)
            #            if (_cmp)
            #                list(LENGTH ${QT5COMP}_PLUGINS numplugs)
            #                if (${numplugs})
            #                    list(GET Qt5Sql_PLUGINS 0 plugin)
            #                    get_target_property(_loc ${plugin} LOCATION)
            #                    get_filename_component(_loc_path ${_loc} DIRECTORY)
            #                    message("Plugin ${plugin} is at location ${_loc_path}")
            #                    set(QT5SQL_PLUGINS_DIR ${_loc_path} CACHE FILEPATH
            #                        "Path to Qt5's qsqlite.<so | dll | lib> plugin library" FORCE)
            #                endif()
            #			endif()
	    endif(${QT5COMP}_FOUND)
	endforeach(QT5COMP)
	list(REMOVE_DUPLICATES QT5_INCLUDE_DIRS)
	
	message(STATUS "Qt5 libraries: ${QT5_LIB_LIST}")
	message(STATUS "Qt5 INCLUDE_DIRS: ${QT5_INCLUDE_DIRS}")
	
	# extract the link directories
	get_target_property(FULLLIBNAME ${Qt5Core_LIBRARIES} LOCATION)
	get_filename_component(LIBPATH ${FULLLIBNAME} PATH)
	SET(QT5_LINK_DIRS ${LIBPATH})
	foreach(LOOPLIB ${QT5_LIB_LIST})
            get_target_property(FULLLIBNAME ${LOOPLIB} LOCATION)
            get_filename_component(LIBPATH ${FULLLIBNAME} PATH)
	    list(APPEND QT5_LINK_DIRS ${LIBPATH})
	endforeach(LOOPLIB)
	list(REMOVE_DUPLICATES QT5_LINK_DIRS)
	
	# just from experience
	IF(WIN32)
            set(WINLIBDIR ${QT5_PREFIX_PATH}/lib)
            list(APPEND QT5_LINK_DIRS ${WINLIBDIR})
	ENDIF(WIN32)
	
	message(STATUS "Qt5 link directories: ${QT5_LINK_DIRS}")
endif()

# ==========================================================
# now, we determine the qt5 version
# ==========================================================

# let's find the path to Qt5ConfigVersion
FIND_PATH(QT5_CONFIG_PATH Qt5ConfigVersion.cmake
        PATH_SUFFIXES
           cmake/Qt5
           lib/cmake/Qt5
        PATHS
            ${QT5_PREFIX_PATH}
            ${QT5_PREFIX_PATH}/..
        NO_DEFAULT_PATH
)


message(STATUS "QT5_CONFIG_PATH: ${QT5_CONFIG_PATH}")

if(QT5_CONFIG_PATH-NOTFOUND)
    message(STATUS "Couldn't find Qt5ConfigVersion.cmake!")
    set(QT5_VERSION_STRING "5.?.?")
else()
    include(${QT5_CONFIG_PATH}/Qt5ConfigVersion.cmake)
    set(QT5_VERSION_STRING "${PACKAGE_VERSION}")
    message(STATUS "Qt5 version: ${QT5_VERSION_STRING}")
endif()


# ==========================================================
# find the private header dir for Qt5Core
# ==========================================================
foreach(QT5INCLDIR ${QT5_INCLUDE_DIRS})
    #message(STATUS "TESTING ${QT5INCLDIR}/${QT5_VERSION_STRING}/QtCore" )
    FIND_PATH(QT5CORE_PRIVATE_DIR private/qitemselectionmodel_p.h
        PATH_SUFFIXES
            ${QT5_VERSION_STRING}/QtCore
        PATHS
            ${QT5INCLDIR}
    )

endforeach()

if(NOT QT5CORE_PRIVATE_DIR)
    message(STATUS "couldn't find QT5CORE_PRIVATE_DIR!")
else()
    message(STATUS "QT5CORE_PRIVATE_DIR=${QT5CORE_PRIVATE_DIR}")
endif()

# ==========================================================
# find the private header dir for Qt5Sql
# ==========================================================
foreach(QT5SQLPRIVDIR ${QT5_INCLUDE_DIRS})
    FIND_PATH(NMQT5SQL_PRIVATE_DIR private/qsqldriver_p.h
        PATH_SUFFIXES
            ${QT5_VERSION_STRING}/QtSql
        PATHS
            ${QT5SQLPRIVDIR}
    )

endforeach()

if(NOT NMQT5SQL_PRIVATE_DIR)
    message(STATUS "couldn't find NMQT5SQL_PRIVATE_DIR!")
else()
    message(STATUS "NMQT5SQL_PRIVATE_DIR=${NMQT5SQL_PRIVATE_DIR}")
endif()






