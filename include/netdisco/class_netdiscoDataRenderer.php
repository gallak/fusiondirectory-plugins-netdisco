<?php

class netdiscoDataRenderer {

private $netdiscoDataDictionnary=
array(
  'ips' =>
        array(
                'port'             => array('label'    => 'Port',      'type' => 'string'),
                'creation'         => array('label'    => 'Creation',     'type' => 'datetime'),
                'alias'            => array('label'    => 'Alias',         'type' => 'string'),
                'ip'               => array('label'    => 'Ip Address',    'type' => 'string'),
                'dns'              => array('label'    => 'FQDN',           'type' => 'string'),
                'subnet'           => array('label'    => 'Subnet',    'type' => 'string'),
            ),

  'node' =>
        array('ips' =>
            array(
                'time_last_stamp'   => array('label'    => 'Last seen',      'type' => 'datetime'),
                'time_first_stamp'  => array('label'    => 'First seen',     'type' => 'datetime'),
                'active'            => array('label'    => 'Active',         'type' => 'bool'),
                'ip'                => array('label'    => 'Ip Address',    'type' => 'string'),
                'mac'               => array('label'    => 'Mac Address',    'type' => 'string'),
                'dns'               => array('label'    => 'FQDN',           'type' => 'string'),
                'oui'               => array('label'    => 'Constructor',    'type' => 'oui'),
            ),
          'sightings' =>
            array(
                'time_last_stamp'   => array('label'    => 'Last seen',      'type' => 'datetime'),
                'time_first_stamp'  => array('label'    => 'First seen',     'type' => 'datetime'),
                'device'            => array('label'    => 'Device Name',    'type' => 'string'),
                'vlan'              => array('label'    => 'Vlan',           'type' => 'string'),
                'mac'               => array('label'    => 'Mac Address',    'type' => 'string'),
                'switch'            => array('label'    => 'Swicth Address', 'type' => 'string'),
                'active'            => array('label'    => 'Active',         'type' => 'boolean'),
		'port'		    => array('label'	=> 'port',	     'type' => 'string'),
            ),
        'netbios' =>
            array (
                'time_last_stamp'   => array('label'    => 'Last seen',      'type' => 'datetime'),
                'time_first_stamp'  => array('label'    => 'First seen',     'type' => 'datetime'),
                'domain'            => array('label'    => 'Domain',         'type' => 'string'),
                'ip'                => array('label'    => 'Ip address',     'type' => 'string'),
                'mac'               => array('label'    => 'Mac Address',    'type' => 'string'),
                'nbuser'            => array('label'    => 'Netbios User',   'type' => 'string'),
                'nbname'            => array('label'    => 'Netbios name',   'type' => 'string'),
                'active'            => array('label'    => 'Active',         'type' => 'boolean'),
                'server'            => array('label'    => 'Server',         'type' => 'boolean'),
                'oui'               => array('label'    => 'Constructor',    'type' => 'oui'),
            ),
        'wireless' =>
            array (
                'time_last_stamp'   => array('label'    => 'Last seen',      'type' => 'datetime'),
                'time_first_stamp'  => array('label'    => 'First seen',     'type' => 'datetime'),
            ),
        ),
    'deviceports' =>
        array(
            'duplex'          => array('label'    => 'Flow',           'type' => 'string'),
            'name'            => array('label'    => 'Name',           'type' => 'string'),
            'slave_of'        => array('label'    => 'slave_of',       'type' => 'string'),
            'manual_topo'     => array('label'    => 'Manual topology','type' => 'bool'),
            'remote_id'       => array('label'    => 'Remote Id',      'type' => 'string'),
            'type'            => array('label'    => 'Type',           'type' => 'string'),
            'vlan'            => array('label'    => 'Vlan',           'type' => 'int'),
            'mac'             => array('label'    => 'Mac Address',    'type' => 'string'),
            'up_admin'        => array('label'    => 'Up Admin',       'type' => 'string'),
            'speed'           => array('label'    => 'Speed',          'type' => 'string'),
            'up'              => array('label'    => 'Up',             'type' => 'string'),
            'remote_port'     => array('label'    => 'Remote port',    'type' => 'int'),
            'creation'        => array('label'    => 'Creation date',  'type' => 'datetime'),
            'remote_type'     => array('label'    => 'Remote type',    'type' => 'string'),
            'is_uplink'       => array('label'    => 'Is uplink',      'type' => 'bool'),
            'pvid'            => array('label'    => 'Pvid',           'type' => 'int'),
            'lastchange'      => array('label'    => 'Last Change',    'type' => 'timestamp'),
            'is_master'       => array('label'    => 'Is master',      'type' => 'bool'),
            'speed_admin'     => array('label'    => 'Speed Admin',    'type' => 'string'),
            'descr'           => array('label'    => 'Description',    'type' => 'string'),
            'port'            => array('label'    => 'Port',           'type' => 'string'),
            'mtu'             => array('label'    => 'MTU',            'type' => 'int'),
            'duplex_admin'    => array('label'    => 'Duplex admin',   'type' => 'string'),
            'stp'             => array('label'    => 'STP',            'type' => 'string'),
        ),
	'subnet' =>
        array(
            'active'          =>array('label'     => 'active',         'type' => 'int'),
            'subnet_size'     =>array('label'     => 'Subnet size',    'type' => 'string'),
            'subnet'          =>array('label'     => 'subnet',         'type' => 'int'),
            'percent'         =>array('label'     => 'percent',        'type' => 'int'),
        ),
	'inventory' => array(
            'active'          =>array('label'     => 'active',         'type' => 'int'),
            'vendor'          =>array('label'     => 'Subnet size',    'type' => 'string'),
            'mac'             =>array('label'     => 'subnet',         'type' => 'string'),
            'dns'             =>array('label'     => 'percent',        'type' => 'string'),
	    'age'             =>array('label'     => 'age',            'type' => 'string'),
	    'time_last'       =>array('label'     => 'Time last',      'type' => 'string'),
	    'time_first'      =>array('label'     => 'Time first',     'type' => 'string'),
	    'ip'              =>array('label'     => 'Ip addres',      'type' => 'string'),
	),
        'vlan' =>
        array(
            'pcount'          =>array('label'     => 'portNumber',         'type' => 'int'),
            'model'           =>array('label'     => 'Model',    'type' => 'string'),
            'vendor'           =>array('label'     => 'Vendor',    'type' => 'string'),
            'dns'           =>array('label'     => 'DNS',    'type' => 'string'),
            'os'              =>array('label'     => 'os',         'type' => 'string'),
            'ip'              =>array('label'     => 'ip',        'type' => 'int'),
	    'vlans'		=> array('label'	=> 'Vlan',	'type' =>  array("format" => "%s (%s)", "fields" => array('vlan','description'))),
        ),
    );

    function getDictionnary($type){
        return($this->netdiscoDataDictionnary[$type]);
    }

    function getLabel($object=array()){
        return $this->netdiscoDataDictionnary[$object]['label'];
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
            return("no value found");
        }
    }




}
  ?>
