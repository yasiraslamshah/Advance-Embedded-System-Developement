#!/bin/bash
#problem 6
#AESD 2019
#author : Yasir

echo "System Information:  $USER">>info.txt

echo "Operating System Type/Brand:">>info.txt  
uname>>info.txt
echo>>info.txt


echo "Operating System Distribution/Version:">>info.txt
cat /etc/*release>>info.txt
echo>>info.txt


echo>>info.txt
echo "System Architecture Information:">>info.txt
echo>>info.txt
lscpu>>info.txt
echo>>info.txt

echo>>info.txt
echo "Information on File system Memory:">>info.txt
echo>>info.txt
free -m>>info.txt
cat /proc/meminfo>>info.txt

echo>>info.txt
echo "Kernel Version:">>info.txt
uname -r>>info.txt
echo>>info.txt


echo>>info.txt
echo "Kernel gcc version build:">>info.txt

echo>>info.txt
cat /proc/version>>info.txt
echo>>info.txt


echo>>info.txt
echo "kernel build time:">>info.txt
echo>>info.txt

uname -v>>info.txt
#uname -m>>info.txt
echo>>info.txt

