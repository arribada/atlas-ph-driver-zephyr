
# This makefile makes it easy to develop using picoprobe as a debugger

continue:
	west build -b rpi_pico -- -DOPENOCD=/usr/bin/openocd -DOPENOCD_DEFAULT_PATH=/usr/share/openocd/scripts -DRPI_PICO_DEBUG_ADAPTER=cmsis-dap

all:
	west build --pristine -b rpi_pico -- -DOPENOCD=/usr/bin/openocd -DOPENOCD_DEFAULT_PATH=/usr/share/openocd/scripts -DRPI_PICO_DEBUG_ADAPTER=cmsis-dap

flash:
	west flash

fm:
	west flash
	minicom
clean:
	rm -rf build
