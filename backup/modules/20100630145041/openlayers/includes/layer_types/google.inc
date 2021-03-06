<?php
// $Id: google.inc,v 1.1.2.11 2010/03/22 23:55:10 zzolo Exp $

/**
 * @file
 * Google Layer Type
 */

/**
 * OpenLayers Google Layer Type class
 */
class openlayers_layer_type_google extends openlayers_layer_type {
  function __construct($layer = array(), $map = array()) {
    parent::__construct($layer, $map);
    if (isset($this->data)) {
      $this->data += $this->options_init();
    }
    else {
      $this->data = $this->options_init();
    }
  }
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'serverResolutions' => openlayers_get_resolutions('900913'),
      'maxExtent' => openlayers_get_extent('900913'),
      'projection' => array('900913'),
      'layer_type' => 'google',
      'layer_handler' => 'google',
      'baselayer' => TRUE,
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    $google_layer_types = array(
      'hybrid' => 'Hybrid',
      'normal' => 'Normal',
      'satellite' => 'Satellite');
    return array(
      'type' => array(
        '#title' => t('Google Layer Type'),
        '#type' => 'select',
        '#default_value' => isset($this->data['type']) ? $this->data['type'] : 'normal',
        '#options' => $google_layer_types
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_google'
      ),
      'numZoomLevels' => array(
        '#type' => 'textfield',
        '#title' => t('Number of Zoom Levels'),
        '#description' => t('Satellite and hybrid maps are occasionally
          unavailable at higher zoom levels.'),
        '#default_value' => isset($this->data['numZoomLevels']) ?
          $this->data['numZoomLevels'] : '15'
      ),
    );
  }

  /**
   * Layer-type-wide settings form
   */
  function settings_form() {
    return array(
      'openlayers_layers_google_api' => array(
        '#type' => 'textfield',
        '#title' => t('Google API Key'),
        '#default_value' => variable_get('openlayers_layers_google_api', ''),
        '#description' => t('<a href="@google">Obtain an API key from Google for your domain</a>',
          array('@google' => 'http://code.google.com/apis/maps/signup.html'))
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/layer_types/google.js');
    drupal_set_html_head('<script src="'. check_url("http://maps.google.com/maps?file=api&v=2&sensor=false&key=" . variable_get('openlayers_layers_google_api', variable_get('googlemap_api_key', ''))) . '" type="text/javascript"></script>');
    return $this->options;
  }
}
