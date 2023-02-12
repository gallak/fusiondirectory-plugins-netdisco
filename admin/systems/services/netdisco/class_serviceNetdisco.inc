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

class serviceNetdisco extends simpleService
{
  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Netdisco Server'),
      'plDescription' => _('Netdisco Server').' ('._('Services').')',
      'plIcon'        => 'geticon.php?context=applications&icon=netdisco&size=16',
      'plObjectClass' => ['fdNetdiscoServer'],

      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }

  /*!
   *  \brief The main function : information about attributes
   */
  static function getAttributesInfo (): array
  {
    return  [
      'main' => [
        'name'  => _('Netdisco server'),
        'attrs' => [
          new StringAttribute(
            _('URL'), _('URL to access the netdisco server'),
            'fdNetdiscoServerURL', TRUE
          ),
          new StringAttribute(
            _('User'), _('User to access netdisco server Rest API.'),
            'fdNetdiscoServerUser', FALSE
          ),
          new PasswordAttribute(
            _('Password'), _('Password to access netdisco server Rest API.'),
            'fdNetdiscoServerPassword', FALSE
          )
        ]
      ],
      'device' => [
        'name'  => _('Devices definition'),
        'attrs' => [
          new StringAttribute(
            _('PhysicalDevices'), _('Regexp for detecting physical Device'),
            'fdNetdiscoServerDeviceDefinition', TRUE
          ),
          new OrderedArrayAttribute(
            new CharSeparatedCompositeAttribute(
              _("beware, all letter are remove before computing real position, please take care of it for regexp"), //_('interface Definition'),
              'fdNetdiscoServerInterfaceDefinition',
                [
                  new StringAttribute (_('Description'),    _('constructor / Model / example'), 'constructorDevice'),
                  new StringAttribute (_('Interface Detection'), _('Regexp for matching interface name'),  'regexpDevice'),
                  new StringAttribute (_('Name interface builder'), _('Regexp for keeping device position'),  'printfDevice'),
                ],
              ';',
              '',
              _('Interface Definition'),
              
            ),
            FALSE,
            [],
            TRUE
          ),
        ]
      ],
    ];
  }
}
