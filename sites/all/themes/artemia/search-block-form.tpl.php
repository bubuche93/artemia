<?php
// $Id: search-theme-form.tpl.php,v 1.1 2007/10/31 18:06:38 dries Exp $

/**
* @file search-theme-form.tpl.php
* Default theme implementation for displaying a search form directly into the
* theme layout. Not to be confused with the search block or the search page.
*
* Available variables:
* - $search_form: The complete search form ready for print.
* - $search: Array of keyed search elements. Can be used to print each form
*   element separately.
*
* Default keys within $search:
* - $search['search_theme_form']: Text input area wrapped in a div.
* - $search['submit']: Form submit button.
* - $search['hidden']: Hidden form elements. Used to validate forms when submitted.
*
* Since $search is keyed, a direct print of the form element is possible.
* Modules can add to the search form so it is recommended to check for their
* existance before printing. The default keys will always exist.
*
*   <?php if (isset($search['extra_field'])): ?>
*     <div class="extra-field">
*       <?php print $search['extra_field']; ?>
*     </div>
*   <?php endif; ?>
*
* To check for all available data within $search, use the code below.
*
*   <?php print '<pre>'. check_plain(print_r($search, 1)) .'</pre>'; ?>
*
* @see template_preprocess_search_theme_form()
*/

/* original code outside of <?php ?> tags
<div id="search" class="container-inline">
  <?php print $search_form; ?>
</div>
*/
?>
<?php
//print '<pre>'. check_plain(print_r($search, 1)) .'</pre>';
?>
<div id="search" class="container-inline">
<?php

    // first make a copy of the $search_form array so that we can make our modifications to it and print it out at the end
    $modified_search_array = $search;

    /* remove ' this site' - probably should try more surefire way using </label> */
    $modified_search_array["search_theme_form"] = str_replace("<label for=\"edit-search-theme-form-1\">Search this site: </label>", "<strong>Quick Search </strong>", $modified_search_array["search_theme_form"]);
   
    $modified_search_array["search_theme_form"] = str_replace("<input type=\"text\" maxlength=\"128\" name=\"search_theme_form\" id=\"edit-search-theme-form-1\" size=\"15\" value=\"\" title=\"Enter the terms you wish to search for.\" class=\"form-text\" />", "<input type=\"text\" maxlength=\"128\" name=\"search_theme_form\" id=\"edit-search-theme-form-1\" size=\"30\" value=\"\" title=\"Enter the terms you wish to search for.\" class=\"form-text\" />", $modified_search_array["search_theme_form"]);
   
    $modified_search_array["submit"] = str_replace("<input type=\"submit\" name=\"op\" id=\"edit-submit-1\" value=\"Search\"  class=\"form-submit\" />", "<input type=\"submit\" name=\"op\" id=\"edit-submit-1\" value=\" \" class=\"form-submit\" />", $modified_search_array["submit"]);

    /* Replace button input type with image. I substituted with the method above by changing the value and removing the "Search" text in the button because I used CSS to display the image rather than an image as a submit button (which didn't display the way I wanted it to.)  */
    /*$modified_search_array["submit"] = str_replace('input type="submit"', 'input type="image" src="'. base_path() . path_to_theme() .'/images/magnify.jpg" ', $modified_search_array["submit"]);*/

    foreach($modified_search_array as $printable_value) {
        print $printable_value;
    }
?>
</div>