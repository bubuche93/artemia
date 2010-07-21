<?php

/**
* Override of the search block form
*
*/


$search["search_block_form"]= str_replace('<label for="edit-search-block-form-1">', '<input type="hidden" name="', $search["search_block_form"]);
$search["search_block_form"]= str_replace('</label>', '">', $search["search_block_form"]);

$search["search_block_form"]= str_replace('<div id="edit-search-block-form-1-wrapper" class="form-item">', "", $search["search_block_form"]);

$search["search_block_form"]= str_replace('</div>', "", $search["search_block_form"]);

$search["search_block_form"]= str_replace('value="" ', 'value="Entrez votre recherche" ', $search["search_block_form"]);


$search["search_block_form"]= str_replace('id="edit-search-block-form-1"', 'id="edit-search-block-form-1" class="inputtextsearch" onfocus="if (this.value == \'Entrez votre recherche\') {this.value = \'\';}" ', $search["search_block_form"]);

  print $search["search_block_form"];
  print $search["submit"];
  print $search["hidden"];


?>
</div>