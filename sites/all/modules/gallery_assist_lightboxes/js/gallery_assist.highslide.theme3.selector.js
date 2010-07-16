// $Id: gallery_assist.highslide.theme3.selector.js,v 1.2 2009/10/31 12:00:00 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.highslide.theme3.selector.js');

$(document).ready(function(){
  var myOpacity = Drupal.settings.lightbox_conf['highslide']['dimmingOpacity'];
  if (myOpacity.length == 0) {
    myOpacity = .8;
  }
  var myWith = Drupal.settings.lightbox_conf['highslide']['width'];
  if (myWith.length == 0) {
    myWith = 600;
  }
  var myHeight = Drupal.settings.lightbox_conf['highslide']['height'];
  if (myHeight.length == 0) {
    myHeight = 400;
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
	hs.outlineType = 'rounded-black';
  hs.wrapperClassName = 'controls-in-heading';
	hs.headingEval = 'this.a.title';
	hs.numberPosition = 'heading';
	hs.useBox = true;
	hs.width = myWith;
	hs.height = myHeight;
	hs.dimmingOpacity = myOpacity;
	hs.showCredits = myCredits;

	// Add the slideshow providing the controlbar and the thumbstrip
	hs.addSlideshow({
		//slideshowGroup: 'group1',
		interval: 5000,
		repeat: false,
		useControls: true,
		fixedControls: 'fit',
		overlayOptions: {
			position: 'top right',
			offsetX: 150,
			offsetY: -9,
		},
		thumbstrip: {
			position: 'rightpanel',
			mode: 'float',
			relativeTo: 'expander',
			width: '100%'
		}
	});

	//Make all images animate to the one visible thumbnail
	var miniGalleryOptions1 = {
		thumbnailId: 'thumb1'
	}
});
