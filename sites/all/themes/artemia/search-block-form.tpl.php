<?php

/**
* Override of the search block form
*
*/


$search["search_block_form"]= str_replace('<label for="edit-search-block-form-1">', '<input type="hidden" name="', $search["search_block_form"]);
$search["search_block_form"]= str_replace('</label>', '">', $search["search_block_form"]);

$search["search_block_form"]= str_replace('<div id="edit-search-block-form-1-wrapper" class="form-item">', "", $search["search_block_form"]);

$search["search_block_form"]= str_replace('</div>', "", $search["search_block_form"]);

$search["search_block_form"]= str_replace('id="edit-search-block-form-1"', 'id="edit-search-block-form-1" class="inputtextsearch" onblur="setTimeout(\'closeResults()\',2000); if (this.value == \'\') {this.value = \'\';}"  onfocus="if (this.value == \'Search...\') {this.value = \'\';}" ', $search["search_block_form"]);

  print $search["search_block_form"];
  print $search["submit"];
  print $search["hidden"];


?>
</div>