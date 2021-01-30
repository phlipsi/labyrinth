add_custom_target(assets)
set(ASSETS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Directory for game assets")

function(convert_xcf_to_png input target crop)
    get_filename_component(name "${input}" NAME_WE)
    set(output "${name}.png")
    add_custom_command(OUTPUT ${output}
        COMMAND convert -crop ${crop} -layers merge "${CMAKE_CURRENT_SOURCE_DIR}/${input}" "${CMAKE_CURRENT_BINARY_DIR}/${output}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${input}"
        COMMENT "Converting ${input} to ${output}")
    add_custom_target(${name} DEPENDS ${output})
    add_dependencies(assets ${name})
endfunction()

convert_xcf_to_png(walls.xcf assets "1024x64+0+0")
