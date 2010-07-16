<?php
// $Id: gallery_assist_lightboxes_display.inc,v 1.1.2.11 2010/04/06 00:58:41 jcmc Exp $

/**
 * @file
 * Build and display the Gallery Assist Container and prepare the output acord wich lightbox is selected.
 * This file will be included if the parameter "show_layout" is setted to "grid"
 * 
 * @see gallery_assist_include_display().
 */

/**
 * Display the Gallery Assist Container as grid.
 */
function gallery_assist_display($node, $opt='', $owner='') {
  global $user;

  $conf = $node->gallconf[$node->type];
  $conf['lightbox'] = variable_get('gallery_assist_lightbox', 'none') == 'none' ? 0 : 1;
  $conf['lightbox_deriv'] = $conf['lightbox'] == 1 ? variable_get('gallery_assist_lightbox', 'none') : '';
  $conf['lightbox_data'] = $conf['lightbox'] == 1 ? variable_get('gallery_assist_lightbox_data', FALSE) : '';
  $conf['nid'] = $node->nid;

  $my_teaser_count = $conf['t_items_per_row'] > 0 ?  $conf['t_items_per_row'] : $conf['items_per_row'];
  $my_page_count = $conf['items_per_row'] == 'unlimited' ? count($node->gallitems) : $conf['items_per_row'] * $conf['rows_per_page'];
  if ($conf['items_per_row'] == 'unlimited' && $opt == 'teaser') {
    $my_teaser_count = $conf['t_items_per_row'] > 0 ?  $conf['t_items_per_row'] : 3;
  }
  $my_previous = '‹ previous';
  $my_next = 'next ›';

  if (count($node->gallitems) == 0) { 
    return; 
  }

  foreach ($node->gallitems as $pid => $item) {
    $img_obj = image_get_info($item->tpath);
    
    if ($img_obj['width'] > $img_obj['height']) { 
      $img_obj['layout'] = 'h'; 
    }
    else { 
      $img_obj['layout'] = 'v'; 
    }

    foreach ($img_obj as $k => $v) {
      $node->gallitems[$pid]->$k = $v;
      $node->gallitems[$pid]->show_title = $node->show_title;
    }

    $node->gallitems[$pid]->my_nid = $node->nid;
    $my_item_box = gallery_assist_item_box($node->type, $node->gallitems[$pid], $conf, $img_obj);
    $my_boxes[] = $my_item_box;
    $block_boxes[] = $my_item_box;
  }

  // Prepare the pager OLD VARIANT OF gallery_assist.
  $node->gall_pager->show = $my_page_count;

  $node->gall_pager->cshow = $node->gall_pager->show;

  $theme = $user->theme ? $user->theme : variable_get('theme_default', FALSE);
  $my_pager_layout = $conf['pager_layout'];

  $my_grafic_next_path = file_exists(drupal_get_path('theme', $theme) .'/ga_pager/'. $my_pager_layout .'/next.png') ? drupal_get_path('theme', $theme) .'/ga_pager/'. $my_pager_layout .'/next.png' : drupal_get_path('module', 'gallery_assist') .'/ga_pager/default/next.png';
  $my_grafic_next = theme('image', $my_grafic_next_path, '', 'go to the next');
  $my_next = $conf['pager_symbol'] == 0 ? t('next ›') : $my_grafic_next;
  $my_grafic_previous_path = file_exists(drupal_get_path('theme', $theme) .'/ga_pager/'. $my_pager_layout .'/previous.png') ? drupal_get_path('theme', $theme) .'/ga_pager/'. $my_pager_layout .'/previous.png' : drupal_get_path('module', 'gallery_assist') .'/ga_pager/default/previous.png';
  $my_grafic_previous = theme('image', $my_grafic_previous_path, '', 'go to the previous');
  $my_previous = $conf['pager_symbol'] == 0 ? t('‹ previous') : $my_grafic_previous;

  $my_separator = $conf['pager_symbol'] == 0 ? ' | ' : '&nbsp;&nbsp;&nbsp;';

  if ($_GET['page'] == '' || $_GET['page'] == 1) {
    $pages = 0;
    $page = 2;
    $preview = '';
    if (count($my_boxes) <= $node->gall_pager->show) {
      $next = "";
    }
    else {
      $next = l($my_next, $_GET['q'], array('html' => TRUE, 'attributes' => array('class' => $node->type .' pager-next'), 'query' => array('page' => $page, 'titles' => $_GET['titles'])));
    }
  }
  else {
    $pages = $node->gall_pager->show * ($_GET['page'] - 1);
    $page = $_GET['page'];
    $node->gall_pager->cshow = $node->gall_pager->show * $_GET['page'];

    $preview = l($my_previous, $_GET['q'], array('html' => TRUE, 'attributes' => array('class' => $node->type .' pager-previous'), 'query' => array('page' => ($_GET['page'] - 1), 'titles' => $_GET['titles'])));
    if ($node->gall_pager->cshow < count($my_boxes)) {
      $next = l($my_next, $_GET['q'], array('html' => TRUE, 'query' => array('page' => ($_GET['page'] + 1), 'titles' => $_GET['titles'])));
    }
    else {
      $next = "";
    }
  }

  if (!empty($preview)) {
    $link[] = $preview;
  }
  if (!empty($next)) {
    $link[] = $next;
  }

  // Atrr.
  //$c_w = $opt == 'teaser' && $node->gallconf[$node->type]['t_items_per_row'] > 0 ? $node->gallconf[$node->type]['t_container_width'] : $node->gallconf[$node->type]['container_width'];
  $c_w = $node->gallconf[$node->type]['container_width'];
  $my_attr_array = array(
    'style' => 'max-width:'. $c_w .'px;',
  );

  $stl = array();
  $pstl = array();

  switch ($opt) {
    case 'teaser':
    for ($i = 0; $i < $my_teaser_count; $i++) {
      $boxes .= $my_boxes[$i];
      unset($my_boxes[$i]);
    }
    $node->gallconf[$node->type]['op'] = 'teaser';
  
    if ($conf['layout']['ga_align'] != 'none' || $conf['layout']['t_ga_float'] != 'none') {
      
      //ALIGN TEASER
      switch ($conf['layout']['ga_align']) {
        case 'left':
          $stl['l'] = 'margin-right:auto';
          break;

        case 'center':
          $stl['ml'] = 'margin-left:auto';
          $stl['mr'] = 'margin-right:auto';
          break;

        case 'right':
          $stl['r'] = 'margin-left:auto';
          break;

      }

      //FLOAT TEASER
      switch ($conf['layout']['t_ga_float']) {
        case 'left':
          $stl['l'] = 'float:left';
          $stl['mr'] = 'margin-right:10px';
          unset($stl['ml']);
          break;

        case 'right':
          $stl['l'] = 'float:right';
          $stl['ml'] = 'margin-left:10px';
          unset($stl['mr']);
          break;

      }
    } // end (styles)
    break;

    case 'page': 
    for ($i=$pages; $i < $node->gall_pager->cshow; $i++) {
      if (drupal_strlen($my_boxes[$i]) > 0) {
        $count[] =  1;
      }
      $boxes .= $my_boxes[$i];
      unset($my_boxes[$i]);
    }
    $node->gallconf[$node->type]['op'] = 'page';
    
    if ($conf['layout']['gap_align'] != 'none' || $conf['layout']['p_ga_float'] != 'none') { //(styles)

      //ALIGN PAGE
      switch ($conf['layout']['gap_align']) {
        case 'left':
          $stl['l'] = 'margin-right:auto';
          $pstl['pl'] = 'text-align:left;';
          break;

        case 'center':
          $stl['ml'] = 'margin-left:auto';
          $stl['mr'] = 'margin-right:auto';
          $pstl['pl'] = 'text-align:center;padding-right:10px;';
          break;

        case 'right':
          $stl['r'] = 'margin-left:auto';
          $pstl['pl'] = 'text-align:right;';
          break;
      }

      //FLOAT PAGE
      switch ($conf['layout']['p_ga_float']) {
        case 'left':
          $stl['l'] = 'float:left';
          $stl['mr'] = 'margin-right:10px';
          unset($stl['ml']);
          break;

        case 'right':
          $stl['l'] = 'float:right';
          $stl['ml'] = 'margin-left:10px';
          unset($stl['mr']);
          break;

      }
    } // end (styles)
    break;
  }

  $my_attr_array['style'] .= implode(';', $stl); 

  $invlisten = '<div style="display:none;">'. implode("", $my_boxes) .'</div>';

  $extras['container_width'] = $opt == 'teaser' ? $conf['t_container_width'] : $conf['container_width'];
  $extras['pager_position'] = $conf['pager_position'];
  $extras['pager_style'] = implode(';', $pstl);

  if (count($node->gallitems) > $node->gall_pager->show && $opt == 'page') {
    $links = implode($my_separator, $link);
    $pager .= theme('gallery_assist_pager', $links, $extras);
  }

  $node->gallconf[$node->type]['my_attr'] = $node->gallconf[$node->type]['items_per_row'] == 'unlimited' ? '' : drupal_attributes($my_attr_array);
  $node->gallconf[$node->type]['nid'] = $node->nid;

  // Check for loading or prepare the load of the css- and js-files.
  $hilfo = variable_get('gallery_assist_lightboxes_files', FALSE);
  if ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'prettyPhoto' || $conf['lightbox_deriv'] == 'prettyphoto') {
    drupal_add_css(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/'. $conf['lightbox_deriv'] .'/css/prettyPhoto.css');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/'. $conf['lightbox_deriv'] .'/js/jquery.prettyPhoto.js');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.prettyPhoto.selector.js', 'module', 'header');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lytebox' || $conf['lightbox_deriv'] == 'lyteshow') {
    drupal_add_css(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/lytebox/lytebox.css');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lytebox/lytebox.js');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'thickbox') {
    drupal_add_css(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/thickbox/css/thickbox.css');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/thickbox/js/thickbox.js');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lightbox') {
    if (module_exists('lightbox2')) {
      drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.lightbox2.selector.js');
    }
    else {
      drupal_add_css(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/lightbox/css/lightbox.css');
      drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lightbox/js/prototype.js');
      drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lightbox/js/scriptaculous.js?load=effects,builder');
      drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lightbox/js/lightbox.js');
    }
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lightwindow') {
    // I have more to investigate about this lightbox
    // it have a conflict with the admin js script
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lightwindow/javascript/prototype.js');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lightwindow/javascript/scriptaculous.js?load=effects,builder');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/lightboxes/lightwindow/javascript/lightwindow.js');
    drupal_add_css(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/lightwindow/css/lightwindow.css');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'fancybox') {
    gallery_assist_get_includes($hilfo);
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.fancybox.selector.js');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'colorbox') {
    gallery_assist_get_includes($hilfo);
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.colorbox.selector.js');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'shadowbox') {
    gallery_assist_get_includes($hilfo);
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.shadowbox.selector.js');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'MultiBox') {
    gallery_assist_get_includes($hilfo);
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'milkbox') {
    gallery_assist_get_includes($hilfo);
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'highslide') {
    gallery_assist_get_includes($hilfo);
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'slimbox') {
    gallery_assist_get_includes($hilfo);
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.slimbox.selector.js');
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'interface') {
    drupal_add_css(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/interface/interface.css');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/interface/interface.js');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes') .'/lightboxes/interface/compressed/imagebox.js');
    drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes') .'/js/gallery_assist.interface.imagebox.selector.js');
  }

  if ($owner == 'block') {
    $num = count($block_boxes) < $conf['block_gallery_visibles'] ? count($block_boxes) :  $conf['block_gallery_visibles'];

    $rand_boxes = array_rand($block_boxes, $num);
    
    foreach ($rand_boxes as $ks => $divs) {
      $bboxes .= $block_boxes[$ks];
      unset($block_boxes[$ks]);
    }

    $b_invlisten = count($block_boxes) > 0 ? '<div style="display:none;">'. implode($block_boxes) .'</div>' : '';

    return theme('gallery_assist_block_display', $bboxes, $node->gallconf[$node->type], $node->type);
  }
  else {
    
    return theme('gallery_assist_display', $pager, $boxes . $invlisten, $node->gallconf[$node->type], $node->type);
    
  }
}

