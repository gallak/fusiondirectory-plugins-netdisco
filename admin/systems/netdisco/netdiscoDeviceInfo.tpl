
<div>
{assign var=deviceInfo value=['name','contact','description','location', 'vendor','layers','model','uptime']}
<table>
 <tr>
  {foreach from=$deviceInfo item=value}
    <td>{$value}</td><td>{$attributes.fdNetdiscoInfo->$value}</td>
  {/foreach}
</tr>
</table>
</div>
