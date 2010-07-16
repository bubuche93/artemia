// $Id: gallery_assist.slimbox.selector.js,v 1.1.2.5 2010/03/17 02:30:42 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.slimbox.selector.js');

$(document).ready(function(){
  if (Drupal.settings.lightbox_conf['slimbox']['loop'] == true) {
    var my_loop = true;
  }
  else {
    var my_loop = false;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['overlayOpacity']) {
    var my_overlayOpacity = Drupal.settings.lightbox_conf['slimbox']['overlayOpacity'];
  }
  else {
    var my_overlayOpacity = 0.3;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['overlayFadeDuration']) {
    var my_overlayFadeDuration = Drupal.settings.lightbox_conf['slimbox']['overlayFadeDuration'];
  }
  else {
    var my_overlayFadeDuration = 400;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['resizeDuration']) {
    var my_resizeDuration = Drupal.settings.lightbox_conf['slimbox']['resizeDuration'];
  }
  else {
    var my_resizeDuration = 400;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['resizeEasing']) {
    var my_resizeEasing = Drupal.settings.lightbox_conf['slimbox']['resizeEasing'];
  }
  else {
    var my_resizeEasing = 'swing';
  }
  if (Drupal.settings.lightbox_conf['slimbox']['initialWidth']) {
    var my_initialWidth = Drupal.settings.lightbox_conf['slimbox']['initialWidth'];
  }
  else {
    var my_initialWidth = 250;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['initialHeight']) {
    var my_initialHeight = Drupal.settings.lightbox_conf['slimbox']['initialHeight'];
  }
  else {
    var my_initialHeight = 250;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['imageFadeDuration']) {
    var my_imageFadeDuration = Drupal.settings.lightbox_conf['slimbox']['imageFadeDuration'];
  }
  else {
    var my_imageFadeDuration = 400;
  }
  if (Drupal.settings.lightbox_conf['slimbox']['captionAnimationDuration']) {
    var my_captionAnimationDuration = Drupal.settings.lightbox_conf['slimbox']['captionAnimationDuration'];
  }
  else {
    var my_initialHeight = 400;
  }

  $(".gallery_assist .image-sheet a").slimbox({
        loop: my_loop,
        overlayOpacity: my_overlayOpacity,
        overlayFadeDuration: my_overlayFadeDuration,
        resizeDuration: my_resizeDuration,
        // resizeEasing: my_resizeEasing, // buggy, this parameter produce error by display.
        initialWidth: my_initialWidth,
        initialHeight: my_initialHeight,
        imageFadeDuration: my_imageFadeDuration,
        captionAnimationDuration: my_captionAnimationDuration,
  });
});
