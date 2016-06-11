all: glinde-json-exporter.zip test.zip debug release doc

glinde: build.py config_in.json
	./build.py

release: glinde-json-exporter.zip test.zip build.py config_in.json
	./build.py release

debug: glinde-json-exporter.zip test.zip build.py config_in.json
	./build.py debug

glinde-json-exporter.zip: glinde-json-exporter/__init__.py
	zip glinde-json-exporter.zip glinde-json-exporter/__init__.py

test.zip: test/*
	./makegame.sh test

doc:
	./makedoc.sh

clean:
	rm -f test.zip
	rm -rf __*
	rm -f glinde-json-exporter.zip
