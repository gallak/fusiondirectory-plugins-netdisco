<?php

class netdiscoDataRenderer {

private $netdiscoDataDictionnary=
array(
  'deviceInfo' =>
        array(
                'ip'               => 'string',
                'uptime'           => 'string',
                'ps2_status'       => 'string',
                'mac'              => 'string',
                'layers'           => 'string',
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
            ),
  'node' =>
        array('ips' =>
            array(
                'time_last_stamp'   => 'datetime',
                'time_first_stamp'  => 'datetime',
                'active'            => 'bool',
                'ip'                => 'string',
                'mac'               => 'string',
                'dns'               => 'string',
                'oui'               => 'oui',
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
