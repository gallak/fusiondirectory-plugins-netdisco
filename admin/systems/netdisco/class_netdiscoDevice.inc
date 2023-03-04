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

class vlanSelectAttribute extends SelectAttribute
{
  public function getAllVlans()
    {
    global $config;
    $ldap = $config->get_ldap_link();
    
    $allVlans=array();
    $ldap->cd($config->current['BASE']);
    $ldap->search('(objectClass=fdIpamVlan)', ['cn','fdIpamVlanInnerId','fdIpamActive']);
    while ($vlan = $ldap->fetch()) {
        $allVlans[$vlan['fdIpamVlanInnerId'][0]]= array( 'vlanID' => $vlan['fdIpamVlanInnerId'][0],
                                       'vlanDescription' => $vlan['cn'][0]." (".$vlan['fdIpamVlanInnerId'][0].")",
                                        );
        }
    return($allVlans);
  }


    function setParent (&$plugin)
    {
        parent::setParent($plugin);
        if (is_object($this->plugin)) {
            $this->updateChoices();
        }
    }

    function updateChoices ()
    {
        global $config;
        $vlansId    = [];
        $vlansDescription = [];
        foreach ($this->getAllVlans() as $name => $content){
            $vlansDescription[] = $content['vlanDescription'];
            $vlansId[] = $name;
        }
    $this->setChoices($vlansId, $vlansDescription);
    }

}



class netdiscoDevice extends simplePlugin
{

  public $netdiscoHandler;

