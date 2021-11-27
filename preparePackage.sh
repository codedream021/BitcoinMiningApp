#!/bin/bash
cp out/build/x64-Debug/bin/envirosoft-Launcher.exe ../package/
cp out/build/x64-Debug/bin/envirosoft-v1.exe ../package/
git rev-list --count HEAD > ../package/version.txt
