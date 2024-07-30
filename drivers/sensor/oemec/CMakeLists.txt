zephyr_library()
zephyr_library_sources(oemec.c)

# This shouldn't be needed if we send this driver upstream.
zephyr_include_directories(.)
