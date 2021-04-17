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

  protected $displayHeader  = TRUE;
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
var_dump($subnetInfo);
    if ($subnetInfo) {
        $subnetDisplayInfo = $this->displayNode($nodeInfo);
    }else{
        $netdiscoData=new netdiscoDataRenderer();
        foreach ($netdiscoData->getDictionnary('subnet') as $key => $value){
            $subnetDisplayInfo[$key] = _('not information available');
        }
    }
    array_shift($subnetDisplayInfo);
    $this->fdNetdiscoTrace = $subnetDisplayInfo;
  }

  public function displayNode ($subnetInfo){

    $netdiscoData = new netdiscoDataRenderer();
    $arrayDiv=array();
    $smarty = get_smarty();
    $item = 'subnet';
    $div = new divSelectBox('rows'.$item);
    // set height depending values
    $div->setHeight(count(array_keys($subnetInfo->$item)) * 30 + 40);
    $headers=array();
    foreach ($netdiscoData->getDictionnary($item) as $field => $attr ) {
        $headers[]=$netdiscoData->getDictionnary($item)[$field]['label'];
        }
    $div->setHeaders($headers);

    foreach ($subnetInfo->$item as &$record) {
        $fields=array();
        foreach ($netdiscoData->getDictionnary($item) as $field => $attr ){
            $fields[]=[ 'string' => $netdiscoData->getRenderValue($attr['type'] ,$record->$field)];
        }
        $div->addEntry($fields);
    }


    return $div->drawList();;
 }


  static function getAttributesInfo (): array
  {
    return [
      'topology' => [
        'class' => ['fullwidth'],
        'name'  => _('Node Trace'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoReportSubnet'),
            ],
          'template' => get_template_path('subnet.tpl', TRUE, dirname(__FILE__)),
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
