if(IS_ABSOLUTE ASSETS_DIRECTORY)
    set(DESTINATION ASSETS_DIRECTORY)
else()
    set(DESTINATION "bin/${ASSETS_DIRECTORY}")
endif()


install(FILES
    Avara.ttf
    walls.png
    bat.png
    light.png
    labyrinth.ogg
    DESTINATION "${DESTINATION}")
