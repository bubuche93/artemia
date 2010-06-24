<?php
// $Id: export.inc,v 1.1.2.15.2.24.2.8 2010/03/26 09:15:42 goba Exp $

/**
 * @file
 *   Gettext export for localization community.
 */

/**
 * User interface for the translation export screen.
 */
function l10n_community_export_page($uri = NULL, $langcode = NULL) {
  $breadcrumb = array(
    l(t('Home'), NULL),
    l(t('Translate'), 'translate'),
  );

  // Include filters just so if user wants to quickly go back to translate or
  // browse forms, they can do that without loosing their filters.
  $filters = l10n_community_build_filter_values($_GET);
  l10n_community_add_url_modifiers($langcode, $filters);
  drupal_add_js(drupal_get_path('module', 'l10n_community') .'/l10n_community.js');

  // Set a matching title with the translation page.
  if (isset($langcode)) {
    $language = l10n_community_get_language($langcode);
    drupal_set_title(t('Export @language translations', array('@language' => $language->name)));
    $breadcrumb[] = l(t('Explore languages'), 'translate/languages');
    $breadcrumb[] = l($language->name, 'translate/languages/'. $langcode);
  }
  else {
    $project = l10n_server_get_projects(array('uri' => $uri));
    drupal_set_title(t('Export @project translation templates', array('@project' => $project->title)));
    $breadcrumb[] = l(t('Explore projects'), 'translate/projects');
    $breadcrumb[] = l($project->title, 'translate/projects/'. $project->uri);
  }
  // Add missing breadcrumb.
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('l10n_community_export_form', $uri, $langcode);
}

/**
 * Translation export form. This is multi-step with the project selection.
 */
function l10n_community_export_form(&$form_state, $uri = NULL, $langcode = NULL) {

  // When project was preset from path, disable the selector.
  $hide_projects = isset($uri);

  // Set initial form state, so we can consistently use that.
  $projects = l10n_server_get_projects();
  if (empty($form_state['values'])) {
    $form_state['values'] = array(
      'project' => isset($projects[$uri]) ? $projects[$uri]->title : (isset($_GET['project']) && isset($projects[$_GET['project']]) ? $projects[$_GET['project']]->title : ''),
      'release' => isset($_GET['release']) ? $_GET['release'] : 'all',
    );
  }

  $form = array();
  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Source data'),
  );
  if ($hide_projects) {
    // This only happens if we have a project to export via $uri.
    // In this case, $uri is already verified via the menu system.
    $form['data']['project'] = array(
      '#type' => 'value',
      '#value' => $form_state['values']['project'],
    );
  }
  else {
    $form['data']['project'] = array(
      '#title' => t('Project'),
      '#required' => TRUE,
      '#default_value' => $form_state['values']['project'],
      '#ahah' => array(
        // 'event' missing, Drupal will apply the per-#type defaults.
        'path' => 'translate/releases-ahah',
        'wrapper' => 'l10n-server-releases',
        'effect' => 'fade',
      ),
    );
    if (($count = count($projects)) <= 30) {
      // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
      $form['data']['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
      $form['data']['project']['#options'] = array();
      foreach ($projects as $project) {
        // Title used to conform to the autocomplete behavior.
        $form['data']['project']['#options'][$project->title] = $project->title;
      }
    }
    else {
      // Autocomplete field for more then 30 projects.
      $form['data']['project'] += array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'translate/project-autocomplete',
      );
    }
  }

  $release_options = array('all' => t('All releases merged'));
  if (!empty($form_state['values']['project'])) {
    $selected_project = $projects[l10n_community_project_uri_by_title($form_state['values']['project'])];
    $releases = l10n_server_get_releases($selected_project->uri);
    foreach ($releases as $rid => $this_release) {
      $release_options[$rid] = t('@release only', array('@release' => $this_release->title));
    }
  }
  if (!isset($release_options[$form_state['values']['release']])) {
    $form_state['values']['release'] = 'all';
  }
  $form['data']['release'] = array(
    '#title' => t('Release'),
    '#required' => TRUE,
    '#type' => 'select',
    '#options' => $release_options,
    '#default_value' => $form_state['values']['release'],
    // Wrapper used to replace release options via AHAH.
    '#prefix' => '<div id="l10n-server-releases">',
    '#suffix' => '</div>',
  );

  if (isset($langcode)) {
    $form['langcode'] = array(
      '#type' => 'value',
      '#value' => $langcode,
    );

    $form['format'] = array(
      '#type' => 'fieldset',
      '#title' => t('Format'),
    );
    // Only include the type selector if we are not presenting
    // a template export page (which does not have a language).
    $form['format']['type'] = array(
      '#title' => t('Type'),
      '#type' => 'radios',
      '#options' => array('translation' => t('Include both English originals and translations'), 'template' => t('Just export English originals')),
      '#default_value' => 'translation',
    );
    $form['format']['version'] = array(
      '#title' => t('Packaging'),
      '#type' => 'radios',
      '#options' => array('drupal-6' => t('Drupal 6 core package format (translations directories)'), 'drupal-5' => t('Drupal 5 core package format (for autolocale module)'), 'flat-package' => t('Drupal 5/6 core flat package format (for CVS commit)'), 'all-in-one' => t('All in one file')),
      '#default_value' => 'drupal-6',
    );
    $form['format']['compact'] = array(
      '#title' => t('Verbosity'),
      '#type' => 'radios',
      '#options' => array(0 => t('Verbose files useful for desktop translation'), 1 => t('Compact files optimized for size, not desktop editing')),
      '#default_value' => 0,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );
  return $form;
}

