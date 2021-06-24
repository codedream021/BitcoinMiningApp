#!/bin/bash
./moveLauncherToDebug.sh
./preparePackage.sh
git rev-list --count HEAD > version.txt
cd ../package
rm ../E-Pluribus-Unum-Installer.exe
../rar/rar a -m5 -md32m -s -sfxDefault64.SFX -z../EPU/archive-comment.txt ../E-Pluribus-Unum-Installer.exe *
rm ../update.dat
../rar/rar a -m5 -md32m -s -z../EPU/archive-comment.txt ../update.dat E-Pluribus-Unum.exe
../rar/rar a -m5 -md32m -s -z../EPU/archive-comment.txt ../update.dat version.txt
cp version.txt ../updates/
cd ../EPU
cp ../E-Pluribus-Unum-Installer.exe ../binaries/
cp ../update.dat ../updates/

