all: glinde glinde-json-exporter.zip test.zip doc

glinde:
	maike --targets=glinde

glinde-json-exporter.zip: glinde-json-exporter/__init__.py
	zip glinde-json-exporter.zip glinde-json-exporter/__init__.py

test.zip:  $(wildcard test/**/*)
	./makegame.sh test

doc:
	./makedoc.sh

clean:
	rm -f test.zip
	rm -rf __*
	rm -f glinde-json-exporter.zip
