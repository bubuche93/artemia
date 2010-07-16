// $Id: gallery_assist.lightbox2.selector.js,v 1.0 2010/01/10 23:36:02 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.lightbox2.selector.js');

// Initialize the lightbox.
Drupal.behaviors.initLightbox4ga = function (context) {
  $('.gallery-container a.prepared4lb2:not(.lightbox-processed)', context).addClass('lightbox-processed').each(function() {
    $('.lightboxAutoModal').triggerHandler('click');
    return false; // Break the each loop.
  });
};
