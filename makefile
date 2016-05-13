all: glinde-json-exporter.zip glinde test.zip

glinde:
	wand profile[release]

glinde-json-exporter.zip: glinde-json-exporter/__init__.py
	zip glinde-json-exporter.zip glinde-json-exporter/__init__.py

test.zip: test/*
	./makegame.sh test
