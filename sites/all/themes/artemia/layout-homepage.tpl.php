<?php include 'page.header.inc'; ?>

<?php if ($help_link): ?>
<div id="helper" class="clear-block">
	  <div class="help-link"><?php if ($help_link) print $help_link; ?></div>
</div>
<? endif; ?>

<div id="header">
	<div class="container header">
	
		<div class="logo clearfix span-4">
			<img src="<?php print $logo ?>">
		</div>
		
		<div class="search clearfix prepend-14 span-6 last">
			<?php print $search ?>
		</div>

		<div class="infocontact clearfix prepend-14 span-6 last">
			<?php print $infocontact ?>
		</div>

		<div id="menu-firstlevel" class="clearfix span-24">
			<?php print($menu) ?>
		</div>
	</div>
</div>


<div id="carrousel">
<div class="container carrousel">
<?php print $carrousel ?>
</div>
</div>

<div id="main">
<div class="container main">
<?php print $center ?>
</div>
</div>

<?php include 'page.footer.inc'; ?>
