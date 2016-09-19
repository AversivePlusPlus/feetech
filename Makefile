PACKAGE_NAME = feetech

.PHONY: arduino_package

arduino_package: build/${PACKAGE_NAME}.zip

build/${PACKAGE_NAME}.zip: build/arduino/${PACKAGE_NAME}.h
	cd build/arduino && zip ${PACKAGE_NAME}.zip . -r && mv ${PACKAGE_NAME}.zip ..

build/arduino/${PACKAGE_NAME}.h: build/arduino
	cp -r include/* $<
	cp pro/arduino/${PACKAGE_NAME}.h $<
	cp -r pro/arduino/examples $<

build/arduino: build
	mkdir $@

build:
	mkdir $@