/**
 * Build and return the gallery item box.
 */
function gallery_assist_item_box($type, &$item, $conf, $img) {
  global $user;

  $item->nodetype = $type;
  $my_title_height = $item->show_title == 1 && $conf['thumbnail_size'] >= 100 ? 19 : 0;
  $my_item_width = $conf['thumbnail_size'] + $conf['thumbnail_size_plus'] + ($conf['item_padding'] * 2) + ($conf['item_border'] * 2);

  if (variable_get('gallery_assist_use_imagecache', 0) == 1 && $conf['force_image_height'] == 1) {
    $my_item_height = $conf['thumbnail_height_size'] + ($conf['item_padding'] * 2) + ($conf['item_border'] * 2) + $my_title_height;
  }
  else {
    $my_item_height = $conf['thumbnail_size'] + ($conf['item_padding'] * 2) + ($conf['item_border'] * 2) + $my_title_height;
  }

  $my_item_attr = array(
    'id' => 'item-box-'. $item->pid,
    'class' => $type .' item-box',
    'style' => 'height:'. $my_item_height .'px;'.
    'width:'. $my_item_width .'px;',
  );

  if ($conf['item_margin'] > 0) {
    $my_item_attr['style'] .= 'margin:'. $conf['item_margin'] .'px;';
  }
  $my_item_attr['style'] .= 'position:relative;';

  $link = l('edit', 'node/gallery_assist/'. $item->my_nid .'/'. $item->pid);
  $a_link = '<div id="image-box-'. $item->pid .'-links" class="'. $type .' image-box-links">[ '. $link .' ]</div>'."\n";

  $ga_item_attr  = drupal_attributes($my_item_attr);
  $edit_link  = $user->uid == $item->uid ? $a_link : '';
  $ga_item_box   = gallery_assist_image_box($type, $item, $conf, $img);
  $ga_item_title = $item->show_title == 1 && $conf['thumbnail_size'] >= 100 ? gallery_assist_title_box($item, $conf) : '';
  $ga_edit_link  = $_GET['links'] == 'on' ? $edit_link : '';
  
  $output = theme('gallery_assist_item_box', $ga_item_box, $ga_item_title, $ga_edit_link, $ga_item_attr);

  return $output;
}

