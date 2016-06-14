#!/bin/bash

make clean
rm -rf jedliad1/
mkdir jedliad1/
cp -rf examples/ jedliad1/
cp -rf src/ jedliad1/
cp Doxyfile jedliad1/
cp Makefile jedliad1/
cp zadani.txt jedliad1/
cp prohlaseni.txt jedliad1/
zip -r jedliad1.zip jedliad1

