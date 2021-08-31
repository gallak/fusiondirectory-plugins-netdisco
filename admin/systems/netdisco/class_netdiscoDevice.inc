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
  

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Netdisco'),
      'plDescription' => _('Netdisco'),
      'plPriority'    => 30,
      'plObjectClass'     => ['netdiscoDevice'],
      'plObjectType'  => ['component' ],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }





  public function refresh ()
  {
    global $config;

    var_dump($this->connectNetdiscoServer());
    // get Toplogy of device
    $deviceTopology = $this->netdiscoHandler->getDevicePorts($this->attrs['ipHostNumber'][0]);
    // get Info From device
    $deviceInfo = $this->netdiscoHandler->getDeviceInfo($this->attrs['ipHostNumber'][0]);

    $smarty = get_smarty();
    $smarty->assign("deviceInfo",array('name','description','location', 'description','uptime'));
    $this->fdNetdiscoTopology = $this->drawDevice($this->setupDeviceTopology($deviceTopology));
    $this->fdNetdiscoInfo = $deviceInfo;

  }
  // increase information per port ( power / vlan )
  private function setupDeviceTopology($deviceTopology){
        $orderedStack=array();
        foreach ($deviceTopology as $record){
            $portXY=explode(".",$record->port);
            $swNumber=$portXY[0];
            $portNumber=$portXY[1];
            $orderedStack[$swNumber - 1][$portNumber -1]['summary'] = $record;
            $netdiscoVlan= $this->netdiscoHandler->getDevicePortVlans($this->attrs['ipHostNumber'][0],$record->port);
	    $vlans = "";
	    foreach ($netdiscoVlan as $value){
    			$vlans = $vlans." ".$value->description." (".$value->vlan.")";
            }
	    $orderedStack[$swNumber - 1][$portNumber -1]['vlan'] = $vlans;
	    $orderedStack[$swNumber - 1][$portNumber -1]['power'] = $this->netdiscoHandler->getDevicePortPower($this->attrs['ipHostNumber'][0],$record->port);
        }

    return($orderedStack);
}


  // Draw device with SVG
  public function drawDevice ($deviceTopology){

    $deviceDraw= new stackNetDiscoRenderer();
    return($deviceDraw->convertNetdiscoArrayToGraphicalArray($deviceTopology));
  }

  static function getAttributesInfo (): array
  {
    return [
      'topology' => [
        'class' => ['fullwidth'],
        'name'  => _('Device topology'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoTopology'),
             new FakeAttribute('fdNetdiscoInfo'),
            ],
          'template' => get_template_path('switch.tpl', TRUE, dirname(__FILE__)),
          ],
    ];
  }


  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {
    global $config;
    parent::__construct($dn, $object, $parent, $mainTab);
    $this->refresh();
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
