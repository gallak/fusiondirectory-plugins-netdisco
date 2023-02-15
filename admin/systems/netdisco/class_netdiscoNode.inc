<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2016-2017  FusionDirectory
  Copyright (C) 2021-2023  Antoine Gallavardin

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
  protected $loaded = TRUE;

  public $allNdServer = array();
  public $currentServer ="";


  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Netdisco Node'),
      'plDescription' => _('Netdisco Node'),
      'plPriority'    => 30,
      'plObjectClass' => ['fdNetdiscoNode'],
      'plObjectType'  => ['workstation','server','printer','phone','component'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }



  public function refresh ()
  {
    global $config;
    $this->connectNetdiscoServer();
    $arrayNodeInfo = array();
    $ldap = $config->get_ldap_link();
    $ldap->cd($this->attrs['dn']);

    $ldap->search('(objectClass=fdNetworkInterface)', ['cn','macAddress']);
    while ($attrs = $ldap->fetch()){
        $mac = $attrs['macAddress'][0];
        $ifname = $attrs['cn'][0];
        $nodeInfo = $this->netdiscoHandler->getSearchNode(array('q'=> $mac));
        if ($nodeInfo) {
            $arrayNodeInfo[$ifname." (".$mac.")"] = displayTable($nodeInfo,
                                array( 'ips' => array('time_last_stamp','time_first_stamp','ip','dns','oui'),
                                        'sightings' => array('time_last_stamp','time_first_stamp','switch','port','vlan'),
                                        'netbios'   => array('time_last_stamp','time_first_stamp','nbname','nbuser','domain','ip'),
                                        'wireless' => array ('time_last_stamp','time_first_stamp')
                                        ),
                                );
        }
    }
    $this->fdNetdiscoTrace = $arrayNodeInfo;
  }


  static function getAttributesInfo (): array
  {
    return [
      'nodeConfig' => [
        'class' => ['fullwidth'],
        'name'  => _('Node Config'),
        'attrs' => [
             new netdiscoServerSelectAttribute (_('Netdisco Service'),_('Name of server where the client is connected to'),
                'fdNetdiscoNodeServiceDn',TRUE),
            ],
          ],
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
  if (isset($this->attrs['fdNetdiscoNodeServiceDn'])){
	    $currentServer=$this->attrs['fdNetdiscoNodeServiceDn'];
	    $this->refresh();
	  }
    $this->ignore_account = FALSE;  // affiche l'activation ou pas du plugin
  }



  function connectNetdiscoServer() {

      global $config;

      $ldap = $config->get_ldap_link();
      $ldap->cd($this->fdNetdiscoNodeServiceDn);
      $ldap->search('(objectClass=fdNetdiscoServer)', ['fdNetdiscoServerURL','fdNetdiscoServerUser','fdNetdiscoServerPassword']);
      $attrs = $ldap->fetch();

      $uServer = $attrs['fdNetdiscoServerURL'][0];
      $uUser   = $attrs['fdNetdiscoServerUser'][0];
      $uPass   = $attrs['fdNetdiscoServerPassword'][0];
      
      $this->netdiscoHandler = new netdisco_server($uServer,$uUser,$uPass);
      return($this->netdiscoHandler->doLogin());

  }

}
