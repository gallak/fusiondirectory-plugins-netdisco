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
      'plObjectClass' => ['netdiscoNode'], //'fdNetdiscoClient'],
      'plObjectType'  => ['workstation'],
//=> [
//        'name'        => _('NetDisco'),
//        'icon'        => 'geticon.php?context=types&icon=ethernet&size=16',
//      ]],

      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }





  public function refresh ()
  {
    global $config;

    $this->connectNetdiscoServer();
    $nodeInfo = $this->netdiscoHandler->getSearchNode(array('q'=>$this->attrs['macAddress'][0]));

    var_dump($nodeInfo);

    $this->fdNetdiscoTrace = $this->displayNode($nodeInfo);

  }

  public function displayNode ($deviceInfo){
/*
    $id   = "info"y;
    $div  = new divSelectBox('rows'.$id);
    $smarty = get_smarty();
//    var_dump(count(array_keys($typeDetails[$category])));
    $div->setHeight(count(array_keys($typeDetails[$category])) * 30);
    //$div->setHeight((int)180);

//    $div->setHeaders([_('Informations'), _('Values')]);

    $fields=[];
    $clientInfo=$category;
    //foreach (array_keys($typeDetails) as $clientInfo) {
      foreach ($typeDetails[$clientInfo] as $infoKey => $infoType){
          $fields = [
             ['string' => $infoType['label']],
             ['string' => $this->dataRenderer($infoType['type'],$arr[$infoKey])],
            ];
        $div->addEntry($fields);

        }
    //}

   return $div->drawList();
*/




   return $deviceInfo;
  }


  static function getAttributesInfo (): array
  {
    return [
      'topology' => [
        'class' => ['fullwidth'],
        'name'  => _('Node Trace'),
        'attrs' => [
             new FakeAttribute('fdNetdiscoTrace'),
            ],
          'template' => get_template_path('trace.tpl', TRUE, dirname(__FILE__)),
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
