<svg  width="40" height="28" viewBox="0 0 160 110" xmlns="http://www.w3.org/2000/svg" x="0" y= "0">

{if isset($data->fd['remote']['link'])}
<a xlink:href="{$data->fd['remote']['link']}" target="_blank"> 
{/if}
<g>

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
  <rect rx="2" stroke-width="2" stroke="#444444" x="5" y="5" id="portIn" height="100" width="150" fill="#999999"/>
  <rect rx="2" stroke-width="2" id="ledLeft" height="20" width="30" y="85" x="5" stroke="#444444" fill="{$leftColor}"/>
  <g>
  <rect rx="2" stroke-width="2" id="ledRight" height="20" width="30" y="85" x="125" stroke="#444444" fill="{$rightColor}"/>
<!--  <animate xlink:href="#ledRight" attributeType="auto" attributeName="fill" dur="1s" to="#f06d06"/>
  <animate attributeType="CSS" attributeName="opacity" from="1" to="0" dur="1s" repeatCount="indefinite" /> -->
</g>
  <rect stroke-width="2" rx="2" id="plug" height="60" width="130" y="15" x="15" stroke="#444444" fill="{$plugColor}"/>
  <rect stroke-width="2" stroke="#444444" id="ergot" height="10" width="30" y="75" x="65" fill="{$plugColor}"/>
{if isset($data->fd['power'])}
  <text font-style="normal" font-weight="bold" xml:space="preserve" text-anchor="start" font-family="sans-serif" font-size="50" stroke-width="0" id="svg_8" y="63" x="61" stroke="#000" fill="{$powerColor}">P</text>
{/if}
 </g>
 </a>
</svg>

