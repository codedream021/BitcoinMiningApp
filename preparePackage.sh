#!/bin/bash
cp out/build/x64-Debug/bin/EPPRS-Launcher.exe ../package/
cp out/build/x64-Debug/bin/EPPRS.exe ../package/
git rev-list --count HEAD > ../package/version.txt
