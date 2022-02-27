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


class netdiscoRangeIP extends simplePlugin
{

  public $netdiscoHandler;

  protected $displayHeader  = FALSE;
  protected $loaded = FALSE;

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('NetdiscoRangeIP'),
      'plDescription' => _('Netdisco Range'),
      'plPriority'    => 30,
      'plObjectClass' => [],
      'plObjectType'  => ['ipamSubnet'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }





  public function refresh ()
  {
    global $config;

    $this->connectNetdiscoServer();
    $subnetInfo = $this->netdiscoHandler->getReportIpSubnets(array('subnet'=> $this->attrs['fdIpamSubnetCIDR'][0] ));
    $ipInventoryInfo = $this->netdiscoHandler->getReportIpInventory(array('subnet'=> $this->attrs['fdIpamSubnetCIDR'][0] ));
    if ($subnetInfo) {
        $subnetDisplayInfo = displayTable(array('subnet' => $subnetInfo),
            array( 'subnet' => array('subnet','subnet_size','active','percent')));
    }
    if ($ipInventoryInfo) {
        $ipInventoryDisplayInfo = displayTable(array('inventory' => $ipInventoryInfo),
            array( 'inventory'  => array('dns','ip','mac','vendor','age','time_first','time_last')));
    }

    $this->fdNetdiscoReportSubnet = $subnetDisplayInfo;
    $this->fdNetdiscoReportIpInventory = $ipInventoryDisplayInfo;
  }


  static function getAttributesInfo (): array
  {
    return [
      'subnet' => [
        'class' => ['fullwidth'],
        'name'  => _('Subnet statistics'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoReportSubnet'),
            ],
          'template' => get_template_path('netdiscoSubnet.tpl', TRUE, dirname(__FILE__)),
          ],
      'inventory' => [
        'class' => ['fullwidth'],
        'name'  => _('Inventory'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoReportIpInventory'),
            ],
          'template' => get_template_path('netdiscoRangeIP.tpl', TRUE, dirname(__FILE__)),
          ],
    ];
  }


  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {

  parent::__construct($dn, $object, $parent, $mainTab);
  $this->refresh();
  $this->ignore_account = FALSE;  // affiche l'activation ou pas du plugin
  }



function connectNetdiscoServer() {

      global $config;


      $ldap = $config->get_ldap_link();
      $ldap->cd($config->current['BASE']);
      $ldap->search('(objectClass=fdNetdiscoServer)', ['fdNetdiscoServerURL','fdNetdiscoServerUser','fdNetdiscoServerPassword']);
      $attrs = $ldap->fetch();

      $uServer = $attrs['fdNetdiscoServerURL'][0];
      $uUser   = $attrs['fdNetdiscoServerUser'][0];
      $uPass   = $attrs['fdNetdiscoServerPassword'][0];

      $this->netdiscoHandler = new netdisco_server($uServer,$uUser,$uPass);
      return($this->netdiscoHandler->doLogin());

}

}
