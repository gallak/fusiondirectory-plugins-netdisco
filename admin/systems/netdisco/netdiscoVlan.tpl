<div id="{$sectionId}" class="plugin-section{$sectionClasses}">
  <span class="legend">
    {if $sectionIcon}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span>
  <div>
    {foreach $attributes.fdNetdiscoReportVlan as $value}
        {$value}
    {/foreach}
</div>
</div>

