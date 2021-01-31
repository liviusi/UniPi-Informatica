#!/usr/bin/env bash
> out.txt
javac *.java
java -ea Main >> out.txt
find . -name "*.class" -type f -delete