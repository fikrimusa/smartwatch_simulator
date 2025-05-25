# List your main source files
set(SRCS "main.c")

# List components you're using from main
set(COMPONENT_REQUIRES 
    heart_rate
    freertos
)

# Register the main application
idf_component_register(
    SRCS "${SRCS}"
    INCLUDE_DIRS "."
    REQUIRES "${COMPONENT_REQUIRES}"
)