// $Id: gallery_assist.highslide.theme2.selector.js,v 1.2 2009/10/31 12:00:00 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.highslide.theme2.selector.js');

$(document).ready(function(){
  var myOpacity = Drupal.settings.lightbox_conf['highslide']['dimmingOpacity'];
  if (myOpacity.length == 0) {
    myOpacity = .8;
  }
  var myCredits = Drupal.settings.lightbox_conf['highslide']['credits'];
  if (myCredits.length == 0) {
    myCredits = true;
  }
  //vars
  hs.graphicsDir = Drupal.settings.lightbox_conf['highslide']['the_path'] + '/graphics/';
  hs.align = 'center';
  hs.transitions = ['expand', 'crossfade'];
  hs.fadeInOut = true;
  hs.dimmingOpacity = Drupal.settings.lightbox_conf['highslide']['dimmingOpacity'];
  hs.outlineType = 'rounded-white';
  hs.captionEval = 'this.thumb.title';
  hs.marginBottom = 105; // make room for the thumbstrip and the controls
  hs.numberPosition = 'caption';
	hs.dimmingOpacity = myOpacity;
	hs.showCredits = myCredits;

  // Add the slideshow providing the controlbar and the thumbstrip
  hs.addSlideshow({
    //slideshowGroup: 'group1',
    interval: 5000,
    repeat: false,
    useControls: true,
    overlayOptions: {
      className: 'text-controls',
      position: 'bottom center',
      relativeTo: 'viewport',
      offsetY: -60
    },
    thumbstrip: {
      position: 'bottom center',
      mode: 'horizontal',
      relativeTo: 'viewport'
    }
  });
});
