# See https://github.com/bxparks/EpoxyDuino for documentation about this
# Makefile to compile and run Arduino programs natively on Linux or MacOS.

APP_NAME := HelloEpoxyEepromAvr
ARDUINO_LIBS := EpoxyEepromAvr AceUtils AceCRC AceRoutine AceCommon
MORE_CLEAN := more_clean
include ../../../../EpoxyDuino.mk

more_clean:
	rm -f epoxyeepromdata
