/* $Id:*/

Drupal.behaviors.GacFunctions = function (context) {
 var Jumper = false;
 setToggleColor();
 if (Drupal.settings.gacLinkCA == 2 || Drupal.settings.gacLinkCB == 2) {
  collapse('hide');
  ToggleFunc();
 }
 if (Drupal.settings.gacLinkCA == 3 || Drupal.settings.gacLinkCB == 3) { 
  collapse('show');
  ToggleFunc();
 }
 if (Drupal.settings.gacLinkCA == 1 || Drupal.settings.gacLinkCB == 1) {
  $('#ga-comment-area-form-above').show();
  $('#ga-comment-area-form-below').show();
 }
 if (Drupal.settings.gacPopSA == 1 || Drupal.settings.gacPopSB == 1) {
  collapse('hide'); 
  setPopColor(); 
  ToggleFunc();
  if (Drupal.settings.gacPopSAUP == 1 && Jumper == false) {
    $( "#ga-comment-area-form-above" ).click(function() {
     newPos('above');
    });
   $( "#ga-comment-area-form-below" ).click(function() {
    newPos('below');
   });
  }
 }
 if (Drupal.settings.gacCCA && Drupal.settings.gacCCB) {
  $("#edit-comment-body").counter({ count: Drupal.settings.gacCountA, goal: Drupal.settings.gacGoalA });
  $("#edit-comment-body-1").counter({ count: Drupal.settings.gacCountB, goal: Drupal.settings.gacGoalB });
 }
 if (Drupal.settings.gacCCA && !Drupal.settings.gacCCB) { 
  $("#edit-comment-body").counter({ count: Drupal.settings.gacCountA, goal: Drupal.settings.gacGoalA });
 }
 if (!Drupal.settings.gacCCA && Drupal.settings.gacCCB) { 
  $("#edit-comment-body").counter({ count: Drupal.settings.gacCountB, goal: Drupal.settings.gacGoalB });
 }
 $('#move-top-above').click(function() {
  goToJump('#move-top-below');
 });
 $('#move-top-below').click(function() {
  goToJump('#ga-comments-container');
 });
 function newPos(arg) {
  var newH = $('#ga-comment-area-form-' + arg).height();
  newH = newH + 70;
  if (arg == 'above' && Jumper == false) {
   $('#ga-comment-area-form-wrapper-' + arg + ' .gac-context-link-box').css({ top: '-' + newH + 'px' });
  }
  if (arg == 'below') {
    $('#ga-comment-area-form-wrapper-' + arg + ' .gac-context-link-box').css({ top: '-' + newH + 'px' });
  }
 }
 function collapse(arg) {
  if (arg == 'show') {
   $('#ga-comment-area-form-above').show();
   $('#ga-comment-area-form-below').show();
  }
  if (arg == 'hide') {
   $('#ga-comment-area-form-above').hide();
   $('#ga-comment-area-form-below').hide();
  }
 }
 function setToggleColor() {
  var aColor = $('.get-link-color a').css('color');
  var aColorJ = $('.get-link-color-jump a').css('color');
  if (Drupal.settings.gacLinkCA > 1) {
   $('#ga-comment-form-toggle-above').addClass('toggle-pointer');
   $('#ga-comment-form-toggle-below').addClass('toggle-pointer');
  }
  $('.toggle-pointer').css({color: aColor});
  $('.jump-pointer').css({color: aColorJ});
 }
 function ToggleFunc() {
  $('#ga-comment-form-toggle-above').click(function() {
   if (Drupal.settings.gacPopSAUP == 1 && Jumper == false) {
    newPos('above');
   }
   $('#ga-comment-area-form-above').toggle();
   if (Drupal.settings.gacPopType == 'simple') {
    $(this).toggle();
    $('#ga-comment-cancel-toggle-above').toggle();
   }
  });
  $('#ga-comment-cancel-toggle-above').click(function() {
   $('#ga-comment-area-form-above').toggle();
   if (Drupal.settings.gacPopType == 'simple') {
    $(this).toggle();
    $('#ga-comment-form-toggle-above').toggle();
   }
  });
  $('#ga-comment-form-toggle-below').click(function() {
   if (Drupal.settings.gacPopSAUP == 1) {
    newPos('below');
   }
   $('#ga-comment-area-form-below').toggle();
   if (Drupal.settings.gacPopType == 'simple') {
    $(this).toggle();
    $('#ga-comment-cancel-toggle-below').toggle();
   }
  });
  $('#ga-comment-cancel-toggle-below').click(function() {
   $('#ga-comment-area-form-below').toggle();
   if (Drupal.settings.gacPopType == 'simple') {
    $(this).toggle();
    $('#ga-comment-form-toggle-below').toggle();
   }
  });
 }
 function setPopColor() {
  var bgColor = $('body').css('background-color');
  $('.gac-context-link-box').css({background: bgColor});
 }
 function goToJump(id){
  $('html, body').animate({scrollTop: $(id).offset().top}, 'slow');
  Jumper = true;
  $('#ga-comment-area-form-wrapper-above .gac-context-link-box').css({ top: '0px' });
 }
};