#!/bin/bash

RED="\033[1;31m"
GREEN="\033[1;32m"
NOCOLOR="\033[0m"

portsinuse=$(netstat -tln | tail -n +3 | awk '{ print $4 }') 

if [[ $portsinuse == *"9999"* ]]
    then 
        echo -e "\n${GREEN}Sumo TraCI Server is running. ${NOCOLOR}\n"
    else
        echo -e "\n${RED}Sumo TraCI Server not running. Please run $./launchSumoTraciServer.sh ${NOCOLOR}\n"
        exit 1
fi

PS3='Please chose a scenario: '
options_net=("IRTSystemXScenario" "LuSTNanoScenario" "LuSTMiniScenario" "LuSTScenario" "UlmScenario")
select opt_net in "${options_net[@]}"
do
    case $opt_net in
        "IRTSystemXScenario")
            echo
            echo "you chose choice $REPLY which is $opt_net"
            break
            ;;
        "LuSTNanoScenario")
            echo
            echo "you chose choice $REPLY which is $opt_net"
            break
            ;;
        "LuSTMiniScenario")
            echo
            echo "you chose choice $REPLY which is $opt_net"
            break
            ;;
        "LuSTScenario")
            echo
            echo "you chose choice $REPLY which is $opt_net"
            break
            ;;
        "UlmScenario")
            echo
            echo "you chose choice $REPLY which is $opt_net"
            break
            ;;
        *) echo
            echo "invalid option $REPLY"
            exit 1            
            ;;
    esac
done

echo
PS3='Please chose a technology: '
options_tech=("ITS-G5 (IEEE 802.11p)" "C-V2X (3GPP PC5 Mode 4)" )
select opt_tech in "${options_tech[@]}"
do
    case $opt_tech in
        "ITS-G5 (IEEE 802.11p)")
            echo
            echo "you chose choice $REPLY which is $opt_tech"
            cd "./veins-f2md/f2md-networks/"
            break
            ;;
        "C-V2X (3GPP PC5 Mode 4)")
            echo
            opt_net="${opt_net}Mode4"
            echo "you chose choice $REPLY which is $opt_tech"
            cd "./simulte-f2md/f2md-networks/"
            break
            ;;
        *) echo "invalid option $REPLY"
            exit 1            
            ;;
    esac
done
echo

PS3='Please chose a simulation environnement: '
options_sim=("Cmdenv" "Qtenv" )
select opt_sim in "${options_sim[@]}"
do
    case $opt_sim in
        "Cmdenv")
            echo
            echo "you chose choice $REPLY which is $opt_sim"
            break
            ;;
        "Qtenv")
            echo
            echo "you chose choice $REPLY which is $opt_sim"
            break
            ;;
        *) echo 
            echo "invalid option $REPLY"
            exit 1            
            ;;
    esac
done

cd $opt_net 
./run -u $opt_sim





