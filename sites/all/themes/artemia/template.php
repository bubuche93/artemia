<?php
/**
* Sets the body tag class and id attributes.
*
* From the Theme Developer's Guide, http://drupal.org/node/32077
*
* @param $is_front
*   boolean Whether or not the current page is the front page.
* @param $layout
*   string Which sidebars are being displayed.
* @return
*   string The rendered id and class attributes.
*/
function phptemplate_body_attributes($is_front = false, $layout = 'none') {

  if ($is_front) {
    $body_id = $body_class = 'homepage';
  }
  else {
    // Remove base path and any query string.
    global $base_path;
    list(,$path) = explode($base_path, $_SERVER['REQUEST_URI'], 2);
    list($path,) = explode('?', $path, 2);
    $path = rtrim($path, '/');
    // Construct the id name from the path, replacing slashes with dashes.
    $body_id = str_replace('/', '-', $path);
    // Construct the class name from the first part of the path only.
    list($body_class,) = explode('/', $path, 2);
  }
  $body_id = 'page-'. $body_id;
  $body_class = 'section-'. $body_class;

  // Use the same sidebar classes as Garland.
  $sidebar_class = ($layout == 'both') ? 'sidebars' : "sidebar-$layout";

  return " id=\"$body_id\" class=\"$body_class $sidebar_class\"";
}


function template_search_block_form($form) {
$form['search_block_form_keys']['#attributes'] =
array(
'onselect' => value='\'\'',
'onclick' => value='\'\''
);
$form['search_block_form_keys']['#value'] =
'rechercher ...';
$form['submit']['#type'] = 'hidden';
$output = form_render($form);
return $output;
}


?>