/**
 * Release field is mandatory.
 */
function l10n_community_export_form_validate($form, &$form_state) {
  if (!$project = l10n_community_project_uri_by_title($form_state['values']['project'])) {
    form_set_error('project', t('Invalid project selected.'));
    $form_state['values']['project'] = '';
  }
}

/**
 * Generate translation or template package on the fly based on
 * all details available and return the output via HTTP.
 */
function l10n_community_export_form_submit($form, &$form_state) {

  // This was validated to work in the validation code.
  $uri = !empty($form_state['values']['project']) ? l10n_community_project_uri_by_title($form_state['values']['project']) : NULL;
  if (empty($uri)) {
    form_set_error('project', t('Please select a project first.'));
    return;
  }

  if (empty($form_state['values']['release'])) {
    form_set_error('release', t('Please choose a release or opt to export for all releases.'));
    $form_state['redirect'] = array($_GET['q'], array('project' => $uri));
    return;
  }
  elseif ($form_state['values']['release'] != 'all') {
    $releases = l10n_server_get_releases($uri);
    if (!isset($releases[$form_state['values']['release']])) {
      form_set_error('release', t('Invalid release chosen.'));
      return;
    }
  }

  $language = NULL;
  if (isset($form_state['values']['langcode'])) {
    $languages = l10n_community_get_languages();
    $language = $languages[$form_state['values']['langcode']];
  }
  $type = (isset($form_state['values']['type']) ? $form_state['values']['type'] : 'template');

  // Generate tarball or PO file and get file name.
  $export_result = l10n_community_export(
    $uri,
    ($form_state['values']['release'] == 'all' ? NULL : $form_state['values']['release']),
    $language,
    ($type != 'translation'),
    // If not set (exporting a template for a module), stick to all-in-one.
    isset($form_state['values']['version']) ? $form_state['values']['version'] : 'all-in-one',
    $form_state['values']['compact']
  );

  if (isset($export_result) && is_array($export_result)) {
    // If we got an array back from the export build, tear that into pieces.
    list($mime_type, $file_name, $serve_name) = $export_result;
    // Return compressed archive to user.
    header('Content-Disposition: attachment; filename='. $serve_name);
    header('Content-Type: '. $mime_type);
    echo file_get_contents($file_name);
    unlink($file_name);
    die();
  }
  else {
    // Messages should already be recorded about any build errors.
    return;
  }
}

/**
 * Helper function to store the export string.
 */
