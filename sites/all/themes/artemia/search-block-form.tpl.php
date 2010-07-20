  <?php

  /* remove ' this site' - probably should try more surefire way using </label> */
  $search["search_theme_form"]= str_replace(" this site", "", $search["search_theme_form"]);

  print $search["search_theme_form"];
  print $search["submit"];
  print $search["hidden"]; 
  
   ?>