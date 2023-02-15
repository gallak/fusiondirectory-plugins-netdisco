<div id="{$sectionId}" class="plugin-section fullwidth">
    <span class="legend">
        {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt="" />{/if}{$section|escape}
    </span>

    <div>
    {if $attributes.fdNetdiscoServerPortsColors}
    <h3>{t}Legend of colours{/t}</h3>
    <table >
        <tbody>
        <tr style="border: 1px solid black;">
        {foreach from=$attributes.fdNetdiscoServerPortsColors key=type item=variables}
            <td>{t}{$type}{/t}</td>
            {foreach from=$variables key=value item=info}
                <td style="background-color: {$info['color']}; color: {$info['color']}">__</td>
                <td style="color: {$info['color']};">{$info['description']}</td>
            {/foreach}
        </tr>
        {/foreach}
        </tbody>
    </table>
    {/if}
    </div>


    <div>
        {if $attributes.fdNetdiscoTopology}
            <h3>{t}Simple representation{/t}</h3>
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
            {t}There is no device inside netdisco system or informations hasn't been updated{/t}
        {/if}
    </div>
</div>
