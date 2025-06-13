set(SRCS "main.c")

set(COMPONENT_REQUIRES 
    # Project-specific components
    cli
    common
    datetime
    heart_rate
    memory
    step_counter
    
    # ESP-IDF system components
    freertos    # FreeRTOS depencencies
    console     # Console I/O
    driver      # Hardware drivers
)

idf_component_register(
    SRCS "${SRCS}"
    INCLUDE_DIRS "."
    REQUIRES "${COMPONENT_REQUIRES}"
)