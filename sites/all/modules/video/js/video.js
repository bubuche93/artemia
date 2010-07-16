// $Id: video.js,v 1.1.2.5 2010/07/09 01:20:04 heshanmw Exp $

/**
 * @file
 * Adds some show/hide to the admin form to make the UXP easier.
 *
 */

$(document).ready(function() {
	//lets see if we have any jmedia movies
	if($.fn.media) {
		$('.jmedia').media();
	}
	
	video_hide_all_options();
	$("input[name='vid_convertor']").change(function() {
		video_hide_all_options();
	});
	
	$('.video_select').each(function() {
		var ext = $(this).attr('rel');
		$('select', this).change(function() {
			if($(this).val() == 'video_play_flv') {
				$('#flv_player_'+ext).show();
			} else {
				$('#flv_player_'+ext).hide();
			}
		});
		if($('select', this).val() == 'video_play_flv') {
			$('#flv_player_'+ext).show();
		}
	});
	
	if(Drupal.settings.video) {
		$.fn.media.defaults.flvPlayer = Drupal.settings.video.flvplayer;

	}
	
	//lets setup our colorbox videos
	$('.video-box').each(function() {
		var url = $(this).attr('href');
		var data = $(this).metadata();
		var width = data.width;
		var height= data.height;
		var player = Drupal.settings.video.player; //player can be either jwplayer or flowplayer.
		$(this).colorbox({
			html: '<a id="video-overlay" href="'+url+'" style="height:'+height+'; width:'+width+'; display: block;"></a>',
			onComplete:function() {
			  if(player == 'flowplayer') {
			    flowplayer("video-overlay", Drupal.settings.video.flvplayer, {
			    	clip: {
			    		autoPlay: Drupal.settings.video.autoplay,
			    		autoBuffering: Drupal.settings.video.autobuffer
			        }
			    });
			  } else {
				$('#video-overlay').media({ flashvars: {autostart: Drupal.settings.video.autoplay}, width:width, height:height }); 
			  }
			}
		});
	});
});

function video_hide_all_options() {
	$("input[name='vid_convertor']").each(function() {
		var id = $(this).val();
	    $('#'+id).hide();
		if ($(this).is(':checked')) {
			$('#' + id).show();
		}			
	});
}

function videoftp_thumbnail_change() {
    // Add handlers for the video thumbnail radio buttons to update the large thumbnail onchange.
	$(".video-thumbnails input").each(function() {
		var path = $(this).val();
		if($(this).is(':checked')) {
			var holder = $(this).attr('rel');
			$('.'+holder+' img').attr('src', Drupal.settings.basePath + path);
		}
	});

}
