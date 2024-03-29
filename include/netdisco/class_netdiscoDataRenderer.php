<?php
/*
 Copyright (C) 2021-2023  Antoine Gallavardin
*/

class netdiscoDataRenderer {

private $netdiscoDataDictionnary=
array(
  'deviceInfo' =>
        array(
                'ip'               => 'string',
                'uptime'           => 'uptime',
                'ps2_status'       => 'string',
                'mac'              => 'string',
                'layers'           => 'osiLayer',
                'vtp_domain'       => 'string',
                'snmp_comm'        => 'string',
                'dns'              => 'string',
                'ps1_type'         => 'string',
                'last_discover'    => 'string',
                'serial'           => 'string',
                'location'         => 'string',
                'contact'          => 'string',
                'ps2_type'         => 'string',
                'snmp_engineid'    => 'string',
                'description'      => 'string',
                'os_ver'           => 'string',
                'name'             => 'string',
                'snmp_class'       => 'string',
                'ps1_status'       => 'string',
                'model'            => 'string',
                'os'               => 'string',
                'fan'              => 'string',
                'snmp_ver'         => 'string',
                'creation'         => 'string',
                'log'              => 'string',
                'vendor'           => 'string',
                'last_macsuck'     => 'string',
                'slots'            => 'string',
                'last_arpnip'      => 'string',
            ),

  'ips' =>
        array(
                'port'             => 'string',
                'creation'         => 'datetime',
                'alias'            => 'string',
                'ip'               => 'string',
                'dns'              => 'string',
                'subnet'           => 'string',
                'oui'               => 'oui',
                'mac'               => 'string',
                'active'            => 'bool',
                'time_last_stamp'   => 'datetime',
                'time_first_stamp'  => 'datetime',
            ),
  'sightings' =>
        array(
            'time_last_stamp'   => 'datetime',
            'time_first_stamp'  => 'datetime',
            'device'            => 'string',
            'vlan'              => 'string',
            'mac'               => 'string',
            'switch'            => 'string',
            'active'            => 'boolean',
            'port'              => 'string',
        ),
  'netbios' =>
        array (
            'time_last_stamp'   => 'datetime',
            'time_first_stamp'  => 'datetime',
            'domain'            => 'string',
            'ip'                => 'string',
            'mac'               => 'string',
            'nbuser'            => 'string',
            'nbname'            => 'string',
            'active'            => 'boolean',
            'server'            => 'boolean',
            'oui'               => 'oui',
        ),
  'wireless' =>
        array (
            'time_last_stamp'   => 'datetime',
            'time_first_stamp'  => 'datetime',
        ),
  'deviceports' =>
        array(
            'duplex'          => 'string',
            'name'            => 'string',
            'slave_of'        => 'string',
            'manual_topo'     => 'bool',
            'remote_id'       => 'string',
            'type'            => 'string',
            'vlan'            => 'int',
            'mac'             => 'string',
            'up_admin'        => 'string',
            'speed'           => 'string',
            'up'              => 'string',
            'remote_port'     => 'int',
            'creation'        => 'datetime',
            'remote_type'     => 'string',
            'is_uplink'       => 'bool',
            'pvid'            => 'int',
            'lastchange'      => 'timestamp',
            'is_master'       => 'bool',
            'speed_admin'     => 'string',
            'descr'           => 'string',
            'port'            => 'string',
            'mtu'             => 'int',
            'duplex_admin'    => 'string',
            'stp'             => 'string',
        ),
    'subnet' =>
        array(
            'active'          => 'int',
            'subnet_size'     =>'string',
            'subnet'          =>'int',
            'percent'         =>'int',
        ),
    'inventory' => array(
            'active'          => 'int',
            'vendor'          => 'string',
            'mac'             => 'string',
            'dns'             => 'string',
            'age'             => 'string',
            'time_last'       => 'string',
            'time_first'      => 'string',
            'ip'              => 'string',
        ),
    'vlan' =>
        array(
            'pcount'          => 'int',
            'model'           => 'string',
            'vendor'          => 'string',
            'dns'             => 'string',
            'os'              => 'string',
            'ip'              => 'int',
            'vlans'           => array("format" => "%s (%s)", "fields" => array('vlan','description')),
        ),
    );

    function getOutputType($info){
        return($this->netdiscoDataDictionnary[$info]);
    }

    function renderOsiLayers($layer){
      $layerRender="";
      foreach (str_split(strrev($layer)) as $key => $value){
          if ($value == 1){
            $layerRender=$layerRender."[".($key+1)."]";
          }else{
            $layerRender=$layerRender."[_]";
          }
      }
      return($layerRender);
    }


    function renderUptime($uptime){
      // uptime is in * 100 second
        $sec=$uptime / 100;
        $years=intval($sec/(3600*24*365));
        $days = intval(($sec - ($years* 3600*24*365))/(3600*24));
        $hours = intval(($sec - ($years*3600*24*365 + $days*24*3600 ))/3600);
        $minutes = intval(($sec - ($years*3600*24*365 + $days *24*3600 + $hours * 3600 ))/60);

        return( $years." "._("years")." ".$days." "._("days")." ".$hours." "._("hours")." ".$minutes." "._("minutes"));
    }


    function getRenderValue($type='', $value = ''){
        if ( $value ){
            if (is_string($type)){
                switch ($type) {
                    case "bool":
                        if ($value == "1"){
                            return("True");
                        }else{
                            return("False");
                        }
                        break;
                    case "datetime":
                        return($value);
                        break;
                    case "string":
                        return($value);
                        break;
                    case "int":
                        return($value);
                        break;
                    case "oui":
                        return($value->company);
                        break;
                    case "osiLayer":
                        return($this->renderOsiLayers($value));
                        break;
                    case "uptime":
                        return($this->renderUptime($value));
                        break;
                    default:
                        return ($value);
                }
            }else{
                // rendere type is an array
                $arrayValue=array();
                if (is_object($value)){
                    // if value is an objet
                    foreach ($type['fields'] as $key){
                        $arrayValue[] = $value->$key;
                    }
                return(vsprintf($type['format'],$arrayValue));
                }
            }
        }else{
            return("no value(s) found");
        }
    }




}
  ?>
