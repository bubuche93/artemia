<?php
  /* remove ' this site' - probably should try more surefire way using </label> */
  $search["search_block_form"]= str_replace("Chercher dans ce site : ", "", $search["search_block_form"]);

  /* Replace button input type with image  */
  $search["submit"]=str_replace('input type="submit"', 'input type="image" src="sites/all/themes/artemia/images/logo.png" ', $search["submit"]);

  print $search["search_block_form"];
  print $search["submit"];
  print $search["hidden"];
 
  
?>