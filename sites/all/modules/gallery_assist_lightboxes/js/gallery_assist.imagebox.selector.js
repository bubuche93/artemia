// $Id: gallery_assist.interface.imagebox.selector.js,v 1.2 2009/10/18 19:15:52 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.interface.imagebox.selector.js');

$(document).ready(
	function()
	{
		$.ImageBox.init(
			{
				loaderSRC: 'loading.gif',
				closeHTML: '<img src="sites/all/modules/contrib/gallery_assist_lightboxes/lightboxes/interface/close.jpg" />'
			}
		);
	}
);