function _l10n_community_export_string_files(&$string_files, $uri, $language, $template, $version, $export_string, $compact = FALSE) {
  $po_folder = ($version == 'drupal-5' ? 'po' : 'translations');
  $output = '';

  if (!empty($export_string)) {

    // Location comments are constructed in fileone:1,2,5; filetwo:123,537
    // format, where the numbers represent the line numbers of source
    // occurances in the respective source files.
    $comment = array();
    foreach ($export_string['comment'] as $path => $lines) {
      $comment[] = preg_replace('!(^[^/]+/)!', '', $path) .':'. join(',', $lines);
    }
    $comment = '#: '. join('; ', $comment) ."\n";

    // File location is dependent on export format and string location.
    if ($version == 'all-in-one') {
      // Fold all strings into this one file.
      $filename = 'general';
    }
    elseif ((strpos($comment, '.info') && $uri == 'drupal') || count(array_keys($export_string['comment'])) > 1) {
      // An .info file string in Drupal core (which is folded into
      // general.po, so that later the module screen has all module info
      // translated for the admin). Or appeared in more then one file, so
      // goes to general.po for that reason.

      // Note that some modules like ubercart might not have their
      // root module in the root folder, so this needs to be rethought.
      $filename = ($version != 'flat-package' ? $po_folder .'/' : '') .'general';
    }
    else {
      // Putting into a file named after the specific directory.
      $filename = dirname(preg_replace('!(^[^/]+/)!', '', array_shift(array_keys($export_string['comment']))));
      if (empty($filename) || $filename == '.') {
        $filename = ($version != 'flat-package' ? ($po_folder .'/') : '') .'root';
      }
      elseif ($version != 'flat-package') {
        $filename .= '/'. $po_folder .'/'. str_replace('/', '-', $filename);
      }
      else {
        $filename = str_replace('/', '-', $filename);
      }
    }

    // Temporary hack to make the core exports put the system module
    // files to the right place. See http://drupal.org/node/212594
    // This should be solved more elegantly with a knowledge on the
    // non-module files (non-info file based directories in Drupal 6)
    // and a default location for each project.
    if (!in_array($version, array('flat-package', 'all-in-one')) && ($uri == 'drupal')) {
      $system_files = array(
        $po_folder .'/general',
        "includes/$po_folder/includes",
        "misc/$po_folder/misc"
      );
      if (in_array($filename, $system_files)) {
        $filename = "modules/system/$po_folder/". basename($filename);
      }
    }

    // Append extension.
    $filename .= ($template ? '.pot' : ($version != 'flat-package' ? ('.'. $language->language) : '') .'.po');

    if (strpos($export_string['value'], "\0") !== FALSE) {
      // This is a string with plural variants.
      list($singular, $plural) = explode("\0", $export_string['value']);
      if (!$compact) {
        $output = $comment;
      }
      if (!empty($export_string['context'])) {
        $output .= 'msgctxt '. _l10n_community_export_string($export_string['context']);
      }
      $output .= 'msgid '. _l10n_community_export_string($singular) .'msgid_plural '. _l10n_community_export_string($plural);
      if (!$template && !empty($export_string['translation'])) {
        // Export translations we have.
        foreach (explode("\0", $export_string['translation']) as $id => $value) {
          $output .= 'msgstr['. $id .'] '. _l10n_community_export_string($value);
        }
      }
      elseif (isset($language)) {
        // Empty msgstrs based on plural formula for language. Could be
        // a plural without translation or a template generated for a
        // specific language.
        for ($pi = 0; $pi < $language->plurals; $pi++) {
          $output .= 'msgstr['. $pi .'] ""'."\n";
        }
      }
      else {
        // Translation template without language, assume two msgstrs.
        $output .= 'msgstr[0] ""'."\n";
        $output .= 'msgstr[1] ""'."\n";
      }
    }
    else {
      // Simple string (and possibly translation pair).
      if (!$compact) {
        $output = $comment;
      }
      if (!empty($export_string['context'])) {
        $output .= 'msgctxt '. _l10n_community_export_string($export_string['context']);
      }
      $output .= 'msgid '. _l10n_community_export_string($export_string['value']);
      if (!empty($export_string['translation'])) {
        $output .= 'msgstr '. _l10n_community_export_string($export_string['translation']);
      }
      else {
        $output .= 'msgstr ""'."\n";
      }
    }

    $file_outputs = array($filename);
    if ($export_string['type'] != 2 /* POTX_STRING_RUNTIME */ && $version != 'all-in-one') {
      // Not runtime-only, so make sure we try to have an installer entry.
      $file_outputs[] = ($uri == 'drupal' ?
        (($version == 'flat-package') ?
          ('installer'. ($template ? '.pot' : '.po')) :
          ('profiles/default/'. $po_folder .'/'. ($template ? 'installer.pot' : ($language->language .'.po')))) :
        ($po_folder .'/installer'. ($template ? '.pot' : ('.'. $language->language .'.po')))
      );
      if ($export_string['type'] == 1 /* POTX_STRING_INSTALLER */) {
        // Installer-only, remove runtime entry.
        $file_outputs = array($file_outputs[1]);
      }
    }
    foreach ($file_outputs as $filename) {
      if (!isset($string_files[$filename])) {
        // Prepare new file storage for use.
        $string_files[$filename] = array('file' => '', 'changed' => 0, 'revisions' => array());
      }
      // Add to existing file storage.
      $string_files[$filename]['file'] .= $output;
      if (!$compact) {
        $string_files[$filename]['file'] .= "\n";
      }
      if (!$template) {
        $string_files[$filename]['changed'] = max($string_files[$filename]['changed'], $export_string['changed']);
      }
      $string_files[$filename]['revisions'] = array_unique(array_merge($string_files[$filename]['revisions'], $export_string['revisions']));
    }
  }
}

