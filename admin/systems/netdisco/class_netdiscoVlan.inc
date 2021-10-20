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


require_once("netdisco/class_netdisco.php");
require_once("netdisco/class_netdiscoDataRenderer.php");


class netdiscoVlan extends simplePlugin
{

  public $netdiscoHandler;

  protected $displayHeader  = FALSE;
  protected $loaded = FALSE;

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('NetdiscoVlan'),
      'plDescription' => _('Netdisco Vlan'),
      'plPriority'    => 30,
      'plObjectClass' => [],
      'plObjectType'  => ['ipamVlan'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }





  public function refresh ()
  {
    global $config;

    $this->connectNetdiscoServer();
    $vlanInfo = $this->netdiscoHandler->getSearchVlan(array('q'=> $this->attrs['fdIpamVlanInnerId'][0]));
    if ($vlanInfo) {
	$vlanDisplayInfo = $this->displayVlan($vlanInfo);
    }

    $this->fdNetdiscoReportVlan = $vlanDisplayInfo;
  }

  public function displayVlan($vlanInfo){

    $netdiscoData = new netdiscoDataRenderer();
    $arrayDiv=array();
    $smarty = get_smarty();
    $item = 'vlan';
    $displayInformations = array( 'vlan'      => array('ip','os','model','pcount','dns','vendor','vlans'));


    $div = new divSelectBox('rows'.$item);
    // set height depending values
    $div->setHeight(count(array_keys($vlanInfo)) * 30 + 40);
    $headers=array();
    foreach ($displayInformations[$item] as $field) {
        $headers[]=_($field);
        }
    $div->setHeaders($headers);

    foreach ($vlanInfo as &$record) {
        $fields=array();

	foreach ($displayInformations[$item] as $field){
            $fields[]=[ 'string' => $netdiscoData->getRenderValue($netdiscoData->getDictionnary($item)[$field]['type'] ,$record->$field)];
        }
        $div->addEntry($fields);
    }
    return $div->drawList();
 }



  static function getAttributesInfo (): array
  {
    return [
      'subnet' => [
        'class' => ['fullwidth'],
        'name'  => _('Vlan Information'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoReportVlan'),
            ],
          'template' => get_template_path('netdiscoVlan.tpl', TRUE, dirname(__FILE__)),
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
