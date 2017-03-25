#!/bin/sh

while true; do 
/home/pi/AirControlling/build/lueftung
cp log.log lastErrorLog.log
sleep 5s # just sleep a little
done


# Add the following to the rc.local
# /home/pi/AirControlling/StartScript.sh &