/**
 * Generates the PO(T) files contents and wrap them in a tarball for a given
 * project.
 *
 * @param $uri
 *   Project URI.
 * @param $release
 *   Release number (rid) to generate tarball for, or NULL to generate
 *   with all releases considered.
 * @param $language
 *   Language object.
 * @param $template
 *   TRUE if templates should be exported, FALSE if translations.
 * @param $version
 *   Version to export with: 'drupal-6', 'drupal-5', 'flat-package' and
 *   'all-in-one'. 'all-in-one' means one .po file, 'flat-package' means no
 *   directory structure generated, the others are suitable packages for the
 *   given Drupal version.
 * @param $compact
 *   A compact export will skip outputting the comments, superfluous
 *   newlines, empty translations and the list of files. TRUE or FALSE.
 * @param $installer
 *   Whether we should only export the translations needed for the installer
 *   and not those needed for the runtime site.
 *
 * @todo
 *   Look into possibly exporting suggestions as fuzzy translations.
 */
function l10n_community_export($uri, $release = NULL, $language = NULL, $template = TRUE, $version = NULL, $compact = FALSE, $installer = FALSE) {
  // l10n_community_requirements() makes sure there is a status
  // error if this is not installed.
  include_once 'Archive/Tar.php';

  $project = l10n_server_get_projects(array('uri' => $uri));
  if ($template) {
    // We are exporting a template explicitly.
    $sql = 'SELECT s.sid, s.value, s.context, f.location, f.revision, l.lineno, l.type FROM {l10n_server_file} f INNER JOIN {l10n_server_line} l ON f.fid = l.fid INNER JOIN {l10n_server_string} s ON l.sid = s.sid WHERE f.pid = %d';
    $sql_args = array($project->pid);
  }
  else {
    // Join differently based on compact method, so we can skip strings without
    // translation for compact method export.
    $translation_join = ($compact) ? 'INNER JOIN' : 'LEFT JOIN';
    // Installer strings are POTX_STRING_INSTALLER or POTX_STRING_BOTH.
    $type_limit = ($installer ? 'AND type IN (0, 1) ' : '');
    // We only export active translations, not suggestions.
    $sql = "SELECT s.sid, s.value, s.context, f.location, f.revision, l.lineno, l.type, t.translation, t.uid_approved, t.time_approved FROM {l10n_server_file} f INNER JOIN {l10n_server_line} l ON f.fid = l.fid INNER JOIN {l10n_server_string} s ON l.sid = s.sid  $translation_join {l10n_server_translation} t ON s.sid = t.sid AND t.language = '%s' AND is_active = 1 AND is_suggestion = 0 $translation_filter WHERE f.pid = %d $type_limit";
    $sql_args = array($language->language, $project->pid);
  }

  if (isset($release)) {
    // Release restriction.
    $sql_args[] = $release;
    $sql .= ' AND f.rid = %d';
    $releases = l10n_server_get_releases($uri);
    $release = $releases[$release];
  }

  // Source strings will be repeated as many times as they appear, so to generate
  // the export file properly, order by the source id.
  $sql .= ' ORDER BY s.sid';

  $result = db_query($sql, $sql_args);
  $previous_sid = 0;
  $export_string = $string_files = array();

  while ($string = db_fetch_object($result)) {
    if ($string->sid != $previous_sid) {
      // New string in the stream.
      _l10n_community_export_string_files($string_files, $uri, $language, $template, $version, $export_string, $compact);

      // Now fill in the new string values.
      $previous_sid = $string->sid;
      $export_string = array(
        'comment'     => array($string->location => array($string->lineno)),
        'value'       => $string->value,
        'context'     => $string->context,
        'translation' => isset($string->translation) ? $string->translation : '',
        'revisions'   => array($string->revision),
        'changed'     => isset($string->time_approved) ? $string->time_approved : 0,
        'type'        => $string->type,
      );
    }
    else {
      // Existing string but with new location information.
      $export_string['comment'][$string->location][] = $string->lineno;
      $export_string['revisions'][] = $string->revision;
      if ($export_string['type'] != 0 && $export_string['type'] != $string->type) {
        // Elevate string type if it is not already 0 (POTX_STRING_BOTH), and
        // the currently found string type is different to the previously found.
        $export_string['type'] = 0;
      }
    }
  }
  if ($previous_sid > 0) {
    // Store the last string.
    _l10n_community_export_string_files($string_files, $uri, $language, $template, $version, $export_string, $compact);
  }

  if (empty($string_files)) {
    // No strings were found.
    if (isset($release)) {
      $message = t('There are no strings in the %release release of %project to export.', array('%project' => $project->title, '%release' => $release->title));
    }
    else {
      $message = t('There are no strings in any releases of %project to export.', array('%project' => $project->title));
    }
    // Message to the user.
    drupal_set_message($message);
    // Message to watchdog for possible automated packaging.
    watchdog('l10n_community', $message);
    return NULL;
  }

  // Generate a 'unique' temporary filename for this package.
  $tempfile = tempnam(file_directory_temp(), 'l10n_community-'. $uri);

  if ($version != 'all-in-one') {
    // Generate tgz file with all files added.
    $tar = new Archive_Tar($tempfile, 'gz');
  }
  foreach ($string_files as $filename => $fileinfo) {
    if (!$compact) {
      if (count($fileinfo['revisions']) == 1) {
        $file_list = '# Generated from file: '. $fileinfo['revisions'][0] ."\n";
      }
      else {
        $file_list = '# Generated from files:'."\n#  ". join("\n#  ", $fileinfo['revisions']) ."\n";
      }
    }
    else {
      $file_list = '';
    }

    $release_title = $project->title .' ('. (isset($release) ? $release->title : 'all releases') .')';
    if (!$template) {
      $header = '# '. $language->name .' translation of '. $release_title ."\n";
      $header .= "# Copyright (c) ". date('Y') .' by the '. $language->name ." translation team\n";
      $header .= $file_list;
      $header .= "#\n";
      $header .= "msgid \"\"\n";
      $header .= "msgstr \"\"\n";
      $header .= "\"Project-Id-Version: ". $release_title ."\\n\"\n";
      $header .= "\"POT-Creation-Date: ". date("Y-m-d H:iO") ."\\n\"\n";
      // Use date placeholder, if we have no date information (no translation here yet).
      $header .= "\"PO-Revision-Date: ". (!empty($fileinfo['changed']) ? date("Y-m-d H:iO", $fileinfo['changed']) : 'YYYY-mm-DD HH:MM+ZZZZ') ."\\n\"\n";
      $header .= "\"Language-Team: ". $language->name ."\\n\"\n";
      $header .= "\"MIME-Version: 1.0\\n\"\n";
      $header .= "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
      $header .= "\"Content-Transfer-Encoding: 8bit\\n\"\n";
      if ($language->formula && $language->plurals) {
        $header .= "\"Plural-Forms: nplurals=". $language->plurals ."; plural=". strtr($language->formula, array('$' => '')) .";\\n\"\n";
      }
    }
    else {
      $language_title = (isset($language) ? $language->name : 'LANGUAGE');
      $header = "# ". $language_title ." translation of ". $release_title ."\n";
      $header .= "# Copyright (c) ". date('Y') ."\n";
      $header .= $file_list;
      $header .= "#\n";
      $header .= "msgid \"\"\n";
      $header .= "msgstr \"\"\n";
      $header .= "\"Project-Id-Version: ". $release_title ."\\n\"\n";
      $header .= "\"POT-Creation-Date: ". date("Y-m-d H:iO") ."\\n\"\n";
      $header .= "\"PO-Revision-Date: YYYY-mm-DD HH:MM+ZZZZ\\n\"\n";
      $header .= "\"Language-Team: ". $language_title ."\\n\"\n";
      $header .= "\"MIME-Version: 1.0\\n\"\n";
      $header .= "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
      $header .= "\"Content-Transfer-Encoding: 8bit\\n\"\n";
      if (isset($language) && $language->formula && $language->plurals) {
        $header .= "\"Plural-Forms: nplurals=". $language->plurals ."; plural=". strtr($language->formula, array('$' => '')) .";\\n\"\n";
      }
      else {
        $header .= "\"Plural-Forms: nplurals=INTEGER; plural=EXPRESSION;\\n\"\n";
      }
    }
    if ($version == 'all-in-one') {
      // Write to file directly. We should only do this once.
      $fh = fopen($tempfile, 'w');
      fwrite($fh, $header ."\n". $fileinfo['file']);
      fclose($fh);
    }
    else {
      // Add file to the tgz.
      $tar->addString($filename, $header ."\n". $fileinfo['file']);
    }
  }

  if ($version == 'all-in-one') {
    // Output a single PO(T) file in this case.
    return array('text/plain', $tempfile, $uri .'-'. (isset($release) ? $release->title : 'all') . (isset($language) ? '-'. $language->language : '') . ($template ? '.pot' : '.po'));
  }
  else {
    // Output a package in this case.
    return array('application/x-compressed', $tempfile, $uri .'-'. (isset($release) ? $release->title : 'all') . (isset($language) ? '-'. $language->language : '') . ($template ? '-templates' : '-translations') .'.tgz');
  }
}

