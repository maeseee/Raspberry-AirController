#!/bin/sh

while true; do 
/home/pi/AirControlling/build/lueftung | tee /home/pi/AirControlling/log.log
cp /home/pi/AirControlling/log.log /home/pi/AirControlling/lastErrorLog.log
echo "AirConditionor has been restarted"
sleep 5s # just sleep a little
done


# Add the following to the rc.local
# /home/pi/AirControlling/StartScript.sh &

