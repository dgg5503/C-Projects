#!/bin/sh
# install links and copies for students
echo installing supplied binaries and program starter files

ln -s /home/course/csci243/pub/projects/firewall/lib*  .
ln -s /home/course/csci243/pub/projects/firewall/*.h  .
ln -s /home/course/csci243/pub/projects/firewall/fwSim .
ln -s /home/course/csci243/pub/projects/firewall/pktAnalyzer .
ln -s /home/course/csci243/pub/projects/firewall/referenceFirewall .
ln -s /home/course/csci243/pub/projects/firewall/packets.1 .
ln -s /home/course/csci243/pub/projects/firewall/valgrind.supp .

# copy the config and .c files
cp /home/course/csci243/pub/projects/firewall/c*.txt  .
cp /home/course/csci243/pub/projects/firewall/f*.c  .
cp /home/course/csci243/pub/projects/firewall/script1.txt  .
cp /home/course/csci243/pub/projects/firewall/header.mak .
cp /home/course/csci243/pub/projects/firewall/script1.txt .

