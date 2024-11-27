#!/bin/bash

I=0

while [ $I -lt $3 ]
do
	$1 $2 add $(echo $(( I + RANDOM )) | md5sum | head -c 20)
	I=$(( I + 1 ))
done
