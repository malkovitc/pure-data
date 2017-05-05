#!/bin/sh

DB="../ext/doc/ceammc.db"

if [ $# -ne 1 ]
then
    echo "Using: $0 NAME"
    exit 1
fi

pd_obj2img -f png -p 2 -x "$DB" $1 obj/$1.png

echo "image for [$1] saved to obj/$1.png"
