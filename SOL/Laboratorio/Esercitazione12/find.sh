#!/bin/bash

if [ $# -ne 3 ]; then
  echo usa $(basename $0) x=minuti y=parola z=dirname
  exit -1
fi

x=$1
y=$2
z=$3

find "$z" -type f -mmin -$x -exec grep "$y" -l {} +