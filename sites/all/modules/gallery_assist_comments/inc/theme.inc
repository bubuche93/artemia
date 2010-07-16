<?php
// $Id: theme.inc,v 1.5 2010/05/29 01:48:42 quiptime Exp $

/**
 * @file
 * Theme functions for the Gallery Assist Comments module.
 */

/**
 * Provide the title for the comments display.
 *
 * @param int $counter_raw
 *   An counter value. The counter digit itself.
 * @return string
 *   The themed title of the comments, included the themeable comments counter.
 * @ingroup themeable
 *
 * @see gallery_assist_comments_list()
 */
function gallery_assist_comments_comments_title($title, $counter_raw) {
  $counter = gallery_assist_comments_comments_counter($counter_raw);

  $output = '<div id="ga-comments-title-wrapper">';
  $output .= theme('gallery_assist_comments_comments_title', $title, $counter);
  $output .= '</div>';

  return $output;
}

/**
 * Display an title for the comments display.
 *
 * @param $title
 *   The title. E. g. Comments 5
 * @return string
 *   The themeable title of the comments.
 *
 * @ingroup themeable
 */
function theme_gallery_assist_comments_comments_title($title, $counter) {
  return '<h2 class="ga-comments">'. t($title) . '</h2> <span class="counter">'. $counter .'</span>';
}

/**
 * Display the comments counter, displayed on a comments list.
 *
 * @param int $counter_raw
 *   The raw counter digit of Database count.
 * @return string
 *   The themed counter.
 */
function gallery_assist_comments_comments_counter($counter) {
  $output = '';

  if ($counter == 0) {
    return FALSE;
  }
  else {
    return theme('gallery_assist_comments_comments_counter', $counter);
  }
}

/**
 * Display the comments counter.
 *
 * @param int $counter
 * @return string
 *   The themeable counter.
 *
 * @ingroup themeable
 */
function theme_gallery_assist_comments_comments_counter($counter) {
  return $counter;
}

/**
 * Display the text for the "Display all comments" link.
 *
 * @return string
 *   The text. E. g. "Display all comments"
 * @see _gallery_assist_comments_area_links()
 */
function gallery_assist_comments_say_all($content = 'All comments') {
  return t(theme('gallery_assist_comments_say_all', $content));
}

/**
 * Display the text for the "Display all comments" link.
 *
 * @return string
 *   The text. E. g. "Display all comments"
 * @ingroup themeable
 */
function theme_gallery_assist_comments_say_all($content) {
  return $content;
}

/**
 * Display the text for the "Display fewer comments" link.
 *
 * @return string
 *   The text. E. g. "Display fewer comments"
 * @see _gallery_assist_comments_area_links()
 */
function gallery_assist_comments_say_fewer($content = 'Fewer comments') {
  return t(theme('gallery_assist_comments_say_fewer', $content));
}

/**
 * Display the text for the "Display fewer comments" link.
 *
 * @return string
 *   The text. E. g. "Display fewer comments"
 * @ingroup themeable
 */
function theme_gallery_assist_comments_say_fewer($content) {
  return $content;
}

/**
 * Display the comment form box.
 *
 * @param $node_light
 *   A node object.
 * @return string
 *   The themed comment form.
 */
function gallery_assist_comments_form_box($node_light) {
  return theme('gallery_assist_comments_form_box', drupal_get_form('gallery_assist_comments_form', $node_light));
}

/**
 * Display the comment form box.
 *
 * @param string $content
 *   The rendered comment form content.
 * @return string
 *   The themeable comment form.
 *
 * @ingroup themeable
 */
function theme_gallery_assist_comments_form_box($content) {
  return $content;
}

/**
 * Display the comment form button value.
 *
 * @return string
 *   The themed and translatable button value.
 */
function gallery_assist_comments_comment_button() {
  $output = FALSE;

  $output = theme('gallery_assist_comments_comment_button');

  $value = $output ? $output : 'Add comment';

  return t($value);
}

/**
 * Display the text of comment form button value.
 * 
 * Note for the returned content:
 * - Don't use the t() function.
 * - Don't use HTML tags.
 * - Don't use images.
 * - There are text only possible.
 *
 * @return string 
 *   The themeable button value.
 *
 * @ingroup themeable
 */
