# fusiondirectory-plugins-netdisco
plugin netdisco for Fusiondirectory


This is a plugin for FusionDirectory that request a netdisco Service.

## NetDisco huhu ?
What is netdisco ? see http://netdisco.org/
Briefly It's a tool that scan your network device ( switch/router) for finding node ( everythings wich have a IP address and/or a mac Address). It provide some report ( vlan utilization, missmatch configuration).
It's very simple to use and install
Netdisco could be request trough an API ( read only) :! https://github.com/netdisco/netdisco/wiki/API

## Why a plugin ?
FD offer a simple théorical network management trough classic ip/mac declaration for several kind of component (switch / router / worksation / server) and the ipam plugin (network and vlan management)
With a plugin like netdisco. a real view is provided to FD user  like :
  - real use of each port of a network component
  - real use of vlan declared

It allow to avoid to play to "where is Waldo ?" when you want to find where is the computer with a specific macAddress, a simple request to netdisco and it's done


## todo
Netdisco Device

add flag to request or not, VLAN an POWER info per equipement DONE
add info to allow other topology like 2 row per physical unit ( very depending to netdisco discovering method) DONE - TESTING
add % of usage of utilisation port DONE
add different icons per type of port (rj45 / fiber ..) TODO
add remote port ( case interco) TODO
