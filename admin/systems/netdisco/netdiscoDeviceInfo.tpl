<div id="{$sectionId}" class="plugin-section">
  <span class="legend">
    {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span>

{assign var=deviceInfo value=['name','contact','description','location', 'vendor','layers','model','uptime']}
<table>
  {foreach from=$deviceInfo item=value}
    <tr><td><b>{t}{$value}{/t}</b></td><td>{$attributes.fdNetdiscoInfo->$value}</td><tr>
  {/foreach}
</table>
</div>
