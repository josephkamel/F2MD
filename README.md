
# Framework For Misbehavior Detection (F2MD)

This project aims to recreate within a simulation all the elements that form the chain of misbehavior detection. 

The scientific publication is available on [ResearchGate](https://www.researchgate.net/publication/340350056_Simulation_Framework_for_Misbehavior_Detection_in_Vehicular_Networks). If you find this code useful in your research, please consider citing :

    @ARTICLE{9056489,
        author={J. {Kamel} and M. R. {Ansari} and J. {Petit} and A. {Kaiser} and I. {Ben Jemaa} and P. {Urien}},
        title={Simulation Framework for Misbehavior Detection in Vehicular Networks},
        journal={IEEE Transactions on Vehicular Technology},
        year={2020}
    }
    
## Supported network technologies:
* ITS-G5 (IEEE 802.11p)
* C-V2X (3GPP PC5 Mode 4)

## Features
* Basic Plausibility Checks on Received Beacons (mdChecks)
* Node Level Plausibility Investigation (mdApplications)
* Real Time Machine Learning for Plausibility Investigation (HTTP to a Python Server: PyMLServer-Local)
* Real Time Detection Status Output (mdStats)
* Support for Multiple Reporting Mechanisms (mdReport)
* Support for Global Reports Collection and Investigation (HTTP to a Python Server: MAServer-Global)
* Some Basic Psudonym Change Policies (mdPCPolicies)
* Some Local and Global Misbehavior Attacks Implementation (mdAttacks)

## Installation
1. Install Sumo 1.5.0
2. Install OMNeT++ 5.6.1
3. Setup the *veins-f2md* version of Veins  (Follow the [Veins Tutorial](https://veins.car2x.org/tutorial/))
3. Setup the *simulte-f2md* version of simulte  (Follow the [OpenCV2X Tutorial](http://www.cs.ucc.ie/cv2x/pages/documentation/documentation.html))

Alternatively, contact the author for a preinstalled **Instant F2MD** virtual machine.

## Project Diagram

 ![alt text](https://github.com/josephkamel/F2MD/blob/master/F2MD-Diagram-V2.jpg)


