cmake_minimum_required(VERSION 2.8)

project(kaqtoos)

find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtNetwork REQUIRED)

set(QT_USE_QTMAIN TRUE)
include(${QT_USE_FILE})

string(REPLACE "/include" "" QT_DIR "${QT_INCLUDE_DIR}")

if(UNIX AND NOT APPLE)
	find_library(LINUX_KQOAUTH kqoauth /usr/lib)
	find_file(LINUX_INCLUDE_KQOAUTH QtKOAuth /usr/include)
endif()

set(TARGET_NAME KacQToos)

set(SOURCE_FILES
	main.cpp
	kaqtoos.cpp
	productitem.cpp
	downloadmanager.cpp
)

set(HEADER_FILES_WITH_MOC
	kaqtoos.h
	productitem.h
	downloadmanager.h
)

set(HEADER_FILES
	${HEADER_FILES_WITH_MOC}
)

QT4_WRAP_CPP(MOC_SOURCE_FILES ${HEADER_FILES_WITH_MOC})

if (UNIX)
	if (APPLE)
		list(APPEND INCLUDES ${QT_LIBRARY_DIR}/kqoauth.framework/Headers)
	else()
		list(APPEND INCLUDES ${LINUX_INCLUDE_KQOAUTH})
	endif()
endif()

include_directories(
	${INCLUDES}
)

add_executable(${TARGET_NAME} WIN32 MACOSX_BUNDLE
  ${HEADER_FILES}
  ${SOURCE_FILES}
  ${MOC_SOURCE_FILES}
)

if(APPLE)
  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND install_name_tool ARGS -change kqoauth.framework/Versions/0/kqoauth ${QT_LIBRARY_DIR}/kqoauth.framework/Versions/0/kqoauth ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.app/Contents/MacOS/${TARGET_NAME}
	)
endif()

if(UNIX)
	if(APPLE)
		set(LIB_KQOAUTH ${QT_LIBRARY_DIR}/kqoauth.framework)
	else()
		set(LIB_KQOAUTH ${LINUX_KQOAUTH})
	endif()
endif()

target_link_libraries(${TARGET_NAME}
  ${QT_LIBRARIES}
	${LIB_KQOAUTH}
)
