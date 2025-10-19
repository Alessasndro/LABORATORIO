cmake_minimum_required(VERSION 3.26)
project(LABORATORIO LANGUAGES CXX)
project(LABORATORIO)

set(CMAKE_CXX_STANDARD 26)

# Imposta il percorso di build per le DLL
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Scarica e compila SFML
include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x  # Usa il branch stabile 2.6.x
)

FetchContent_MakeAvailable(SFML)

# Crea il tuo eseguibile
add_executable(${PROJECT_NAME}
        main.cpp
        Personaggio.cpp
        Personaggio.h
        Mappa.cpp
        Mappa.h
        ostacoli.cpp
        ostacoli.h
        Nemico.cpp
        Nemico.h
        NavigationGrind.cpp
        NavigationGrind.h)

# Collega SFML al tuo progetto
target_link_libraries(${PROJECT_NAME}
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network
)

# Copia automaticamente le DLL dopo la build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:sfml-network>
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)