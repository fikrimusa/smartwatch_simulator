# Required for FreeRTOS APIs
set(COMPONENT_REQUIRES 
    freertos
    driver
    step_counter)

# Source files to compile
set(COMPONENT_SRCS "cli.c")

# Public header directory
set(COMPONENT_ADD_INCLUDEDIRS "include")

# Register the component
register_component()