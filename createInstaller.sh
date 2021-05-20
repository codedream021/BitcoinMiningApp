./moveLauncherToDebug.sh
./preparePackage.sh
cd ../package
rm ../E-Pluribus-Unum-Installer.exe
../rar/rar a -m5 -md32m -s -sfxDefault64.SFX -z../EPU/archive-comment.txt ../E-Pluribus-Unum-Installer.exe *
cd ../EPU