/**
 * Print out a string on multiple lines
 */
function _l10n_community_export_string($str) {
  $stri = addcslashes($str, "\0..\37\\\"");
  $parts = array();

  // Cut text into several lines
  while ($stri != "") {
    $i = strpos($stri, "\\n");
    if ($i === FALSE) {
      $curstr = $stri;
      $stri = "";
    }
    else {
      $curstr = substr($stri, 0, $i + 2);
      $stri = substr($stri, $i + 2);
    }
    $curparts = explode("\n", _l10n_community_export_wrap($curstr, 70));
    $parts = array_merge($parts, $curparts);
  }

  // Multiline string
  if (count($parts) > 1) {
    return "\"\"\n\"". implode("\"\n\"", $parts) ."\"\n";
  }
  // Single line string
  elseif (count($parts) == 1) {
    return "\"$parts[0]\"\n";
  }
  // No translation
  else {
    return "\"\"\n";
  }
}

/**
 * Custom word wrapping for Portable Object (Template) files.
 */
function _l10n_community_export_wrap($str, $len) {
  $words = explode(' ', $str);
  $ret = array();

  $cur = "";
  $nstr = 1;
  while (count($words)) {
    $word = array_shift($words);
    if ($nstr) {
      $cur = $word;
      $nstr = 0;
    }
    elseif (strlen("$cur $word") > $len) {
      $ret[] = $cur ." ";
      $cur = $word;
    }
    else {
      $cur = "$cur $word";
    }
  }
  $ret[] = $cur;

  return implode("\n", $ret);
}
