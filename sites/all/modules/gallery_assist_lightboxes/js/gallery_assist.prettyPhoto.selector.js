// $Id: gallery_assist.prettyPhoto.selector.js,v 1.1.2.8 2010/03/17 02:30:42 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.prettyPhoto.selector.js');

$(document).ready(function(){

  var mytheme = Drupal.settings.lightbox_conf['prettyPhoto']['theme'];
  var mycounter_separator_label = Drupal.settings.lightbox_conf['prettyPhoto']['counter_separator_label'];
  var myanimationSpeed = Drupal.settings.lightbox_conf['prettyPhoto']['animationSpeed'];
  var myopacity = Drupal.settings.lightbox_conf['prettyPhoto']['opacity'];
  //var mypadding = Drupal.settings.lightbox_conf['prettyPhoto']['padding'];
  var myshowTitle = Drupal.settings.lightbox_conf['prettyPhoto']['showTitle'];
  var myallowresize = Drupal.settings.lightbox_conf['prettyPhoto']['allowresize'];
  var myModal = Drupal.settings.lightbox_conf['prettyPhoto']['modal'];

  if (mytheme.length == 0) {
    mytheme = 'dark_square';
  }
  if (mycounter_separator_label.length == 0) {
    mycounter_separator_label = '/';
  }
  if (myanimationSpeed.length == 0) {
    myanimationSpeed = 'normal';
  }
  if (myopacity.length == 0) {
    myopacity = 0.9;
  }      
  /*if (mypadding.length == 0) {
    mypadding = '40';
  }*/
  if (myshowTitle.length == 0) {
    myshowTitle = false;
  } else {
    myshowTitle = parseInt(myshowTitle);
  }
  if (myallowresize.length == 0) {
    myallowresize = true;
  } else {
    myallowresize = parseInt(myallowresize);
  }
  if (myModal.length == 0) {
    myModal = true;
  } else {
    myModal = myModal;
  }

  $("a[rel^='prettyPhoto']").prettyPhoto({
    theme: mytheme,
			animationSpeed: myanimationSpeed, /* fast/slow/normal */
			opacity: myopacity, /* Value between 0 and 1 */
			showTitle: myshowTitle, /* true/false */
			counter_separator_label: mycounter_separator_label, /* The separator for the gallery counter 1 "of" 2 */
			theme: mytheme, /* light_rounded / dark_rounded / light_square / dark_square / facebook */
			allowresize: myallowresize, /* true/false */
			default_width: 500,
			default_height: 344,
      modal: myModal, /* If set to true, only the close button will close the window */
      //padding: parseInt(mypadding),
  });
});
