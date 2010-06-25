<?php include 'page.header.inc'; ?>

<div id='branding' class='clear-block'>
  <?php if ($admin_link): ?><div class="admin-link"><?php print $admin_link; ?></div><?php endif; ?>
  <?php if ($site_name): ?><h1 class='site-name'><?php print $site_name ?></h1><?php endif; ?>
  <?php if ($site_slogan): ?><div class='site-slogan'><?php print $site_slogan ?></div><?php endif; ?>
  <div class="help-link"><?php if ($help_link) print $help_link; ?></div>
</div>

<?php if ($header): ?>
	<div id="header">
		
		<?php if ($logo): ?>
			<div class="logo">
				<?php print $logo ?>
			</div>
		<?php endif; ?>
		
		<?php if ($search): ?>
			<div class="search">
				<?php print $search ?>
			</div>
		<?php endif; ?>
		
		<?php print $header ?>
	</div>
<?php endif; ?>

<?php if ($menu): ?>
	<div id="menu-firstlevel">
		<?php print($menu) ?>
		<?php if (isset($primary_links)) : ?>
    		<?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
  		<?php endif; ?>
	</div>
<?php endif; ?>

<div id="main">
<?php if ($center): ?>
	<div id="center" class="clear-block">
		<?php print $content ?>
	</div>
<?php endif; ?>
</div>


<?php include 'page.footer.inc'; ?>