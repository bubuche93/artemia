<?php

$search["search_theme_form"]=str_replace('value=""', 'value="Search..." onblur="setTimeout(\'closeResults()\',2000); if (this.value == \'\') {this.value = \'\';}"  onfocus="if (this.value == \'Search...\') {this.value = \'\';}" ', $search["search_theme_form"]);

  
?>