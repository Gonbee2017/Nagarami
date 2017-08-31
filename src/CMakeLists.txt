SET(CMAKE_LEGACY_CYGWIN_WIN32 0)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_EXE_LINKER_FLAGS "-static")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin
)
SET(NAGARAMI_SOURCE_FILES
    apiwrapper.cpp
    component.cpp
    execute.cpp
    helper.cpp
    main.cpp
    resource.rc
    systemobject.cpp
    window.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES Release)
    ADD_EXECUTABLE(Nagarami WIN32
        ${NAGARAMI_SOURCE_FILES}
    )
    TARGET_LINK_LIBRARIES(Nagarami
        WinMM
    )
ELSEIF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_EXECUTABLE(Nagarami WIN32
        ${NAGARAMI_SOURCE_FILES}
    )
    TARGET_LINK_LIBRARIES(Nagarami
        WinMM
    )
ENDIF()
