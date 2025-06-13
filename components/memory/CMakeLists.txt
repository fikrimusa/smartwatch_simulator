# Required for FreeRTOS APIs
set(COMPONENT_REQUIRES freertos)

# Source files to compile
set(COMPONENT_SRCS "heap_tracker.c")

# Public header directory
set(COMPONENT_ADD_INCLUDEDIRS "include" "../common/include")

# Register the component
register_component()