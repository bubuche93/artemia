// $Id: gallery_assist.colorbox.selector.js,v 1.2 2009/10/31 12:00:00 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.colorbox.selector.js');

$(document).ready(function() {
  $("a.cboxelement").colorbox({
    slideshow : Drupal.settings.lightbox_conf['colorbox']['slideshow'],
    transition : Drupal.settings.lightbox_conf['colorbox']['transition'],
    speed : Drupal.settings.lightbox_conf['colorbox']['speed'],
    href : Drupal.settings.lightbox_conf['colorbox']['href'],
    title : Drupal.settings.lightbox_conf['colorbox']['title'],
    rel : Drupal.settings.lightbox_conf['colorbox']['rel'],
    width : Drupal.settings.lightbox_conf['colorbox']['width'],
    height : Drupal.settings.lightbox_conf['colorbox']['height'],
    innerWidth : Drupal.settings.lightbox_conf['colorbox']['innerWidth'],
    innerHeight : Drupal.settings.lightbox_conf['colorbox']['innerHeight'],
    initialWidth : Drupal.settings.lightbox_conf['colorbox']['initialWidth'],
    initialHeight : Drupal.settings.lightbox_conf['colorbox']['initialHeight'],
    maxWidth : Drupal.settings.lightbox_conf['colorbox']['maxWidth'],
    maxHeight : Drupal.settings.lightbox_conf['colorbox']['maxHeight'],
    scalePhotos : Drupal.settings.lightbox_conf['colorbox']['scalePhotos'],
    scrolling : Drupal.settings.lightbox_conf['colorbox']['scrolling'],
    iframe : Drupal.settings.lightbox_conf['colorbox']['iframe'],
    inline : Drupal.settings.lightbox_conf['colorbox']['inline'],
    html : Drupal.settings.lightbox_conf['colorbox']['html'],
    photo : Drupal.settings.lightbox_conf['colorbox']['photo'],
    opacity : Drupal.settings.lightbox_conf['colorbox']['opacity'],
    open : Drupal.settings.lightbox_conf['colorbox']['open'],
    preloading : Drupal.settings.lightbox_conf['colorbox']['preloading'],
    current : Drupal.settings.lightbox_conf['colorbox']['current'],
    previous : Drupal.settings.lightbox_conf['colorbox']['previous'],
    next : Drupal.settings.lightbox_conf['colorbox']['next'],
    close : Drupal.settings.lightbox_conf['colorbox']['close'],
    overlayClose : Drupal.settings.lightbox_conf['colorbox']['overlayClose'],
  });
});
