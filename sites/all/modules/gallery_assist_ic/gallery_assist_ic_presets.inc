<?php
// $Id: gallery_assist_ic_presets.inc,v 1.1 2009/10/11 05:55:52 jcmc Exp $

/**
 * @file
 * The function of this script is to create ImageCache presets acording the format settings 
 * from Gallery Assist and content types with gallery assigment.
 */

/**
 * Presets will be created by gallery_assist_ic install or by 
 * the assigment of gallery functionalities to a content type.
 */
function gallery_assist_presets_maker($type, $data = array()) {

  $thm_size  = $data['thumbnail_size'] ? $data['thumbnail_size'] : 150;
  $prev_size = $data['preview_size'] ? $data['preview_size'] : 550;

  $presets['thm'] = array();

  // Thumbnail.
  $presets['thm'] = array (
    'presetname' => 'gallery_assist-'. $type .'-thumbnail-'. $thm_size,
    'actions' => array (
      'weight' => '0',
      'module' => 'gallery_assist',
      'action' => 'imagecache_scale',
      'data' => array (
        'width' => $thm_size,
        'height' => $thm_size,
        'upscale' => 0,
      ),
    ),
  );

  // Preview
  $presets['prev'] = array (
    'presetname' => 'gallery_assist-'. $type .'-preview-'. $prev_size,
    'actions' => array (
      'weight' => '0',
      'module' => 'gallery_assist',
      'action' => 'imagecache_scale',
      'data' => 
        array (
          'width' => $prev_size,
          'height' => $prev_size,
          'upscale' => 0,
      ),
    ),
  );

  imagecache_preset_save($presets['thm']);
  $presets['thm']['actions']['presetid'] = db_last_insert_id('imagecache_preset', 'presetid');
  imagecache_action_save($presets['thm']['actions']);
  imagecache_preset_flush($presets['thm']);
  imagecache_presets(TRUE);

  imagecache_preset_save($presets['prev']);
  $prev_ic_presetid = db_last_insert_id('imagecache_preset', 'presetid');
  $presets['prev']['actions']['presetid'] = $prev_ic_presetid;
  imagecache_action_save($presets['prev']['actions']);
  imagecache_preset_flush($presets['prev']);
  imagecache_presets(TRUE);

  $ic_data = array(
    'thm_presetid' => $presets['thm']['actions']['presetid'],
    'thm_presetname' => 'gallery_assist-'. $type .'-thumbnail-'. $thm_size ,
    'prev_presetid' => $presets['prev']['actions']['presetid'],
    'prev_presetname' => 'gallery_assist-'. $type .'-preview-'. $prev_size ,
    'use_imagecache' => 1 ,
  );

  variable_set('gallery_assist_'. $type .'_ic_data', $ic_data);

  return 1;
}
