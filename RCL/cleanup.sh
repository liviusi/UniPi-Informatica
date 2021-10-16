#!/usr/bin/env bash

[[ $# -ne 1 ]] && { echo "Specify a folder to clean"; exit 1; }

rm -v $1/*.class $1/*.txt
exit 0
