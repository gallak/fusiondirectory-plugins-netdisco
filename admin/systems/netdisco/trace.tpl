
<div id="{$sectionId}" class="plugin-section">
  <span class="legend">
    {if $sectionIcon}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span>
  <div>

{foreach $attributes.fdNetdiscoTrace->sightings as $value}
  {$value->time_first}, {$value->time_last}, {$value->switch}, {$value->port}, {$value->vlan}
{/foreach}

</td>


</div>
</div>
