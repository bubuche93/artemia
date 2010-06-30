*******************************************************************************/
var expand = function(){
   var exp = {
      classe : '.expand',
      off    : 'off',
      on     : 'on'
   };
/**************************************/
   if(jQuery(exp.classe).length < 1){
      return;
   }
/**************************************/
   var init = function(){
      jQuery(exp.classe).each(function(){   
         jQuery(this).addClass(exp.off);
      });
   }();
   jQuery(exp.classe).each(function(){
      jQuery(this).find('a[href^="#"]:first').click(function(e){
         var that = jQuery(this);
         var ancre = that.attr('href');
         e.preventDefault();
         that.blur();
         if(that.parents(exp.classe).attr('class').search(exp.off) != -1){
            jQuery(ancre).slideDown(function(){
               that.parents(exp.classe).removeClass(exp.off).addClass(exp.on);
            });
         }else{
            jQuery(ancre).slideUp(function(){
               that.parents(exp.classe).addClass(exp.off).removeClass(exp.on);
            });
         }
      });
   });
};
/*