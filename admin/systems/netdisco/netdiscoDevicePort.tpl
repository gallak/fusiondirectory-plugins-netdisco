<svg  width="40" height="28" viewBox="0 0 160 110" xmlns="http://www.w3.org/2000/svg" x="0" y= "0">
{if isset($data->fd['remote']['link'])}
  <a xlink:href="{$data->fd['remote']['link']}" target="_blank"> 
{/if}
<title>Name  : {$data->name}
{if isset($data->port)}Port  : {$data->port}
{/if}
{if isset($data->speed)}Speed : {$data->speed}
{/if}
{if isset($data->up)}State : {$data->up}
{/if}
{if isset($data->pvid)}Pvid  : {$data->pvid}
{/if}
{if isset($data->stp)}SpanningTree : {$data->stp}
{/if}
{if isset($data->fd['power'])}Power : {$data->fd['power']} mW
{/if}
{if isset($data->fd['vlans'])}Vlans : {$data->fd['vlans']}
{/if}
{if isset($data->fd['remote']['port'])}Lien vers : {$data->fd['remote']['port']}
{/if}
  </title>
  <rect rx="3" id="portOut" height="110" width="160" y="0" x="0" stroke="#444444" fill="#ffffff"/>
  <!-- port config -->

  {if isset($data->slave_of)}
  <rect rx="2" stroke-width="1" stroke="#444444" x="5" y="5" id="portIn" height="100" width="150" fill="{$attributes.fdNetdiscoServerPortsColors['slave_of']['slave_of']["color"]}"/>
  {else}
  <rect rx="2" stroke-width="1" stroke="#444444" x="5" y="5" id="portIn" height="100" width="150" fill="#c0bfbc"/>
  {/if}
 <!-- Half / Full duplex -->  
  <rect rx="2" stroke-width="1" id="ledLeft" height="20" width="30" y="85" x="5" stroke="#444444" fill="{$attributes.fdNetdiscoServerPortsColors['duplex'][$data->duplex]["color"]}"/>
    <!-- Speed -->
  <rect rx="2" stroke-width="1" id="ledRight" height="20" width="30" y="85" x="125" stroke="#444444" fill="{$attributes.fdNetdiscoServerPortsColors['speed'][$data->speed]["color"]}"/>

  <!-- up / down -->>
  <rect stroke-width="1" rx="2" id="plug" height="50" width="120" y="20" x="20" stroke="#444444" fill="{$attributes.fdNetdiscoServerPortsColors['up'][$data->up]["color"]}"/>
  
    <!-- if power -->
  {if $data->fd['power'] > 0}
  <text font-style="normal" font-weight="bold" xml:space="preserve" text-anchor="start" font-family="sans-serif" font-size="50" stroke-width="0" id="svg_8" y="63" x="61" stroke="#000" fill="#FF0000">P</text>
   <!-- fin if power -->
  {/if}
</svg>