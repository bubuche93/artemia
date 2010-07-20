<?php
  /* Remove "Search this site: " */
  $prefix_string = 'Search this site';
  $transl_prefix_string = t($prefix_string). ':';
  $search['new_search_theme_form'] = str_replace($transl_prefix_string, "", $search["search_theme_form"]);

  /* Replace button input type with image */
  // $search["submit"]=str_replace('input type="submit"', 'input type="image" src="yoursite-path-to-your-search.png" ', $search["submit"]);

  /* "Search" button with "Find" */
  //$search['submit'] = str_replace('value="Search"', 'value="Find"', $search["submit"]);

  print $search['new_search_theme_form'];
  print $search['submit'];
  print $search['hidden'];

  
?>