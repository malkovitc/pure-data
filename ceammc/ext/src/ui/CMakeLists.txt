if(WIN32)
    add_definitions("-D_WINDOWS=1")
endif()

set(UILINK_TCL ui_link.tcl)

add_custom_target(uilink_tcl
    DEPENDS ${UILINK_TCL}
    SOURCES ${UILINK_TCL} ui_link_tcl.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tcl_to_c.sh ${UILINK_TCL} uilink_tcl_init > ui_link_tcl.h
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

macro(ceammc_ui_extension name)
    list(APPEND UI_SOURCES "ui_${name}.cpp")
endmacro()

ceammc_ui_extension(arrayview)
ceammc_ui_extension(bang)
ceammc_ui_extension(colorpanel)
ceammc_ui_extension(display)
ceammc_ui_extension(dsp)
ceammc_ui_extension(env)
ceammc_ui_extension(gain)
ceammc_ui_extension(gain2)
ceammc_ui_extension(icon)
ceammc_ui_extension(incdec)
ceammc_ui_extension(keyboard)
ceammc_ui_extension(knob)
ceammc_ui_extension(label)
ceammc_ui_extension(link)
ceammc_ui_extension(matrix)
ceammc_ui_extension(menu)
ceammc_ui_extension(meter)
ceammc_ui_extension(mouse_filter)
ceammc_ui_extension(mouse_route)
ceammc_ui_extension(number)
ceammc_ui_extension(number_tilde)
ceammc_ui_extension(plot_tilde)
ceammc_ui_extension(polar)
ceammc_ui_extension(preset)
ceammc_ui_extension(radio)
ceammc_ui_extension(rslider)
ceammc_ui_extension(scope)
ceammc_ui_extension(single_value)
ceammc_ui_extension(slider)
ceammc_ui_extension(slider2d)
ceammc_ui_extension(sliders)
ceammc_ui_extension(spectroscope)
ceammc_ui_extension(tab)
ceammc_ui_extension(toggle)

add_library(ceammc_ui STATIC mod_ui.cpp ${UI_SOURCES})

target_link_libraries(ceammc_ui ceammc_env)
