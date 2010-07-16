// $Id: gallery_assist.highslide.theme1.selector.js,v 1.2 2009/10/31 12:00:00 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.highslide.theme1.selector.js');

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
  hs.outlineType = 'rounded-white';
  hs.wrapperClassName = 'controls-in-heading';
  hs.fadeInOut = true;
	hs.dimmingOpacity = myOpacity;
  hs.creditsTarget = '_blank';
  hs.marginLeft = 125;
  hs.marginRight = 125;
  hs.marginTop = 25;
  hs.marginBottom = 35;
	hs.showCredits = myCredits;

  // Add the controlbar
  hs.addSlideshow({
    //slideshowGroup: 'group1',
    interval: 4000,
    repeat: true,
    useControls: true,
    fixedControls: false,
    overlayOptions: {
      opacity: 1,
      position: 'top center', //'bottom center',
      hideOnMouseOut: false,
    }
  });
});
