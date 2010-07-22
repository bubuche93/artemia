<div id="block-user-0" class="clear-block block block-user test">
	<form action="<?php echo $_SERVER['REDIRECT_URL']; ?>?destination=<?php echo urlencode($_SERVER['REDIRECT_URL']); ?>"  accept-charset="UTF-8" method="post" id="user-login-form">
  
		<div><div class="form-item" id="edit-name-wrapper">
<label for="edit-name">Username: <span class="form-required" title="This field is required.">*</span></label>
<input type="text" maxlength="60" name="name" id="edit-name" size="15" value="" class="form-text required" />
</div>
<div class="form-item" id="edit-pass-wrapper">
<label for="edit-pass">Password: <span class="form-required" title="This field is required.">*</span></label>
<input type="password" name="pass" id="edit-pass"  maxlength="60"  size="15"  class="form-text required" />
</div>
<input type="submit" name="op" id="edit-submit" value="Log in"  class="form-submit" />
<div class="item-list"><ul><li class="first"><a href="/user/register" title="Create a new user account.">Create new account</a></li>
<li class="last"><a href="/user/password" title="Request new password via e-mail.">Request new password</a></li>
</ul></div><input type="hidden" name="form_build_id" id="<?php form_clean_id('edit-'. drupal_get_token()  .'-form-token') ?>" value="<?php drupal_get_token() ?>"  />
<input type="hidden" name="form_id" id="edit-user-login-block" value="user_login_block"  />
</div></form>
</div>
</div>