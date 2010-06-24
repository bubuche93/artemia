<?php 
//$Id: video-play-divx.tpl.php,v 1.1.2.4 2010/06/06 15:40:55 iLLin Exp $
/*
 * @file
 * Theme file to handle divx output.
 * 
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 * 
 */
?> 
<object classid="clsid:67DABFBF-D0AB-41fa-9C46-CC0F21721616" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" codebase="http://go.divx.com/plugin/DivXBrowserPlugin.cab">
  <param name="src" value="<?php print $video->url; ?>" />
  <param name="pluginspage" value="http://go.divx.com/plugin/download/" />
  <param name="mode" value="zero" />
  <object class="video-object" type="video/divx" data="<?php print $video->url; ?>" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>" mode="zero">
    <?php print t('No video?  Get the DivX Web Player !plugin', array('!plugin' => l(t('Plugin'), 'http://go.divx.com/plugin/download/'))); ?>
  </object>
</object>