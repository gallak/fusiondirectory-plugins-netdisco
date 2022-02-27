<div id="{$sectionId}" class="plugin-section">
  <span class="legend">
    {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span>

{if $attributes.fdNetdiscoInfo}
<table>
        {foreach $attributes.fdNetdiscoInfo as $info}
<tr>
            <td><b>{t}{$info@key}{/t}</b></td><td>{$info}</td>
</tr>
        {/foreach}
</table>
{else}
<span>{t}No informations (Mac address has been specified ?){/t}</span>
{/if}
</div>
