APP=check_python

build:
	gcc check_python.c -o $(APP)

run: build
	./$(APP)

clean:
	rm -f $(APP)
	rm -f *.ipk
	rm -f /tmp/python_ver.log

package: build
	mkdir -p package/usr/bin
	cp $(APP) package/usr/bin/
	tar -czf check-python_1.0_mock.ipk package
