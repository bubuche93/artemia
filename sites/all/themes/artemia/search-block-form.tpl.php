<div id="search" class="container-inline">
  <?php

  /* remove ' this site' - probably should try more surefire way using </label> */
  $search["search_theme_form"]= str_replace(" this site", "", $search["search_theme_form"]);

  /* Replace button input type with image  */
  $search["submit"]=str_replace('input type="submit"', 'input type="image" src="yoursite-path-to-your-search.png" ', $search["submit"]);

  print $search["search_theme_form"];
  print $search["submit"];
  print $search["hidden"]; 
  
   ?>
</div>