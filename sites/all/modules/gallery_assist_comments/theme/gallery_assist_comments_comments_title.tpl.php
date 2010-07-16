<?php
// $Id: gallery_assist_comments_comments_title.tpl.php,v 1.5 2010/05/29 01:48:42 quiptime Exp $

/**
 * @file gallery_assist_comments_comments_title.tpl.php
 * Default theme implementation to display the title of the comments content.
 *
 * @see theme_gallery_assist_comments_comments_title()
 *
 * Note:
 * Don't make the variable $counter translatable.
 */
?>
<h2 class="ga-comments"><?php print t('Comments'); ?> <?php print $counter; ?></h2>
