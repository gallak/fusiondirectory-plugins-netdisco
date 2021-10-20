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


class stackNetDiscoRenderer {

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
    }


    function drawStack($deviceInfo){
        return($this->drawTableStack($deviceInfo));
    }


}
