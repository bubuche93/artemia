<?php include 'page.header.inc'; ?>

<?php if ($help_link): ?>
<div id="helper" class="clear-block">
	  <div class="help-link"><?php if ($help_link) print $help_link; ?></div>
</div>
<? endif; ?>

<div id="header">
	<div class="container">
	<?php if ($logo): ?>
		<div class="logo" class="clear-block">
			<?php print $logo ?>
		</div>
	<?php endif; ?>
		
	<?php if ($search): ?>
		<div class="search" class="clear-block">
			<?php print $search ?>
		</div>
	<?php endif; ?>

		<div class="search" class="clear-block">
	<?php print $infocontact ?>
		</div>


	<div id="menu-firstlevel" class="clearfix">
		<?php print($menu) ?>
	</div>
	</div>
</div>


<div id="carrousel">
<div class="container">
<?php print $carrousel ?>
</div>
</div>

<div id="main">
<div class="container">
<?php print $center ?>
</div>
</div>

<?php include 'page.footer.inc'; ?>
