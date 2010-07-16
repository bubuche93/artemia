<?php
// $Id: gallery_assist_comments_container.tpl.php,v 1.4 2010/05/27 23:49:22 quiptime Exp $

/**
 * @file gallery_assist_comments_container.tpl.php
 * Default theme implementation to display a single comment.
 *
 * @param object $comment
 *   A comment object.
 *
 * @see theme_gallery_assist_comments_container()
 *
 * Note:
 * The first opened div is closed in theme_gallery_assist_comments_command_links().
 * Don't remove it from template.
 */
?>
<div <?php print $classes; ?>>
  <div class="ga-comment-wrapper">
    <div class="clear-block">
      <div class="submitted-by"><?php print $comment->name; ?></div>
      <div class="submitted-date"><?php print $comment->created; ?></div>
    </div>
    <div class="content">
      <?php print $comment->comment; ?>
    </div>
  </div>