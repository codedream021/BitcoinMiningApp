./moveLauncherToDebug.sh
./preparePackage.sh
cd ../package
rm ../E-Pluribus-Unum-Installer.exe
../rar/rar a -m5 -md32m -s -sfxDefault64.SFX -z../EPU/archive-comment.txt ../E-Pluribus-Unum-Installer.exe *
rm ../update.dat
../rar/rar a -m5 -md32m -s -z../EPU/archive-comment.txt ../update.dat E-Pluribus-Unum.exe
cd ../EPU
cp ../E-Pluribus-Unum-Installer.exe ../binaries/
cp ../update.dat ../updates/
cp version.txt ../updates/
