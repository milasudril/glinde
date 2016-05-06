all: glinie-json-exporter.zip glinie test.zip

glinie:
	wand profile[release]

glinie-json-exporter.zip: glinie-json-exporter/__init__.py
	zip glinie-json-exporter.zip glinie-json-exporter/__init__.py

test.zip: test
	./makegame.sh
