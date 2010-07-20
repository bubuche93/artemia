<?php


  //    * Remove "Search this site: "  *    ////
  $search["search_theme_form"]= str_replace("Chercher dans ce site : ", "", $search["search_theme_form"]);
 
 
  //    * "Search" button with "Go!" *      ////
  $search["submit"]=str_replace('value="Recherche"', 'value="Go!"', $search["submit"]);

  print $search["search_theme_form"];
  print $search["submit"];
  print $search["hidden"];


?>