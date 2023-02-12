# fusiondirectory-plugins-netdisco
plugin netdisco for Fusiondirectory


This is a plugin for FusionDirectory that request a netdisco Service.

## NetDisco 
What is [netdisco](http://netdisco.org/)
Briefly It's a tool that scan your network device switch/router to finding a node, everythings wich have a IP address and/or a mac Address).
It will provide some report :

* vlan utilization
* mismatch configuration
* subnet statistic
...

It's very simple to use and install

### how to request information from netdisco 

You can request information from Netdisco trough an [REST API in read only](https://github.com/netdisco/netdisco/wiki/API)

## Why a plugin ?

FusionDirectory offer a simple théorical network management trough classic ip/mac declaration for several kind of component :

* workstation
* server
* switch
* router

The FusionDirectory ipam plugin manage the base of network and vlan management

With a plugin like netdisco. a real view is provided to FD user  like :

  - real use of each port of a network component
  - real use of vlan declared

This avoid to play "where is Waldo ?" when you want to find where is the computer with a specific macAddress, a simple request to netdisco and it's done


