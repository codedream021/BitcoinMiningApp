#!/bin/bash
./moveLauncherToDebug.sh
./preparePackage.sh
git rev-list --count HEAD > version.txt
cd ../package
rm ../envirosoft-Installer.exe
../rar/rar a -m5 -md32m -s -sfxDefault64.SFX -z../EPU/archive-comment.txt ../envirosoft-Installer.exe *
rm ../update.dat
../rar/rar a -m5 -md32m -s -z../EPU/archive-comment.txt ../update.dat envirosoft-v1.exe
../rar/rar a -m5 -md32m -s -z../EPU/archive-comment.txt ../update.dat version.txt
cp version.txt ../updates/
cd ../EPU
cp ../envirosoft-v1.exe ../binaries/
cp ../update.dat ../updates/

