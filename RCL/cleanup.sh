#!/usr/bin/env bash

[[ $# -ne 1 ]] && { echo "Specify a folder to clean"; exit 1; }

rm $1/*.class
exit 0