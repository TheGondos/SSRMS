add_library(Ultra OBJECT
        Orbitersdk/libUltra/KOST/kost_elements.cpp
        Orbitersdk/libUltra/KOST/kost_linalg.cpp
        Orbitersdk/libUltra/KOST/kost_math.cpp
        Orbitersdk/libUltra/KOST/kost_propagate.cpp
        Orbitersdk/libUltra/KOST/kost_shape.cpp
        Orbitersdk/libUltra/src/AerodynamicLookupTables.cpp
        Orbitersdk/libUltra/src/Averager.cpp
        Orbitersdk/libUltra/src/BasicValve.cpp
        Orbitersdk/libUltra/src/BIU.cpp
        Orbitersdk/libUltra/src/BusController.cpp
        Orbitersdk/libUltra/src/BusTerminal.cpp
        Orbitersdk/libUltra/src/DiscInPort.cpp
        Orbitersdk/libUltra/src/DiscOutPort.cpp
        Orbitersdk/libUltra/src/DiscreteBundle.cpp
        Orbitersdk/libUltra/src/DiscreteBundleManager.cpp
        Orbitersdk/libUltra/src/DiscretePort.cpp
        Orbitersdk/libUltra/src/HydraulicActuatedValve.cpp
        Orbitersdk/libUltra/src/LambertBurnTargeting.cpp
        Orbitersdk/libUltra/src/MMUTapeImage.cpp
        Orbitersdk/libUltra/src/PEG4Targeting.cpp
        Orbitersdk/libUltra/src/PIDControl.cpp
        Orbitersdk/libUltra/src/PressureActuatedValve.cpp
        Orbitersdk/libUltra/src/Sensor.cpp
        Orbitersdk/libUltra/src/SolenoidValve.cpp
        Orbitersdk/libUltra/src/StateVectorPropagator.cpp
        Orbitersdk/libUltra/src/UltraMath.cpp
        Orbitersdk/libUltra/src/ValveManager.cpp

)

add_dependencies(Ultra
	${OrbiterTgt}
	Orbitersdk
)

target_include_directories(Ultra PUBLIC
	${ORBITER_BINARY_SDK_DIR}/include
    Orbitersdk/libUltra/include
    Orbitersdk/libUltra/KOST
)


add_subdirectory(Orbitersdk/SSRMS)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Config
DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Doc
DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Meshes
DESTINATION ${ORBITER_INSTALL_ROOT_DIR}
)
