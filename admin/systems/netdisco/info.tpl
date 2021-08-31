
<div>
{assign var=deviceInfo value=['name','description','location','uptime']};
<ul>
  {foreach from=$deviceInfo item=value}
    <li>{$value} : {$attributes.fdNetdiscoInfo->$value}</li>
  {/foreach}
</ul>
  </div>
