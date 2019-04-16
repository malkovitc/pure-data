include(PdExternal)
include(../../ext/src/ceammc_pd.cmake)

add_definitions(-DPD)
include_directories(${PROJECT_SOURCE_DIR}/src)
# include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib/cicm/Sources)
include_directories(HoaLibrary-PD/ThirdParty/CicmWrapper/Sources/)

# compatibility with original branch
FILE(GLOB HOA_WRAPPER_ORIGINAL_C HoaLibrary-PD/ThirdParty/CicmWrapper/Sources/*.c)
FILE(GLOB HOA_WRAPPER_ORIGINAL_HEADERS HoaLibrary-PD/ThirdParty/CicmWrapper/Sources/*.h)

set( HOA_HEADERS
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Decoder.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Defs.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Encoder.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Exchanger.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Harmonics.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Hoa.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Hrir.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/HrirIrc1002C2D.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/HrirIrc1002C3D.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Math.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Meter.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Optim.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Planewaves.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Processor.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Projector.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Recomposer.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Rotate.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Scope.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Signal.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Source.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Tools.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Vector.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Voronoi.hpp
HoaLibrary-PD/ThirdParty/HoaLibrary/Sources/Wider.hpp
hoa.library.hpp
)

set (HOA_SOURCES_STATIC
HoaLibrary-PD/Sources/hoa.decoder_tilde.cpp
HoaLibrary-PD/Sources/hoa.encoder_tilde.cpp
HoaLibrary-PD/Sources/hoa.exchanger_tilde.cpp
HoaLibrary-PD/Sources/hoa.io.cpp
HoaLibrary-PD/Sources/hoa.map_gui.cpp
HoaLibrary-PD/Sources/hoa.map_tilde.cpp
HoaLibrary-PD/Sources/hoa.meter_gui_tilde.cpp
HoaLibrary-PD/Sources/hoa.optim_tilde.cpp
HoaLibrary-PD/Sources/hoa.process_tilde.cpp
HoaLibrary-PD/Sources/hoa.projector_tilde.cpp
HoaLibrary-PD/Sources/hoa.recomposer_tilde.cpp
HoaLibrary-PD/Sources/hoa.rotate_tilde.cpp
HoaLibrary-PD/Sources/hoa.scope_gui_tilde.cpp
HoaLibrary-PD/Sources/hoa.space_gui.cpp
HoaLibrary-PD/Sources/hoa.tools.cpp
HoaLibrary-PD/Sources/hoa.wider_tilde.cpp

)

SET_SOURCE_FILES_PROPERTIES( ${HOA_SOURCES_STATIC} PROPERTIES LANGUAGE CXX )

set (HOA_SOURCES
HoaLibrary-PD/hoa.library.cpp
)

set (HOA_HEADERS
HoaLibrary-PD/hoa.library.hpp
)

find_library( ACCELERATE_LIBRARY Accelerate)
message( STATUS "ACCELERATE_LIBRARY is ${ACCELERATE_LIBRARY}")

set( HOA_DEPENDENCIES
${ACCELERATE_LIBRARY}
)

# compatibility
add_library(hoa_compatible_wrapper STATIC ${HOA_WRAPPER_ORIGINAL_C} ${HOA_WRAPPER_ORIGINAL_HEADERS})
set(HOA_HEADERS ${HOA_HEADERS} ${HOA_WRAPPER_ORIGINAL_HEADERS})
add_library(hoa_static STATIC ${HOA_SOURCES_STATIC} ${HOA_HEADERS})

pd_add_external(NAME hoa
        FILES ${HOA_SOURCES} ${HOA_HEADERS}
        INTERNAL True
        LIBRARY hoa
        LINK hoa_static hoa_compatible_wrapper
        )

if(UNIX)
    set_target_properties(hoa PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