function theme_gallery_assist_comments_comment_button() {
  return 'Add comment';
}

/**
 * Provide an comment.
 *
 * @param object $comment
 *   A comment object.
 * @param array $attributes
 *   An array of HTML attributes to apply to the comment container.
 * @return string
 *   A themed complete comment.
 * @ingroup themeable
 */
function gallery_assist_comments_container($comment, $attributes = array()) {
  $classes = $attributes['class'] ? $attributes['class'] = 'ga-comment-container clear-block '. $attributes['class'] : 'ga-comment-container clear-block';
  $classes = drupal_attributes(array('class' => $classes));

  return theme('gallery_assist_comments_container', $comment, $classes);
}

/**
 * Display a single comment.
 *
 * @param object $comment
 *   A comment object.
 * @param string $classes
 *   The string contains CSS classes to apply to the comment container.
 * @return string
 *   A themeable comment.
 *
 * @ingroup themeable
 *
 * Note:
 * The first opened div is closed in theme_gallery_assist_comments_command_links().
 */
function theme_gallery_assist_comments_container($comment, $classes) {
  $output = '';

  $output = '<div '. $classes .'>'."\n";
  $output .= '<div class="ga-comment-wrapper">'."\n";
  $output .= '<div class="clear-block">'."\n";
  $output .= '<div class="submitted-by">'. $comment->name .'</div>'."\n";
  $output .= '<div class="submitted-date">';
  $output .= $comment->created;
  $output .= '</div>'."\n";
  $output .= '</div>'."\n";
  $output .= '<div class="content">'."\n";
  $output .= $comment->comment;
  $output .= '</div>'."\n";
  $output .= '</div>'."\n";

  return $output;
}

/**
 * Build command links for a comment (unpublish, publish, delete) with respect to the current user's access permissions.
 *
 * @param object $comment
 *   A comment object.
 * @param array $links
 *   The array contains the rendered command links.
 * @param array $attributes
 *   An array of HTML attributes to apply to the links.
 * @return string
 *   The themed content containing the links.
 *
 * @see gallery_assist_comments_list()
 */
function gallery_assist_comments_command_links($comment, $links, $attributes = array()) {
  $classes = $attributes['class'] ? $attributes['class'] = 'ga-comment-command-links ga-comment-command-links-'. $comment->cid .' clear-block '. $attributes['class'] : 'ga-comment-command-links ga-comment-command-links-'. $comment->cid .' clear-block';
  $classes = drupal_attributes(array('class' => $classes));

  return theme('gallery_assist_comments_command_links', $comment, $links, $classes);
}

/**
 * Display the command links container.
 *
 * @param object $comment
 *   A comment object.
 * @param string $links
 *   Contain the command links unpublish/publish, delete.
 * @param array $classes
 *   The string contains CSS classes to apply to the links container.
 * @return string
 *   The themeable command links container.
 *
 * @ingroup themeable
 *
 * Note:
 * The last closed div tag is opened in theme_gallery_assist_comments_container().
 */
function theme_gallery_assist_comments_command_links($comment, $links, $classes) {
  $output = '';

  $output = '<div '. $classes .'>'."\n";
  $output .= $links;
  $output .= '</div>'."\n";
  $output .= '</div>'."\n";

  return $output;
}

/**
 * Display the comment submitted information.
 *
 * @param string $submitted
 *   The HTML formatted submitted line.
 * @return string
 *   The themeable submitted date.
 *
 * @see gallery_assist_comments_submitted()
 * @see gallery_assist_comments_load_content()
 *
 * @ingroup themeable
 */
function theme_gallery_assist_comments_submitted($submitted) {
  return '<span class="ga-comment-submitted">'. $submitted .'</span>';
}

/**
 *  Display comments content.
 *
 * @param string $content
 *   The HTML formatted comments content.
 * @return string
 *   The themeable comments content.
 *
 * @see gallery_assist_comments_submitted()
 * @see gallery_assist_comments_load_content()
 * 
 * @ingroup themeable
 */
function theme_gallery_assist_comments_content($content) {
  return $content;
}
