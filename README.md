# fusiondirectory-plugins-netdisco
Plugin netdisco for Fusiondirectory


This is a plugin for FusionDirectory that request a netdisco Service.

## NetDisco 
What is [netdisco](http://netdisco.org/)
Briefly It's a (magical !!) tool that scan your network device switch/router to finding a node, everythings wich have a IP address and/or a mac Address).
It will provide some report :

* vlan utilization
* mismatch configuration
* subnet statistic
...

It's very simple to use and install see [Install chapter ](https://metacpan.org/pod/App::Netdisco)
Update your netdisco instance often and subscribe to mailing list (a lot of useful informations are provided)

### how to request information from netdisco 

You can request information from Netdisco trough an [REST API in read only](https://github.com/netdisco/netdisco/wiki/API)
Pay attention about data refresh rate. Netdisco ( and then FusionDirectory) didn't show you a real time vision

## Why this plugin ?

FusionDirectory offer a simple theorical network management trough classic ip/mac declaration for several kind of component :

* workstation
* server
* switch
* router

The FusionDirectory IPAM plugin manage the base of network and vlan management

With a plugin like netdisco. a real view is provided to FD user  like :

  - real use of each port of a network component
  - real use of vlan declared
  - a simple view of network component ( useful to know wich port must be modify or not)
  - where is the device ?
  - where are my vlan ?


