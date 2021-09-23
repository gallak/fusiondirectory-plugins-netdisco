<div>

{foreach $attributes.fdNetdiscoTrace as $macInfo}
<div id="{$macInfo@key}" class="plugin-section">
<span class="legend">
        	{t}Mac address{/t} : {$macInfo@key}
</span>
        {foreach $macInfo as $value}
		<div>
		        <b>{t}{$value@key}{/t}</b><br>
		        {$value}
		</div>
        {/foreach}
	</div>
</div>
{/foreach}

</div>