  protected $displayHeader  = TRUE;
  protected $loaded = TRUE;
  public $physicalDef = "/ethernetCsmacd/";
  public $interfacesDef = [];
  public $portsColors =[];

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
  public function translateTopology($object)
  {
    global $config;
    $ldap = $config->get_ldap_link();

    $fdDeviceTopology=array();
    foreach ($object as $record){
      $record = (array)$record;
      if (preg_match($this->physicalDef,$record['type'])){
        // clean All alphabetical
        $cleanPort=preg_replace("/[a-zA-Z]/",'',$record['port']);
        // loop for testing  intergface
        foreach ($this->interfacesDef as $oneDef){
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
        //fetch Vlan member
        if ( isset($this->fdNetdiscoDeviceShowVlanMembership) && $this->fdNetdiscoDeviceShowVlanMembership == 'TRUE'){
          $netdiscoVlan= $this->netdiscoHandler->getDevicePortVlans($this->attrs['ipHostNumber'][0],$record['port']);
          $vlans = "";
          foreach ($netdiscoVlan as $value){
              $vlans = $vlans." ".$value->description." (".$value->vlan.")";
              $record['fd']['vlanList'][]=$value->vlan;
            }
          $record['fd']['vlans']=$vlans;
        }
        // fetch Power Consumption
        if ( isset($this->fdNetdiscoDeviceShowPower) && $this->fdNetdiscoDeviceShowPower == 'TRUE'){
          $record['fd']['power']=$this->netdiscoHandler->getDevicePortPower($this->attrs['ipHostNumber'][0],$record['port'])->power;
          
        }

        // fetch Remote Device and direct link inside Fusion
        if (isset($record['remote_id']) && isset($record['remote_port']) ){
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
        
        // highlight Vlan

        if ($this->fdNetdiscoFakeHighlightVlan != ''){
          //var_dump($this->fdNetdiscoFakeHighlightVlan);
          //var_dump($this->fdNetdiscoFakeHighlightType);
          //var_dump($record['fd']['vlanList']);
          switch ($this->fdNetdiscoFakeHighlightType) {
            case 'pvid':
              $arrayHL= [$record['pvid']];
              break;
            case 'members' :
              //var_dump("test des memebres vlman");
              $arrayHL= $record['fd']['vlanList'];
              break;
          }
          //var_dump($arrayHL);
          if ( in_array($this->fdNetdiscoFakeHighlightVlan,$arrayHL)){
            $record['fd']['highlight']="yes";
          }else{
            $record['fd']['highlight']="no";
          }
          $arrayHL=array();
        }
        //var_dump($record);

        $fdDeviceTopology[]=(object)$record;
        
      }
    }

    return($fdDeviceTopology);
  }


  // get topology from netdisco and redraw it
  public function refreshAndGetTopology ()
  {
      $this->getMainInfo();
      //get all info from netdisco
      $deviceTopology = $this->netdiscoHandler->getDevicePorts($this->attrs['ipHostNumber'][0]);
      // translate topology ( physical position / and other information)
      $deviceTopology=$this->translateTopology($deviceTopology);
      // Add information for creating real position for drawing switch
      $stack=new stackNetdiscoRender($deviceTopology,$this->fdNetdiscoDeviceType);
      $this->fdNetdiscoTopology=(array)$stack->renumberingStack();
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
             new vlanSelectAttribute(
                  _('Vlan to highlight'), _('Vlan High Light'),
                  'fdNetdiscoFakeHighlightVlan', FALSE),
              new SelectAttribute(_('PVID or member'),_('Which vlan should be highlighted'),
                  'fdNetdiscoFakeHighlightType',TRUE,
                  array('pvid','members'),'pvid',
                  array('PVID','vlan members'),
                  ),
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
      'topology' => [
        'class' => ['fullwidth'],
        'name'  => _('Device topology'),
        'attrs' => [
            new FakeAttribute('fdNetdiscoServerPortsColors'),
            new FakeAttribute('fdNetdiscoTopology'), 
            ],
         'template' => get_template_path('netdiscoDevice.tpl', TRUE, dirname(__FILE__)),
          ],
      'detailsIps' => [
        'class' => ['fullwidth'],
        'name'  => _('Device Ips'),
        'attrs' => [
            new FakeAttribute('fdNetdiscoInfoIp'),
            ],
          'template' => get_template_path('netdiscoDeviceInfoIp.tpl', TRUE, dirname(__FILE__)),
          ],


        ];
  }


  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {
    global $config;
    parent::__construct($dn, $object, $parent, $mainTab);
    $this->attributesAccess['fdNetdiscoFakeHighlightVlan']->setInLdap(FALSE);
    $this->attributesAccess['fdNetdiscoFakeHighlightType']->setInLdap(FALSE);
    
    if (isset($this->attrs['fdNetdiscoDeviceServiceDn'])){
        $currentServer=$this->attrs['fdNetdiscoDeviceServiceDn'];
	      $this->getMainInfo();
        }
    $this->ignore_account = FALSE;
  }

  public function makeLegend() :array
  {
    $palette=[];
    foreach ($this->portsColors as $color){
      $values=preg_split("/;/",$color);
      $palette[$values[1]][$values[2]] = array("description"=> $values[0], "color" => $values[3]);
    }

    array_shift($palette);
    return($palette);
  }



  function connectNetdiscoServer() {

      global $config;

      $ldap = $config->get_ldap_link();
      $ldap->cd($this->fdNetdiscoDeviceServiceDn);
      $ldap->search('(objectClass=fdNetdiscoServer)', ['fdNetdiscoServerURL','fdNetdiscoServerUser','fdNetdiscoServerPassword','fdNetdiscoServerDeviceDefinition','fdNetdiscoServerInterfaceDefinition','fdNetdiscoServerPortsColors']);


      $attrs = $ldap->fetch();

      $uServer = $attrs['fdNetdiscoServerURL'][0];
      $uUser   = $attrs['fdNetdiscoServerUser'][0];
      $uPass   = $attrs['fdNetdiscoServerPassword'][0];
      $this->physicalDef = $attrs['fdNetdiscoServerDeviceDefinition'][0];
      $this->interfacesDef = $attrs['fdNetdiscoServerInterfaceDefinition'];
      $this->portsColors=$attrs['fdNetdiscoServerPortsColors'];
      $this->fdNetdiscoServerPortsColors=$this->makeLegend();
      //var_dump($this->portsColors);
      //var_dump($this->makePalette());
      $this->netdiscoHandler = new netdisco_server($uServer,$uUser,$uPass);
      return($this->netdiscoHandler->doLogin());

  }

}