/**
 * Theme the Gallery Assist Item-Box.
 *
 * @ingroup themeable
 */
function theme_gallery_assist_item_box($ga_item_box, $ga_item_title, $ga_edit_link = '', $ga_item_attr) {
  $output  = '<div'. $ga_item_attr .'>'."\n";
  $output .= $ga_edit_link;
  $output .= $ga_item_box;
  $output .= $ga_item_title;
  $output .= '</div>'."\n";

  return $output;
}

/**
 * Build and return the image box. 
 */
function gallery_assist_image_box($type, &$item, $conf, $img) {
  global $base_url;
  $ct_conf = variable_get('gallery_assist_'. $type .'_data', FALSE);

  if ($conf['shadow'] == 1) {
    $withshadow = ' shadow3';
    drupal_add_css(drupal_get_path('module', 'gallery_assist') .'/css/gallery_assist_shadow.css', 'module', 'all', FALSE);
  }

  if (variable_get('gallery_assist_use_imagecache', 0) == 1 && $conf['force_image_height'] == 1) {
    $my_image_height = $conf['thumbnail_height_size'];

    $my_box_attr = array(
      'id' => 'image-box-'. $item->pid,
      'class' => $type .' image-box'. $withshadow,
      'style' => 'height:'. $my_image_height .'px;'.
                 'width:'. $conf['thumbnail_size'] + ($conf['item_padding'] * 2) .'px;',
    );
  
    if ($conf['item_padding'] > 0) {
      $my_box_attr['style'] .= 'padding:'. $conf['item_padding'] .'px;';
    }
  
    if ($conf['item_border'] > 0) {
      $my_box_attr['style'] .= 'border:'. $conf['item_border'] .'px solid '. $conf['item_border_color'] .';';
    }
  
    $my_sheet_attr = array(
      'id' => 'image-sheet-'. $item->pid,
      'class' => $type .' image-sheet'. $my_img_behavior,
      'style' => 'height:'. $my_image_height .'px;'.
                 'width:'. $conf['thumbnail_size'] .'px;',
    );
  
    $img = gallery_assist_image_more_info($type, $img, $conf, $my_image_height);
  
    $my_layout = $img['pos'] == 'v' ? 'height' : 'width';
    $my_img_attr = array(
      $my_layout => $conf['thumbnail_size'] .'px',
      'style' => 'margin-top:'. $img['top'] .'px;',
    );
  }
  else {
    $my_box_attr = array(
      'id' => 'image-box-'. $item->pid,
      'class' => $type .' image-box'. $withshadow,
      'style' => 'height:'. $conf['thumbnail_size'] + ($conf['item_padding'] * 2) .'px;'.
                 'width:'. $conf['thumbnail_size'] + ($conf['item_padding'] * 2) .'px;',
    );
  
    if ($conf['item_padding'] > 0) {
      $my_box_attr['style'] .= 'padding:'. $conf['item_padding'] .'px;';
    }
  
    if ($conf['item_border'] > 0) {
      $my_box_attr['style'] .= 'border:'. $conf['item_border'] .'px solid '. $conf['item_border_color'] .';';
    }
  
    $my_sheet_attr = array(
      'id' => 'image-sheet-'. $item->pid,
      'class' => $type .' image-sheet'. $my_img_behavior,
      'style' => 'height:'. $conf['thumbnail_size'] .'px;'.
                 'width:'. $conf['thumbnail_size'] .'px;',
    );
  
    $img = gallery_assist_image_more_info($type, $img, $conf);
  
    $my_layout = $img['pos'] == 'v' ? 'height' : 'width';
    $my_img_attr = array(
      $my_layout => $conf['thumbnail_size'] .'px',
      'style' => 'margin-top:'. $img['top'] .'px;',
    );
  }

  $item->my_img_attr = $my_img_attr;
  if ($conf['lightbox_deriv'] == 'prettyPhoto') {
    $palt = array();
    if ($conf['lightbox_data']['showTitle']) {
      $palt[] = $item->ptitle;
    }
    $item->palt = implode(' | ', $palt);
  }
  $my_img = gallery_assist_prepare_thms($item, $conf);

  if ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'prettyPhoto' || $conf['lightbox_deriv'] == 'prettyphoto') {
    $my_desc = $conf['lightbox_data']['show_copy'] ? '&copy; '. $item->copyright .' | ' : '';
    $my_desc .= strlen($item->pdescription) > 0 && $conf['lightbox_data']['show_pdesc'] == 1 ? $item->pdescription : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_desc, 'class' => 'prettyPhoto', 'rel' => 'prettyPhoto[gallery'. $item->gid .']'), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lytebox') {
    $my_copy = strlen($item->copyright) > 0 && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'rel' => 'lytebox['. $item->my_nid .']'), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lyteshow') {
    $my_copy = strlen($item->copyright) > 0 && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'rel' => 'lyteshow['. $item->my_nid .']'), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'thickbox') {
    $my_copy = strlen($item->copyright) > 0 && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'class' => 'thickbox', 'rel' => $item->my_nid), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lightbox') {
    if (module_exists('lightbox2')) {
      $cmmt_text = module_exists('gallery_assist_comments') && $conf['comments'] == 1 ? t(' Comments are allowed.') : '';
      //$cmmt_query = module_exists('gallery_assist_comments') && $conf['comments'] == 1 ? t(' Comments are allowed.') : '';
      $cmmt_query = 'comments=1';
      //$cmmt_text = t(' Comments are allowed.');
      $addtext = array();
      $addtext[] = $item->copyright && $conf['lightbox_data']['show_copy'] == 1 ? '<span style="font-size:10px;">'. _filter_html(_filter_url($item->copyright, 1), 1) .'</span><br />' : '';
      $addtext[] = '<div id="gallery-item-caption-title" class="gallery-item-caption-title">'. l($item->ptitle, 'node/'. $item->my_nid .'/'. $item->pid, array('query' => $cmmt_query, 'attributes' => array('title' => t('Go to image page.') . $cmmt_text))) .'</div>';
      $pdesc = '<div id="gallery-item-caption-text" class="gallery-item-caption-text">'. $item->pdescription .'</div>';
      $addtext[] = isset($item->pdescription) && $conf['lightbox_data']['show_pdesc'] == 1 ? $pdesc : '';
      $addtext[] = $ct_conf['show_download_link'] == 1 && $conf['show_download_link'] == 1 && $item->sid == 1 || $ct_conf['show_download_link'] == 1 && $item->sid == 1  ? '<div id="gallery-item-download-link" class="gallery-item-download-link">'. l('download', file_create_url($item->download_path), array('attributes' => array('title' => t('Download original image'), 'target' => '_blank'))) .'</div>' : '';
      $output_text = implode('', $addtext);
      $attributes = array(
        'attributes' => array(
          'class' => 'prepared4lb2',
          'rel' => $conf['lightbox_data']['lightbox2_method'] .'['. $item->my_nid .']['. $output_text .']',
        ),
        'html' => TRUE,
      );
      $my_item_link = l($my_img, file_create_url($item->ppath), $attributes);
    }
    else {
      $my_copy = strlen($item->copyright) > 0 && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
      $my_desc = strlen($item->pdescription) > 0 && $conf['lightbox_data']['show_pdesc'] == 1 ? ' | '. $item->pdescription : '';
      $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle . $my_desc, 'rel' => 'lightbox['. $item->my_nid .']'), 'html' => TRUE));
    }
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'lightwindow') {
    $my_item_link = l($my_img, file_create_url($item->ppath), array(
      'attributes' => array(
        'title' => $item->copyright . $item->ptitle, 
        'rel' => 'gallery['. $item->my_nid .']',
      ), 
      'html' => TRUE,
    ));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'fancybox') {
    $my_copy = strlen($item->copyright) > 0 && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'rel' => $item->my_nid), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'colorbox') {
    $my_copy = isset($item->copyright) && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    //'title' => $my_copy . $item->ptitle, 
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'class' => 'cboxelement', 'rel' => $item->my_nid), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'shadowbox') {
    $my_copy = isset($item->copyright) && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    $my_title = $conf['lightbox_data']['show_title'] == 1 ? $item->ptitle : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $my_title, 'rel' => 'shadowbox['. $item->my_nid .']'), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'milkbox') {
    $my_copy = isset($item->copyright) && $conf['lightbox_data']['show_copy'] == 1 ? '<div><a href="#">(c) '. $item->copyright .'</a></div> | <br />' : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'rel' => 'milkbox['. $item->my_nid .']'), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'MultiBox') {
    $my_copy = isset($item->copyright) && $conf['lightbox_data']['show_copy'] == 1 ? '<div>'. _filter_html(_filter_url($item->copyright, 1), 1) .'</div> | <br />' : '';
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => $my_copy . $item->ptitle, 'class' => 'multiBoxDesc '. $item->my_nid), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'highslide') {
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('class' => 'highslide ', 'onclick' => 'return hs.expand(this, { slideshowGroup: '. $item->my_nid .' })'), 'html' => TRUE));
    $my_item_link .= '<div class="highslide-caption">';
    $my_item_link .= $item->copyright && $conf['lightbox_data']['show_copy'] == 1 ? '<span style="font-size:10px;"><a href="#">&copy;'. $item->copyright .'</a></span><br />' : '';
    $my_item_link .= '<strong>'. $item->ptitle .'</strong>';
    $my_item_link .= isset($item->pdescription) && $conf['lightbox_data']['show_pdesc'] == 1 ? '<div>'. $item->pdescription .'</div>' : '';
    $save_link = '<div>'. l('download' .': '. $item->ptitle, $item->download_path) .'</div>';
    $my_item_link .= $ct_conf['show_download_link'] == 1 && $conf['show_download_link'] == 1 && $item->sid == 1 || $ct_conf['show_download_link'] == 1 && $item->sid == 1  ? $save_link : '';
    $my_item_link .= '</div>';
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'interface') {
    $my_item_link = l($my_img, file_create_url($item->ppath), array('attributes' => array('title' => '<div><a href="#">&copy;'. $item->copyright .'</a></div> | <br />'. $item->ptitle, 'rel' => 'imagebox-'. $item->my_nid), 'html' => TRUE));
  }
  elseif ($conf['lightbox'] == 1 && $conf['lightbox_deriv'] == 'slimbox') {
    $my_itm = array();
    if ($conf['lightbox_data']['showTitle']) {
      $my_itm[] = $item->copyright && $conf['lightbox_data']['show_copy'] == 1 ? '(c) '. $item->copyright .' | ' : ''; // 
    }
    if ($conf['lightbox_data']['show_copy']) {
      $my_itm[] = isset($item->pdescription) && $conf['lightbox_data']['show_pdesc'] == 1 ? $item->pdescription : '';
    }
    $my_item = implode('', $my_itm);
    $my_item_link .= l($my_img, file_create_url($item->ppath), array('attributes' => array('original' => $item->opath, 'title' => $my_item, 'rel' => 'lightbox-'. $item->my_nid), 'html' => TRUE));
  }
  else {
    $item_link = 'node/'. $item->my_nid .'/'. $item->pid;
    $my_item_link = l($my_img, $item_link, array('html' => TRUE));
  }

  $edit_link_attr = array(
    'class' => $type .' ga-item-edit-link',
    'style' => 'position:absolute;top:0px;left:0px;width:100%;',
  );

  $get_values = $_GET;
  unset($get_values['q']);
  $link = l('[edit]', 'node/ga_edit_one/'. $item->my_nid .'/'. $item->pid, array('query' => $get_values));
  $a_link = '<div id="image-box-'. $item->pid .'-links" class="'. $type .' image-box-links">[ '. $link .' ]</div>'."\n";
  $edit_link  = $user->uid == 1 || $user->uid == $item->uid || user_access('edit gallery_assist content', $user) || user_access('edit gallery_assist_'. $type .' content', $user) ? $link : '';

  $output  = '<div'. drupal_attributes($my_box_attr) .'>'."\n";
  $output .= '  <div'. drupal_attributes($my_sheet_attr) .'>'."\n";
  $output .= '    '. $my_item_link;
  $output .= '  </div>'."\n";
  $output .= '</div>'."\n";

  $output2  = '<div'. drupal_attributes($my_box_attr) .'>'."\n";
  $output2 .= '<div'. drupal_attributes($my_sheet_attr) .'>'."\n";
  $output2 .= '    '. $my_item_link;
  $output2 .= '</div>'."\n";
  $output2 .= '</div>'."\n";
  $output2 .= '<div'. drupal_attributes($edit_link_attr) .'>'. $edit_link .'</div>'."\n";
  
  return $output2;
}

