<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2016-2017  FusionDirectory

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


require_once("netdisco/class_netdisco.php");
require_once("netdisco/class_netdiscoDataRenderer.php");


class netdiscoDevice extends simplePlugin
{

  public $netdiscoHandler;

  protected $displayHeader  = TRUE;
  protected $loaded = TRUE;
  public $physicalDef = "/ethernetCsmacd/";
  public $interfacesDef = [];

  var $objectclasses = array('fdNetdiscoDevice');

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Netdisco Device'),
      'plDescription' => _('Netdisco Device '),
      'plTitle'       => _('Netdisco Device'),
      'plPriority'    => 30,
      'plObjectClass'     => ['fdNetdiscoDevice'],
      'plObjectType'  => ['component' ],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }



  public function getMainInfo () {
    global $config;
    $this->connectNetdiscoServer();
    // get Info From device
    $smarty = get_smarty();
    $deviceInfo = $this->netdiscoHandler->getDeviceInfo($this->attrs['ipHostNumber'][0]);
    if (! is_null($deviceInfo)){
        $this->fdNetdiscoInfo=displayList($deviceInfo,array( 'deviceInfo' => array('description','serial','layers','uptime','mac','last_discover','last_macsuck','last_arpnip','creation')));
        $this->fdNetdiscoInfoIp=displayTable(array('ips' => $this->netdiscoHandler->getDeviceIpsDetails($this->attrs['ipHostNumber'][0])),array( 'ips'=> array('ip','alias','subnet','port','dns')));
    }
  }

  // translate topolgy keep only physical port
  // could add more information 
  //  - if slave_of is not null -> MLT
  public function translateTopology($object)
  {
    global $config;
    $ldap = $config->get_ldap_link();

    // to put in globle config
    //$physicalDef="/ethernetCsmacd/";
//    $avayaDef="/^[0-9]+\.[0-9]+$/";  // <swicth>.<port>
//    $ciscoDef="/^[a-zA-Z]{2}[0-9]+\/[0-9]+\/[0-9]+$/";  // Te<swictch>/<module>/<port>

    $physicalDef = $this->physicalDef;
    $interfacesDef = $this->interfacesDef;
    
    //var_dump($physicalDef);
    //var_dump($interfacesDef);
    $fdDeviceTopology=array();
    foreach ($object as $record){
      $record = (array)$record;
      if (preg_match($physicalDef,$record['type'])){
        // clean All alphabetical
        $cleanPort=preg_replace("/[a-zA-Z]/",'',$record['port']);
        // loop for testing  intergface
        foreach ($interfacesDef as $oneDef){
            //var_dump($oneDef);
            $oneDefArray=preg_split("/;/",$oneDef);
            if (preg_match($oneDefArray[1],$cleanPort)){
              
              $position=preg_split($oneDefArray[2],$cleanPort);
              if (isset($position[2])){
                // cisco / H3C / dell style like <unit>/<module>/<port>
                $record['fd']['position']=array("switch"=>$position[0], "module"=> $position[1], "port" =>$position[2]);
              }else{
                // exception nortel style like <unit>.<port>
                $record['fd']['position']=array("switch"=>$position[0], "module"=> 0, "port" =>$position[1]);
              }
              break;
            }

        }

        // fetch addictionnal informations
        if ( isset($this->fdNetdiscoDeviceShowVlanMembership) && $this->fdNetdiscoDeviceShowVlanMembership == 'TRUE'){
          $netdiscoVlan= $this->netdiscoHandler->getDevicePortVlans($this->attrs['ipHostNumber'][0],$record['port']);
          $vlans = "";
          foreach ($netdiscoVlan as $value){
              $vlans = $vlans." ".$value->description." (".$value->vlan.")";
          }
          $record['fd']['vlans']=$vlans;
        }
        // fetch Power Consumption
        if ( isset($this->fdNetdiscoDeviceShowPower) && $this->fdNetdiscoDeviceShowPower == 'TRUE'){
          $record['fd']['power']=$this->netdiscoHandler->getDevicePortPower($this->attrs['ipHostNumber'][0],$record['port'])->power;
          
        }

        // fetch Remote
        if (isset($record['remote_id']) && isset($record['remote_port']) ){
          
          //var_dump($record['remote_id']);
          $ldap->cd($config->current['BASE']);
          $fields=array('cn','description');
          $ldap->search('(&(objectClass=ipHost)(macAddress='.$record['remote_id'].'))', $fields );
          $attrs = $ldap->fetch();
          if (isset($attrs['cn'])){
            foreach ($fields as $k ){
              $record['fd']['remote'][$k]=$attrs[$k][0];
            }
          $record['fd']['remote']['link']=htmlspecialchars('main.php?plug='.$_GET['plug'].'&reset=1&act=listing_edit&dn='.$attrs['dn']);
          $record['fd']['remote']['port']=$attrs['cn'][0]." - ".$record['remote_port'];
          }
        }

        // is a trunk ?
        if (isset($record['slave_of'])){
          $record['fd']['type']="TRUNK";
        }
        
        $fdDeviceTopology[]=(object)$record;
      }
    }

    return($fdDeviceTopology);
  }


  // get topology from netdisco and redraw it
  public function refreshAndGetTopology ()
  {
      $this->getMainInfo();
      //get all info from netdisc
      $deviceTopology = $this->netdiscoHandler->getDevicePorts($this->attrs['ipHostNumber'][0]);
      // translate topology ( physical position / and other information)
      $deviceTopology=$this->translateTopology($deviceTopology);
            
      $test=new stackNetdiscoRenderv2($deviceTopology,$this->fdNetdiscoDeviceType);
      $this->fdNetdiscoTopology=(array)$test->renumberingStack();
  }




  // increase information per port ( power / vlan )
  private function setupDeviceTopology($deviceTopology){
        $orderedStack=array();
        foreach ($deviceTopology as $record){
          
          $swNumber=$record->fd['position']['switch'];
          $portNumber=$record->fd['position']['port'];

            $orderedStack[$swNumber - 1][$portNumber -1]['summary'] = $record;
            if ( isset($this->fdNetdiscoDeviceShowVlanMembership) && $this->fdNetdiscoDeviceShowVlanMembership == 'TRUE'){
                $netdiscoVlan= $this->netdiscoHandler->getDevicePortVlans($this->attrs['ipHostNumber'][0],$record->port);
                $vlans = "";
                foreach ($netdiscoVlan as $value){
                    $vlans = $vlans." ".$value->description." (".$value->vlan.")";
                }
                $orderedStack[$swNumber - 1][$portNumber -1]['vlan'] = $vlans;
            }
            if ( isset($this->fdNetdiscoDeviceShowPower) && $this->fdNetdiscoDeviceShowPower == 'TRUE'){
                $orderedStack[$swNumber - 1][$portNumber -1]['power'] = $this->netdiscoHandler->getDevicePortPower($this->attrs['ipHostNumber'][0],$record->port);
            }
        }
   //var_dump($orderedStack);
    return($orderedStack);
  }


  static function getAttributesInfo (): array
  {
    return [
      'settings' => [
        'name'  => _('Device Settings'),
        'attrs' =>[
             new netdiscoServerSelectAttribute (_('Netdisco Service'),_('Name of server where the client is connected to'),
                        'fdNetdiscoDeviceServiceDn',TRUE),
             new SelectAttribute(_('Physical topology'),_('How device port are organized'),
                 'fdNetdiscoDeviceType',TRUE,
                 array('2-leftup','2-leftdown','4-leftup','4-leftdown','1-leftup'),'2-leftup',
                 array('2 lines of port, first is on the up left','2 lines of port, first is on down left','4 lines of ports, first is on the left up','4 lines of ports, first is on the left up','1 lines of port, first is on the left'),
                 ),
             new BooleanAttribute(_('Show Power status'), _('Show Power information per port'),
                 'fdNetdiscoDeviceShowPower',FALSE,FALSE),
             new BooleanAttribute(_('Show Vlan Membership'), _('Show Vlan which this ports is belong to'),
                 'fdNetdiscoDeviceShowVlanMembership',FALSE,FALSE),
             new ButtonAttribute(_('Fetch Informations'), _('Fetch informations'),
              'fdNetdiscoUpdate', _('Fetch'),'refreshAndGetTopology'
           ),

            ],
      ],
      'detailsInfo' => [
        'class' => ['fullwidth'],
        'name'  => _('Device Details'),
        'attrs' => [
            new FakeAttribute('fdNetdiscoInfo'),
            ],
          'template' => get_template_path('netdiscoDeviceInfo.tpl', TRUE, dirname(__FILE__)),
          ],

      'detailsIps' => [
        'class' => ['fullwidth'],
        'name'  => _('Device Ips'),
        'attrs' => [
            new FakeAttribute('fdNetdiscoInfoIp'),
            ],
          'template' => get_template_path('netdiscoDeviceInfoIp.tpl', TRUE, dirname(__FILE__)),
          ],

      'topology' => [
        'class' => ['fullwidth'],
        'name'  => _('Device topology'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoTopology'), 
            ],
          'template' => get_template_path('netdiscoDevice.tpl', TRUE, dirname(__FILE__)),
          ],
        ];
  }


  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {
    global $config;
    parent::__construct($dn, $object, $parent, $mainTab);
    if (isset($this->attrs['fdNetdiscoDeviceServiceDn'])){
        $currentServer=$this->attrs['fdNetdiscoDeviceServiceDn'];
	$this->getMainInfo();
        }
    $this->ignore_account = FALSE;
  }



  function connectNetdiscoServer() {

      global $config;

      $ldap = $config->get_ldap_link();
      $ldap->cd($this->fdNetdiscoDeviceServiceDn);
      $ldap->search('(objectClass=fdNetdiscoServer)', ['fdNetdiscoServerURL','fdNetdiscoServerUser','fdNetdiscoServerPassword','fdNetdiscoServerDeviceDefinition','fdNetdiscoServerInterfaceDefinition']);


      $attrs = $ldap->fetch();

      $uServer = $attrs['fdNetdiscoServerURL'][0];
      $uUser   = $attrs['fdNetdiscoServerUser'][0];
      $uPass   = $attrs['fdNetdiscoServerPassword'][0];
      $this->physicalDef = $attrs['fdNetdiscoServerDeviceDefinition'][0];
      $this->interfacesDef = $attrs['fdNetdiscoServerInterfaceDefinition'];


      $this->netdiscoHandler = new netdisco_server($uServer,$uUser,$uPass);
      return($this->netdiscoHandler->doLogin());

  }

}
