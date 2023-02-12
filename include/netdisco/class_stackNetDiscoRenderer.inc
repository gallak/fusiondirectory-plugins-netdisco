<?php
/*
  This code is an addon to FusionDirectory (https://www.fusiondirectory.org/)
  Copyright (C) 2021 Antoine Gallavardin

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */



<<<<<<< HEAD
class stackNetdiscoRenderv2 {
    //$stack=array();
    //$type="";
    //$orderedStack=array();
    
    function __construct($stack,$type){
        $this->stack = $stack;
        $this->type = $type;
        $this->orderedStack =array();
=======
    // on convertit le tableau de port netdisco en un tableau representant l'aspect graphique
//    function convertNetdiscoArrayToGraphicalArray($deviceInfo){
	function convertNetdiscoArrayToGraphicalArray($orderedStack,$type){
        // on range les propriete des ports dans un tableau multidimensionnel 
/*        $orderedStack=array();
        foreach ($deviceInfo as $record){
            $portXY=explode(".",$record->port);
            $swNumber=$portXY[0];
            $portNumber=$portXY[1];
	    $orderedStack[$swNumber - 1][$portNumber -1]['summary'] = $record;
	
        }

	var_dump($orderedStack[0][21]['summary']); */
        // on a un tableau 1 ligne par swicth
        // un swicth peut avoir plusieur rangee de port

        // on fera en sorte que ce soit parametrable par la suite
//        $nb_row = "2"; // il y a 2 lines de port par swicth.
        $typeArray=explode("-",$type);
	$nb_row = $typeArray[0];
        $firstPort= $typeArray[1];
        $graphicalStack=array();

        foreach ($orderedStack as $indice => $unit){
            $nbPorts = count($unit);
	    $nbPortsPerLine = $nbPorts / $nb_row;

            foreach($unit as $position => $port){
                switch ($firstPort) {
                    case "leftup" :
                        $realY = ($position % $nb_row ) + 1;
                        break;
                    case "leftdown" :
                        $realY = $nb_row - ($position % $nb_row ) + 1;
                        break;
                }
                //$realY = ($position % $nb_row ) + 1;
                $realX = floor($position / $nb_row);
                $graphicalStack[$indice][$realY][$realX]=$port;
		ksort($graphicalStack[$indice][$realY]);
            }
            ksort($graphicalStack[$indice]);
        }
       ksort($graphicalStack);
    return($graphicalStack);
>>>>>>> 77f48af402685bfc65cab6c65f39ec932cddb917
    }


    // type of switch repartition
    //$typeArray=explode("-",$type);
	//$nb_row = $typeArray[0];
    //$firstPort= $typeArray[1];

    function renumberingStack(){

        $typeArray=explode("-",$this->type);
        // get number of line per module
        $nb_row = $typeArray[0];
        // get first port
        $firstPort= $typeArray[1];
        // stack => swicth > module > X > Y
        $finalOrderedStack=array();
        // ordering physical stack by position
        foreach($this->stack as $value){
          $this->orderedStack[$value->fd['position']['switch']]
                             [$value->fd['position']['module']]
                             [$value->fd['position']['port']]=$value;
        }
        //var_dump($this->orderedStack);
        foreach ($this->orderedStack as $switch){
            foreach ($switch as $module){
                // nb de port
                $nbPorts=count($module);
                // nb port per line
                $nbPortsPerLine = $nbPorts / $nb_row;
                foreach ($module as $port) {
                    $position = $port->fd['position']['port'];
                    switch ($firstPort) {
                        case "leftup" :
                            $realY = (($position -1 )% $nb_row );
                            $realX = intdiv(($position - (($position -1) % $nb_row)), $nb_row);
                            break;
                        case "leftdown" :
                            $realY = $nb_row - ($position % $nb_row );
                            $realX = (int)floor($position / $nb_row);
                            break;
                    }
                    $finalOrderedStack[$port->fd['position']['switch']]
                                      [$port->fd['position']['module']]
                                      [$realY][$realX] = $port;
                    
                    //$this->orderedStack[$port->fd['position']['switch']]
                    //                   [$port->fd['position']['module']]
                    //                   [$port->fd['position']['port']]->fd['XY']=array("X"=>$realX,"Y" => $realY);
                    
                }
                
                foreach ($finalOrderedStack[$port->fd['position']['switch']][$port->fd['position']['module']] as $k => $linev ){
                    ksort($finalOrderedStack[$port->fd['position']['switch']][$port->fd['position']['module']][$k]);
                }
                ksort($finalOrderedStack[$port->fd['position']['switch']][$port->fd['position']['module']]);
               

            }

        // trier les tableau !!!            
        }


        return($finalOrderedStack);
    }


}
