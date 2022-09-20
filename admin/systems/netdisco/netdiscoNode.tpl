<div>

{foreach $attributes.fdNetdiscoNodeServiceDn as $netdiscoSrv}
{$netdiscoSrv}
{/foreach}


{if $attributes.fdNetdiscoTrace}
{foreach $attributes.fdNetdiscoTrace as $macInfo}
<div id="{$macInfo@key}" class="plugin-section">
<span class="legend">
        	{t}Interface{/t} - {$macInfo@key}
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
{else}
<span>{t}No informations (Mac address has been specified ?){/t}</span>
{/if}

</div>
