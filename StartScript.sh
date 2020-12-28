#!/bin/sh

while true; do 
/home/pi/Raspberry-AirController/build/lueftung | tee /home/pi/Raspberry-AirController/log.log
cp /home/pi/Raspberry-AirController/log.log /home/pi/Raspberry-AirController/lastErrorLog.log
echo "AirConditionor has been restarted"
sleep 5s # just sleep a little
done


# Add the following to the /etc/rc.local
# /home/pi/Raspberry-AirController/StartScript.sh &
