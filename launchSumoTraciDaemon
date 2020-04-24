#!/bin/bash

RED="\033[1;31m"
GREEN="\033[1;32m"
NOCOLOR="\033[0m"

if (echo >/dev/tcp/localhost/9999) &>/dev/null
    then 
        echo -e "\n${RED}Sumo TraCI Server already running. ${NOCOLOR}\n"
        exit 1
fi

PS3='Please chose a configuration: '
options_sim=("With GUI" "Without GUI" )
select opt_sim in "${options_sim[@]}"
do
    case $opt_sim in
        "With GUI")
            echo "you chose choice $REPLY which is $opt_sim"
            ./veins-f2md/sumo-launchd.py  -vv -c sumo-gui
            break
            ;;
        "Without GUI")
            echo "you chose choice $REPLY which is $opt_sim"
            ./veins-f2md/sumo-launchd.py  -vv -c sumo
            break
            ;;
        *) echo "invalid option $REPLY"
            exit 1            
            ;;
    esac
done