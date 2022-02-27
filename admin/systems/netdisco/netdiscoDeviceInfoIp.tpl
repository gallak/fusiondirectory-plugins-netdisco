<div id="{$sectionId}" class="plugin-section{$sectionClasses}">
  <span class="legend">
    {if $sectionIcon}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span>
  <div>
        {foreach $attributes.fdNetdiscoInfoIp as $value}
        <div>
                {$value}
        </div>
        {/foreach}
    <!-- {$attributes.fdNetdiscoInfoIp} -->
</div>
</div>

