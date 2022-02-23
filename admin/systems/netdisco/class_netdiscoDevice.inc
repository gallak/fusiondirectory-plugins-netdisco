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

  protected $displayHeader  = FALSE;
  protected $loaded = TRUE;

  var $objectclasses = array('fdNetdiscoDevice');

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Netdisco'),
      'plDescription' => _('Netdisco'),
      'plTitle'       => _('Netdisco'),
      'plPriority'    => 30,
      'plObjectClass'     => ['netdiscoDevice'],
      'plObjectType'  => ['component' ],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }





  public function refresh ()
  {
    global $config;

    $this->connectNetdiscoServer();
    // get Info From device
    $smarty = get_smarty();
    $deviceInfo = $this->netdiscoHandler->getDeviceInfo($this->attrs['ipHostNumber'][0]);

    if (! is_null($deviceInfo)){
        // get Topology of device
        $deviceTopology = $this->netdiscoHandler->getDevicePorts($this->attrs['ipHostNumber'][0]);
        $this->fdNetdiscoTopology=$this->drawDevice($this->setupDeviceTopology($deviceTopology));
        $this->fdNetdiscoInfo=$this->displayInfo($deviceInfo);
        $this->fdNetdiscoInfoIp=$this->displayIps($this->netdiscoHandler->getDeviceIpsDetails($this->attrs['ipHostNumber'][0]));

    }
  }

  public function displayInfo($data){
    $render="<table style=\"border: 1px solid black;\">";
    foreach($data as $field => $value){
        $render=$render."<tr><td><b>".$field."</b></td><td>".$value."</td></tr>";
    }
      $render=$render."</table>Ã";
  return $render;
  }


  public function displayIps($ipInfo){

    $netdiscoData = new netdiscoDataRenderer();
    $arrayDiv=array();
    $smarty = get_smarty();
    $item = 'ips';
    $displayInformations = array( 'ips'      => array('ip','alias','subnet','port','dns'));

    $div = new divSelectBox('rows'.$item);
    // set height depending values
    $div->setHeight(count(array_keys($ipInfo)) * 30 + 40);
    $headers=array();
    foreach ($displayInformations[$item] as $field) {
        $headers[]=_($field);
        }
    $div->setHeaders($headers);

    foreach ($ipInfo as &$record) {
        $fields=array();
        foreach ($displayInformations[$item] as $field){
            $fields[]=[ 'string' => $netdiscoData->getRenderValue($netdiscoData->getDictionnary($item)[$field]['type'] ,$record->$field)];
        }
        $div->addEntry($fields);
    }
    return $div->drawList();
 }



  // increase information per port ( power / vlan )
  private function setupDeviceTopology($deviceTopology){
        $orderedStack=array();
        foreach ($deviceTopology as $record){
            $portXY=explode(".",$record->port);
            $swNumber=$portXY[0];
            $portNumber=$portXY[1];
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

    return($orderedStack);
  }


  // Draw device with SVG
  public function drawDevice ($deviceTopology){

    $deviceDraw= new stackNetDiscoRenderer();
    return($deviceDraw->convertNetdiscoArrayToGraphicalArray($deviceTopology,$this->fdNetdiscoDeviceType));
  }

  static function getAttributesInfo (): array
  {
    return [
      'settings' => [
        'name'  => _('Device Settings'),
        'attrs' =>[
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
                 'fdNetdiscoUpdate', _('Fetch'),'refresh'
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
    //$this->refresh();
    $this->ignore_account = TRUE;
  }



  function connectNetdiscoServer() {

      global $config;


      $ldap = $config->get_ldap_link();
      $ldap->search('(objectClass=fdNetdiscoServer)', ['fdNetdiscoServerURL','fdNetdiscoServerUser','fdNetdiscoServerPassword']);
      $attrs = $ldap->fetch();

      $uServer = $attrs['fdNetdiscoServerURL'][0];
      $uUser   = $attrs['fdNetdiscoServerUser'][0];
      $uPass   = $attrs['fdNetdiscoServerPassword'][0];

      $this->netdiscoHandler = new netdisco_server($uServer,$uUser,$uPass);
      return($this->netdiscoHandler->doLogin());

  }

}
