<?php
// $Id: gallery_assist_comments_command_links.tpl.php,v 1.1 2010/05/27 23:49:22 quiptime Exp $

/**
 * @file gallery_assist_comments_command_links.tpl.php
 * Default theme implementation to display the command links unpublish/pulish and delete.
 *
 * @see theme_gallery_assist_comments_command_links()
 *
 * Note:
 * The last closed div tag is opened in theme_gallery_assist_comments_container().
 * Don't remove it from template.
 */
?>
  <div <?php print $classes; ?>>
    <?php print $links; ?>
  </div>
</div>