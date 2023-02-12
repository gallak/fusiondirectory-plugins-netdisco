<div id="{$sectionId}" class="plugin-section fullwidth">
    <span class="legend">
        {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt="" />{/if}{$section|escape}
    </span>


    <div>
        {if $attributes.fdNetdiscoTopology}
            <!-- Begin Stack -->
            <table style="border: 2px solid green;">
                <tbody>
                    {foreach from=$attributes.fdNetdiscoTopology key=indice_unit item=unit}
                        <tr>
                            {assign var=widthModule value=$unit[0][0]|@count}
                            {assign var=nbModule value=$unit[0]|@count}
                            <td style="border: 2px solid orange;" colspan={math equation="x * y" x=$widthModule y=$nbModule}>UNIT {$indice_unit}</td>
                        </tr>
                        <tr>


                            {foreach from=$unit key=indice_module item=module}
                                <!--<td style="border: 2px solid orange;"> -->
                            <tr>
                                <td style="border: 2px solid red;" colspan={$module[0]|@count}>Module {$indice_module}</td>
                            </tr>
                            {foreach from=$module key=realY item=row}
                                <tr>
                                    {foreach from=$row key=realX item=data}
                                        <!-- display active port -->
                                        {if $data->fd['type'] == "TRUNK"}
                                            {assign var=leftColor value="#4FBA6F"}
                                            {assign var=rightColor value="#F29C1F"}
                                            {assign var=plugColor value="#990033"}                                            
                                        {else}
                                            {if $data->up_admin == "down"}
                                                {assign var=leftColor value="#35495E"}
                                                {assign var=rightColor value="#35495E"}
                                                {assign var=plugColor value="#000000"}
                                            {else}

                                                {if $data->up == "up"}
                                                    {assign var=leftColor value="#4FBA6F"}
                                                    {assign var=rightColor value="#F29C1F"}
                                                    {assign var=plugColor value="#5FBF00"}
                                                {else}
                                                    {assign var=leftColor value="#35495E"}
                                                    {assign var=rightColor value="#35495E"}
                                                    {assign var=plugColor value="#7f7f7f"}
                                                {/if}
                                                <!-- power display -->
                                                {if isset($data->fd['power'])}
                                                    {if $data->fd['power'] == "0" }
                                                        {assign var=powerColor value="#b2b2b2"}
                                                    {else}
                                                    {assign var=powerColor value="#ff7f00"}
                                                    {/if}
                                                {/if}
                                            {/if}
                                        {/if}


                                        <!-- show port details -->
                                        <td style="border: 1px solid black;align:center">
                                            {include file="./netdiscoDevicePort.tpl"}
                                        </td>
                                    {/foreach}
                                </tr>
                            {/foreach}

                            <!-- </td> -->
                        {/foreach}



                        </tr>
                    {/foreach}
                </tbody>
            </table>
        {else}
            {t}There is no device inside netdisco system or informations hasn't been updated V2 {/t}
        {/if}
</div>
