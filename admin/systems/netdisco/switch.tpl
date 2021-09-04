<div>
{include file="./info.tpl"}
<div>
  {foreach from=$attributes.fdNetdiscoTopology key=indice item=unit}
    <table style="border: 2px solid black;">
    <thead>
</thead>
<tbody>
<tr><td style="border: 2px solid red;font-weight: bold;" colspan={$unit[1]|@count}>Unit {$indice}</td></tr>



 {foreach from=$unit key=realY item=row}
      <tr style="border: 1px solid grey;">
        {foreach from=$row key=realX item=data}
                {assign var=position value="."|explode:$data['summary']->port}
		{if $data['summary']->up == "up"}
                        {assign var=leftColor value="#4FBA6F"}
                        {assign var=rightColor value="#F29C1F"}
			{assign var=plugColor value="#5FBF00"}
		{else}
			{assign var=leftColor value="#35495E"}
			{assign var=rightColor value="#35495E"}
			{assign var=plugColor value="#7f7f7f"}
		{/if}
                {if isset($data['power'])}
                    {if $data['power']->power == "0" }
			{assign var=powerColor value="#b2b2b2"}
		    {else}
			{assign var=powerColor value="#ff7f00"}
                    {/if}
		{/if}

          <td style="border: 1px solid grey;">
	{include file="./port.tpl"}
		</td>

        {/foreach}
      </tr>
    {/foreach}
</tbody>
    </table>
  {/foreach}

</div>

  </div>
