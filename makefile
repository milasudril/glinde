all: glinda-json-exporter.zip glinda

glinda:
	wand

glinda-json-exporter.zip: glinda-json-exporter/__init__.py
	zip glinda-json-exporter.zip glinda-json-exporter/__init__.py
