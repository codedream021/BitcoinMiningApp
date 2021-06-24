#!/bin/bash
cp out/build/x64-Debug/bin/E-Pluribus-Unum-Launcher.exe ../package/
cp out/build/x64-Debug/bin/E-Pluribus-Unum.exe ../package/
git rev-list --count HEAD > ../package/version.txt
