<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
<title><?php print $head_title ?></title>
</head>

<body<?php print phptemplate_body_attributes($is_front, $layout); ?>>

<?php if ($help_link): ?>
<div id="helper" class="clear-block">
	  <div class="help-link"><?php if ($help_link) print $help_link; ?></div>
</div>
<? endif; ?>

<div id="header">
			<?php print $header ?>
</div>

<div id="main">
	<div class="container main">
		<?php print $center ?>
	</div>
</div>

<?php include 'page.footer.inc'; ?>
