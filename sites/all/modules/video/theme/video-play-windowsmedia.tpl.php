<?php 
//$Id: video-play-windowsmedia.tpl.php,v 1.1.2.4 2010/06/06 15:40:55 iLLin Exp $
/*
 * @file
 * Theme file to handle windows media output.
 * 
 * Variables passed.
 * $video is the video object.
 * $node is the node object.
 * 
 */
?>

<object type="video/x-ms-wmv" data="<?php print $video->url; ?>" width="<?php print $video->player_width; ?>" height="<?php print $video->player_height; ?>">
    <param name="src" value="<?php print $video->url; ?>" valuetype="ref" type="<?php print $video->url; ?>">
    <param name="animationatStart" value="true">
    <param name="transparentatStart" value="true">
    <param name="autostart" value="<?php print $video->autoplay; ?>">
    <param name="controller" value="1">
    <?php print t('No video?  Get the Windows Media !plugin', array('!plugin' => l(t('Plugin'), 'http://www.microsoft.com/windows/windowsmedia/player/download/'))); ?>
</object>