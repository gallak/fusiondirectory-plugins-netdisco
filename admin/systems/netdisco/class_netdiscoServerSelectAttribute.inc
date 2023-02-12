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



class netdiscoServerSelectAttribute extends SelectAttribute
{
  public function getAllNetdiscoServers()
    {
    global $config;
    $ldap = $config->get_ldap_link();
    $allServers=array();
    $ldap->search('(objectClass=fdNetdiscoServer)', ['dn','cn','description']);
    while ($uServer = $ldap->fetch()) {
        $allServers[$uServer['dn']]= array(     'netdiscoCn' => $uServer['cn'][0],
                                                'netdiscoDescription' => $uServer['description'][0],
                                        );
        }
    return($allServers);
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
        $serversName    = [];
        $serversDescription = [];
        foreach ($this->getAllNetdiscoServers() as $name => $content){
            $serversDescription[] = $content['netdiscoCn']." ".$content['netdiscoDescription'];
            $serversDn[] = $name;
            $this->setChoices($serversDn, $serversDescription);
        }
    }

}

