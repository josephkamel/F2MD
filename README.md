
# Framework For Misbehavior Detection (F2MD)

This project aims to recreate within a simulation all the elements that form the chain of misbehavior detection.

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
1. Install Sumo 1.0.1
2. Install OMNeT++ 5.4.1
3. Setup the *veins-f2md* version of Veins  ( Follow the [Veins Tutorial](https://veins.car2x.org/tutorial/) )

Alternatively, contact the author for a preinstalled ** Instant F2MD ** virtual machine.

## Project Diagram

 ![alt text](https://github.com/josephkamel/F2MD/blob/master/F2MD-Diagram-V2.jpg)


