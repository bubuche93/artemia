// $Id: gallery_assist.interface.slidershow.selector.js,v 1.2 2009/10/18 19:15:52 jcmc Exp $
// drupal_add_js(drupal_get_path('module', 'gallery_assist_lightboxes')  .'/js/gallery_assist.interface.slidershow.selector.js');

$(document).ready(
	function()
	{
		$.slideshow(
			{
				container : 'slideShow1',
				loader: 'images/slideshow_loader.gif',
				linksPosition: 'top',
				linksClass: 'pagelinks',
				linksSeparator : ' | ',
				fadeDuration : 400,
				activeLinkClass: 'activeSlide',
				nextslideClass: 'nextSlide',
				prevslideClass: 'prevSlide',
				captionPosition: 'bottom',
				captionClass: 'slideCaption',
				autoplay: 5,
				random: true,
				images : [
					{
						src: 'images/panda/Dynamic.jpg',
						caption: 'Panda Dynamic'
					},
					{
						src: 'images/panda/Actual.jpg',
						caption: 'Panda Actual'
					},
					{
						src: 'images/panda/Panda.jpg',
						caption: 'Fiat Panda'
					},
					{
						src: 'images/panda/Active.jpg',
						caption: 'Panda Active'
					}
				]
			}
		)
		
		$('a').ToolTip(
			{
				className: 'inputsTooltip',
				position: 'mouse'
			}
		);
	}
);
