#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: makegame.sh directory"
fi

pushd .
cd "$1"
zip -r "../$1.zip" *
popd
