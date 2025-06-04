# Required for FreeRTOS APIs
set(COMPONENT_REQUIRES 
    freertos
    datetime
    nvs_flash
    esp_common)

# Source files to compile
set(COMPONENT_SRCS "datetime.c")

# Public header directory
set(COMPONENT_ADD_INCLUDEDIRS "include")

# Register the component
register_component()