#!/bin/bash

clear

echo " AESD HW 1 - System Information " > system_information.txt
echo "date                        : $(date)" >> system_information.txt
echo "User information            : $USER" > system_information.txt
echo "Host name                   : $(uname -n)" >> system_information.txt  
echo "Type of Operating system    : $(uname -o)" >> system_information.txt
echo "OS Distribution             : $(lsb_release -i)" >> system_information.txt
echo "version of kernel           : $(uname -r)" >> system_information.txt
echo "Kernel gcc version build    : $(cat /proc/version)" >> system_information.txt
echo "Kernel build time           : $(uname -v)" >> system_information.txt
echo "System architecture info    : $(uname -m)" >> system_information.txt
echo "Disk Space Utilization:     : $(df -mh)" >> system_information.txt
echo "File system memory          :              
$(free -m)" >> system_information.txt #Currently active users.

