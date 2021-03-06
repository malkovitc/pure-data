set(SYNTH_SOURCES)
set(SYNTH_STK_SOURCES
    stksynth.cpp
    synth_fgrain.cpp
    synth_shakers.cpp
    synth_wurley.cpp
    synth_bthree.cpp
    synth_rhodey.cpp)

macro(ceammc_synth_obj name)
    list(APPEND SYNTH_SOURCES "synth_${name}.cpp")
    ceammc_faust_gen_obj(synth ${name} JSON)
endmacro()

ceammc_synth_obj(birds)
ceammc_synth_obj(church_bell)
ceammc_synth_obj(dubdub)
ceammc_synth_obj(eguitar)
ceammc_synth_obj(ks)
ceammc_synth_obj(marimba)
ceammc_synth_obj(risset_arp)
ceammc_synth_obj(risset_tone)

list(APPEND SYNTH_SOURCES "synth_dx7.cpp")
ceammc_faust_gen_obj(synth dx7 JSON OCPP)

include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/stk/stk/include)

add_library(ceammc_synth STATIC mod_synth.cpp ${SYNTH_SOURCES} ${SYNTH_STK_SOURCES})
target_link_libraries(ceammc_synth stk)
set_target_properties(ceammc_synth PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
