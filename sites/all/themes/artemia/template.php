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
function phptemplate_body_attributes($is_front = false, $layout = 'template_files') {

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
  $sidebar_class = ($layout == 'template_files') ? 'sidebars' : "sidebar-$layout";

  return " id=\"$body_id\" class=\"$body_class $sidebar_class\"";


}


/**
 * Preprocessor for theme('node').
 */
function artemia_preprocess_node(&$vars) {
  $vars['layout'] = FALSE;

  // Don't show node title on page views
  if (menu_get_object() === $vars['node']) {
    if (!isset($_GET['print'])) {
      unset($vars['title']);
    }
    $vars['attr']['class'] .= ' node-page';
  }
  else {
    $vars['attr']['class'] .= ' node-teaser';
  }
  // We don't want the print friendly header on individual nodes.
  if (isset($vars['pre_object'])) {
    unset($vars['pre_object']);
  }
}

/**
 * Preprocessor for theme('page').
 */
 
function artemia_preprocess_page(&$vars) {
  // Site name
  $vars['site_name'] = theme('site_name');
}



/**
 * Add picture in a menu
 */
 
function artemia_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }

  if(strpos($link['localized_options']['attributes']['title'], '<img') === 0) {
    // Allow HTML if the menu description is an image tag:
    $link['localized_options']['html'] = TRUE;
    // exchange description with title
    $tmp = $link['localized_options']['attributes']['title'];
    $link['localized_options']['attributes']['title'] = $link['title'];
    $link['title'] = $tmp;
  }

  return l($link['title'], $link['href'], $link['localized_options']);
}


function artemia_theme() {
	return array(
		'user_login_block' => array(
			'template' => 'user-login-block',
			'arguments' => array('form' => NULL),
		)
	);


}

?>
