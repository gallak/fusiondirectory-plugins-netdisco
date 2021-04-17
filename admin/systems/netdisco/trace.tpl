
<!-- <div id="{$sectionId}" class="plugin-section">
  <span class="legend">
    {if $sectionIcon}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span> -->
  <div>


    {foreach $attributes.fdNetdiscoTrace as $macInfo}
        <h1>{$macInfo@key}</h1>


        {foreach $macInfo as $value}
        <h2>{$value@key}</h2>
            {$value}
        {/foreach}


    {/foreach}

</td>


</div>
<!--
</div>
-->
