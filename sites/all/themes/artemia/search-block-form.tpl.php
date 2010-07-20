<?php



  $prefix_string = 'Chercher dans ce site';
  $transl_prefix_string = t($prefix_string). ':';
  $search['new_search_block_form'] = str_replace($transl_prefix_string, "", $search["search_block_form"]);

$search["new_search_block_form"]= str_replace('<div id="edit-search-block-form-1-wrapper" class="form-item">', "", $search["new_search_block_form"]);

$search["new_search_block_form"]= str_replace('</div>', "", $search["new_search_block_form"]);

  print $search["new_search_block_form"];
  print $search["submit"];
  print $search["hidden"];


?>