# Required for FreeRTOS APIs
set(COMPONENT_REQUIRES freertos)

# Source files to compile
set(COMPONENT_SRCS "step_counter.c")

# Public header directory
set(COMPONENT_ADD_INCLUDEDIRS "include")

# Register the component
register_component()