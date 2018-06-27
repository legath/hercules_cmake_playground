
# Нужен cmake >=3.9
CMAKE_MINIMUM_REQUIRED(VERSION 3.9)

include(base.cmake)
PROJECT(hercules_template)



# Дерективы препроцессора, нужные для библиотеки STM.
ADD_DEFINITIONS(
# Тут нужно подставить свой чип (MD, MD_VL и т.д)

)


##################################################################################


file(GLOB_RECURSE _tmp_list ${PROJECT_SOURCE_DIR}/hal/source/*.c)

set(SRC_HAL_FILES)
foreach(f ${_tmp_list})
    if(NOT f MATCHES "sys_main")
        list(APPEND SRC_HAL_FILES ${f})
    endif()
endforeach()
##################################################################################
# Исходники проекта
file(GLOB_RECURSE SRC_C_FILES ${PROJECT_SOURCE_DIR}/app/*.c)
file(GLOB_RECURSE SRC_CXX_FILES ${PROJECT_SOURCE_DIR}/app/*.cpp)
file(GLOB_RECURSE SRC_HEADERS ${PROJECT_SOURCE_DIR}/app/*.h)

##################################################################################
include_directories(${PROJECT_SOURCE_DIR}/src/)
SET(PROJECT_SOURCES
  ${SRC_HAL_FILES}
  ${SRC_C_FILES}
  ${SRC_CXX_FILES}
  ${SRC_HEADERS}
)


# Флаги компилятора для разных типов сборки.
SET(COMPILE_DEFINITIONS_DEBUG -O0 -g3 -DDEBUG)
SET(COMPILE_DEFINITIONS_RELEASE -Os)


SET(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/hal/source/sys_link.ld ${CMAKE_EXE_LINKER_FLAGS}")

# Добавляем пути поиска заголовочных файлов
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/.
  ${PROJECT_SOURCE_DIR}/hal/include
)

# Собираем исходники пректа, модули, и т.д. в elf
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES} ${MOD_SOURCES} ${STARTUP_SOURCE} ${SERVICE_SOURCES})

# Конвертируем elf в hex и bin

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
