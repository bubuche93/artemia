// $Id: gallery_assist.fancybox.selector.js,v 1.3 2009/11/29 16:50:23 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.fancybox.selector.js');

$(document).ready(function() {

  $(".image-sheet a").fancybox({
    'padding'            : Drupal.settings.lightbox_conf['fancybox']['padding'],
    'imageScale'         : Drupal.settings.lightbox_conf['fancybox']['imageScale'],
    'zoomOpacity'        : Drupal.settings.lightbox_conf['fancybox']['zoomOpacity'],
    
    'zoomSpeedIn'        : Drupal.settings.lightbox_conf['fancybox']['zoomSpeedIn'],
    'zoomSpeedOut'       : Drupal.settings.lightbox_conf['fancybox']['zoomSpeedOut'],
    'zoomSpeedChange'    : Drupal.settings.lightbox_conf['fancybox']['zoomSpeedChange'],
    
    'easingIn'           : Drupal.settings.lightbox_conf['fancybox']['easingIn'],
    'easingOut'          : Drupal.settings.lightbox_conf['fancybox']['easingOut'],
    'easingChange'       : Drupal.settings.lightbox_conf['fancybox']['easingChange'],
    
    'frameWidth'         : Drupal.settings.lightbox_conf['fancybox']['frameWidth'],
    'frameHeight'        : Drupal.settings.lightbox_conf['fancybox']['easingOut'],
    'overlayShow'        : Drupal.settings.lightbox_conf['fancybox']['overlayShow'],
    
    'overlayOpacity'     : Drupal.settings.lightbox_conf['fancybox']['overlayOpacity'],
    'overlayColor'       : Drupal.settings.lightbox_conf['fancybox']['overlayColor'],
    'enableEscapeButton' : Drupal.settings.lightbox_conf['fancybox']['enableEscapeButton'],
    
    'showCloseButton'    : Drupal.settings.lightbox_conf['fancybox']['showCloseButton'],
    'hideOnOverlayClick' : Drupal.settings.lightbox_conf['fancybox']['hideOnOverlayClick'],
    'hideOnContentClick' : Drupal.settings.lightbox_conf['fancybox']['hideOnContentClick'],
    'centerOnScroll'     : true,
	});
  
});
