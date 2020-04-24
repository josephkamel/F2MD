#!/bin/bash
clear

if [ -z "$1" ]
  then
    echo "No argument supplied"
    exit 1
fi

if ! [ "$1" = "V1" ] && ! [ "$1" = "V2" ] 
  then
    echo "Please enter V1 or V2"
    exit 1
fi

if [ "$1" = "V1" ]
  then
    echo "called: python3 MAServer.py 9980 V1"
    python3 MAServer.py 9980 V1
fi

if [ "$1" = "V2" ]
  then
    echo "called: python3 MAServer.py 9981 V2"
    python3 MAServer.py 9981 V2
fi