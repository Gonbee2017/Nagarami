SET(CMAKE_LEGACY_CYGWIN_WIN32 0)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin
)
SET(NAGARAMI_SOURCE_FILES
    apiwrapper.cpp
    component.cpp
    execute.cpp
    helper.cpp
    systemobject.cpp
    window.cpp
)
SET(NAGARAMI_TEST_SOURCE_FILES
    helpertest.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES Release)
    SET(CMAKE_EXE_LINKER_FLAGS "-static")
    ADD_EXECUTABLE(Nagarami WIN32
        ${NAGARAMI_SOURCE_FILES}
        resource.rc
        main.cpp
    )
    TARGET_LINK_LIBRARIES(Nagarami WinMM)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_EXECUTABLE(nagaramitest
        ${NAGARAMI_SOURCE_FILES}
        ${NAGARAMI_TEST_SOURCE_FILES}
        maintest.cpp
        nagaramitest.cpp
    )
    TARGET_LINK_LIBRARIES(nagaramitest
        CppUTest
        CppUTestExt
        WinMM
    )
ENDIF()
