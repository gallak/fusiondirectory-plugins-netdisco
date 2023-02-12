<?php
/*
 Copyright (C) 2021-2023  Antoine Gallavardin
*/


class stackNetdiscoRenderv2 {
    //$stack=array();
    //$type="";
    //$orderedStack=array();
    
    function __construct($stack,$type){
        $this->stack = $stack;
        $this->type = $type;
        $this->orderedStack =array();
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
