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


class netdiscoNode extends simplePlugin
{

  public $netdiscoHandler;

  protected $displayHeader  = TRUE;
  protected $loaded = FALSE;

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('NetdiscoNode'),
      'plDescription' => _('Netdisco Node'),
      'plPriority'    => 30,
      'plObjectClass' => [],
      'plObjectType'  => ['workstation','server'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }





  public function refresh ()
  {
    global $config;

    $this->connectNetdiscoServer();
    $arrayNodeInfo = array();
    foreach ( $this->attrs['macAddress'] as $mac ){
        $nodeInfo = $this->netdiscoHandler->getSearchNode(array('q'=> $mac));
        if ($nodeInfo) {
            $arrayNodeInfo[$mac] = $this->displayNode($nodeInfo);
        }else{
            $netdiscoData=new netdiscoDataRenderer();
            foreach ($netdiscoData->getDictionnary('node') as $key => $value){
                $arrayNodeInfo[$mac][$key] = _('not information available');
            }
        }
    }
    array_shift($arrayNodeInfo);
    $this->fdNetdiscoTrace = $arrayNodeInfo;
  }

  public function displayNode ($deviceInfo){


    $netdiscoData = new netdiscoDataRenderer();
    $arrayDiv=array();
    $smarty = get_smarty();
    // what to display
    $displayInformations = array( 'ips'       => array('time_last_stamp','time_first_stamp','ip','dns','oui'),
                                 'sightings' => array('time_last_stamp','time_first_stamp','switch','port','vlan'),
			         'netbios'   => array('time_last_stamp','time_first_stamp','nbname','nbuser','domain','ip')
                               );

    foreach ($deviceInfo as $item => $value){
        if ($value) {
            $div = new divSelectBox('rows'.$item);
            // set height depending values
            $div->setHeight(count(array_keys($deviceInfo->$item)) * 30 + 40);
            $headers=array();
//            foreach ($netdiscoData->getDictionnary('node')[$item] as $field => $attr ) {
              foreach ($displayInformations[$item] as $field){
		$headers[]=_($field);
            }
            $div->setHeaders($headers);

            foreach ($deviceInfo->$item as &$record) {
                $fields=array();
                //foreach ($netdiscoData->getDictionnary('node')[$item] as $field => $attr ){
                foreach ($displayInformations[$item] as $field){
                    $fields[]=[ 'string' => $netdiscoData->getRenderValue($netdiscoData->getDictionnary('node')[$item][$field]['type'] ,$record->$field)];
                }
            $div->addEntry($fields);
            }
        $arrayDiv[$item] = $div->drawList();
        }

    }
    return $arrayDiv;
 }


  static function getAttributesInfo (): array
  {
    return [
      'nodeTrace' => [
        'class' => ['fullwidth'],
        'name'  => _('Node Trace'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoTrace'),
            ],
          'template' => get_template_path('netdiscoNode.tpl', TRUE, dirname(__FILE__)),
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
      $ldap->search('(objectClass=fdNetdiscoServer)', ['fdNetdiscoServerURL','fdNetdiscoServerUser','fdNetdiscoServerPassword']);
      $attrs = $ldap->fetch();

      $uServer = $attrs['fdNetdiscoServerURL'][0];
      $uUser   = $attrs['fdNetdiscoServerUser'][0];
      $uPass   = $attrs['fdNetdiscoServerPassword'][0];

      $this->netdiscoHandler = new netdisco_server($uServer,$uUser,$uPass);
      return($this->netdiscoHandler->doLogin());

}

}
