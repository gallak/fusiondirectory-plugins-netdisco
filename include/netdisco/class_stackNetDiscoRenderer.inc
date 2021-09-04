<?php

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
