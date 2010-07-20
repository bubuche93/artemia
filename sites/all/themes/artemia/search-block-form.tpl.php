<?php



 $search["search_block_form"]= str_replace("Chercher dans ce site : ", "", $search["search_block_form"]);
 $search["search_block_form"]= str_replace('<div id="edit-search-block-form-1-wrapper" class="form-item">', "", $search["search_block_form"]);
 $search["search_block_form"]= str_replace('</div>', "", $search["search_block_form"]);

  print $search["search_block_form"];
  print $search["submit"];
  print $search["hidden"];


?>