/**
 * Build and return the item title box. 
 */
function gallery_assist_title_box($item, $conf) {
  $points = drupal_strlen($item->ptitle) > $conf['title_substr'] ? "..." : '';
  $title = drupal_substr($item->ptitle, 0, $conf['title_substr']) . $points;

  $output  = '<div id="title-box-'. $item->pid .'" class="'. $item->nodetype .' title-box">'."\n";
  $output .= l($title, 'node/'. $item->my_nid .'/'. $item->pid, array('html' => TRUE, 'attributes' => array('title' => $item->ptitle)));
  $output .= '</div>'."\n";

  return $output;
}

/**
 * Theme the Gallery Assist Container as grid.
 *
 * @ingroup themeable
 */
function theme_gallery_assist_display($pager, $boxes, $conf, $type = 'gallery_assist') {

  $output  = '<div id="gallery-container-'. $conf['nid'] .'" class="'. $type .' gallery-container '. $conf['op'] .' clear-block"'. $conf['my_attr'] .'>';
  $output .= $conf['pager_position'] == 'top' || $conf['pager_position'] == 'both' ? $pager : '';
  $output .= $boxes;
  $output .= $conf['pager_position'] == 'bottom' || $conf['pager_position'] == 'both' ? $pager : '';
  $output .= '</div>';

  return  $output;
}

