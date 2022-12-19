
## SSRMS

add_library(SSRMS SHARED
	SSRMS.cpp
	LEESystem.cpp
)

add_dependencies(SSRMS
	${OrbiterTgt}
	Orbitersdk
)

set_target_properties(SSRMS
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_PLUGIN_DIR}
	OUTPUT_NAME SSRMSD
)

target_include_directories(SSRMS PUBLIC
	${ORBITER_BINARY_SDK_DIR}/include
	../libUltra/include
)

target_link_libraries(SSRMS
	${ORBITER_SDK_LIB}
	Ultra
)

install(TARGETS SSRMS
#	RUNTIME
	DESTINATION ${ORBITER_INSTALL_MODULE_DIR}
)
