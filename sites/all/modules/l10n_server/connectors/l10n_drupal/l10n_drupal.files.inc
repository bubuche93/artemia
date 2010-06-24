<?php
// $Id: l10n_drupal.files.inc,v 1.1.2.1 2010/03/31 13:53:00 goba Exp $

// = Project synchronization ===================================================

/**
 * Scans files of a project and release picked.
 *
 * @param $automated
 *   TRUE if the execution was automated, so user feedback should
 *   not be provided. FALSE otherwise.
 */
function l10n_drupal_files_scan($source = NULL, $automated = FALSE) {

  // We look for projects in the working directory.
  $workdir = variable_get('l10n_server_connector_l10n_drupal_files_directory', '');

  if (!is_dir($workdir)) {
    drupal_set_message(t('The configured directory (%workdir) cannot be found. <a href="@configure">Check your configuration</a>.', array('%workdir' => $workdir, '@configure' => url('admin/l10n_server/connectors/config/l10n_drupal/files'))));
  }
  else {
    // Packages are always .tar.gz files.
    $files = file_scan_directory($workdir, '.tar.gz$');
    if (count($files)) {
      foreach ($files as $path => $file) {

        if (!l10n_drupal_is_supported_version($path)) {
          // Skip files for unsupported versions.
          continue;
        }

        // Get rid of $workdir prefix on file names, eg.
        // files/Drupal/drupal-4.6.7.tar.gz or
        // files/Ubercart/ubercart-5.x-1.0-alpha8.tar.gz.
        $path = $package = trim(preg_replace('!(^'. preg_quote($workdir, '!') .')(.+)\.tar\.gz!', '\2', $path), '/');

        $project_title = '';
        if (strpos($path, '/')) {
          // We have a slash, so this package is in a subfolder.
          // Eg. Drupal/drupal-4.6.7 or Ubercart/ubercart-5.x-1.0-alpha8.
          // Grab the directory name as project title.
          list($project_title, $package) = explode('/', $path);
        }
        if (strpos($package, '-')) {
          // Only remaining are the project uri and release,
          // eg. drupal-4.6.7 or ubercart-5.x-1.0-alpha8.
          list($project_uri, $release_version) = explode('-', $package, 2);

          l10n_drupal_save_data($project_uri, ($project_title ? $project_title : $project_uri), $release_version, $path .'.tar.gz', filemtime($file->filename));
        }
        else {
          // File name not formatted properly.
          $result['error'] = t('File name should have project codename and version number included separated with hyphen, such as drupal-5.2.tar.gz.');
        }
      }
    }
  }

  $user_feedback = FALSE;
  $results = db_query_range("SELECT * FROM {l10n_server_release} WHERE pid IN (SELECT pid FROM {l10n_server_project} WHERE connector_module = 'l10n_drupal_files' AND status = 1) ORDER BY last_parsed ASC", 0, variable_get('l10n_server_connector_l10n_drupal_files_limit', 1));
  while ($release = db_fetch_object($results)) {

    // Only parse file if something changed since we last parsed it.
    $file_name = $workdir .'/'. $release->download_link;

    if (file_exists($file_name)) {
      if (filemtime($file_name) > $release->last_parsed) {
        $result = l10n_drupal_parse_package($file_name, $release);

        // User feedback, if not automated. Log messages are already done.
        if (isset($result['error']) && !$automated) {
          $user_feedback = TRUE;
          drupal_set_message($result['error'], 'error');
        }
        elseif (isset($result['message']) && !$automated) {
          $user_feedback = TRUE;
          drupal_set_message($result['message']);
        }
      }
      else {
        if (!$automated) {
          $user_feedback = TRUE;
          drupal_set_message(t('@release was already parsed, no need to scan again.', array('@release' => $release->download_link)));
        }
      }
    }
    // Hackish update of last parsed time so other tarballs will get into the queue too.
    // @todo: work on something better for this.
    db_query("UPDATE {l10n_server_release} SET last_parsed = %d WHERE rid = %d", time(), $release->rid);
  }
  if (!$automated && !$user_feedback) {
    drupal_set_message(t('No (new) local Drupal files found to scan in %workdir.', array('%workdir' => $workdir)));
  }

  // Ensure that a Drupal page will be displayed with the messages.
  return '';
}

/**
 * Data save callback for local project and release mappings.
 *
 * @param $project_uri
 *   Short project URI, the first part of file names, eg. 'drupal', 'og'.
 * @param $project_title
 *   Project title, effectively the directory name under which this release
 *   was found. Characters allowed in directory names are allowed here.
 * @param $release_version
 *   Version identified string, such as '5.2' or '5.x-1.x-dev'.
 * @param $filepath
 *   Path to file (without the working directory name).
 * @param filedate
 *   Modification date of the package file.
 */
function l10n_drupal_save_data($project_uri, $project_title, $release_version, $filepath, $filedate) {
  // Save project information first.
  if ($existing_project = db_fetch_object(db_query("SELECT * FROM {l10n_server_project} WHERE uri = '%s'", $project_uri))) {
    if ($existing_project->connector_module == 'l10n_drupal_files') {
      if ($existing_project->status == 0) {
        // Skip this project if it is disabled.
        return;
      }
      // No need to update home_link here, as uri is not changed, and we base home_link on the uri.
      db_query("UPDATE {l10n_server_project} SET title = '%s' WHERE uri = '%s' AND connector_module = 'l10n_drupal_files'", $project_title, $project_uri);
    }
    else {
      // Log error on existing project with another connector and skip the rest of this function.
      $t_args = array('%uri' => $project_uri, '%other_connector' => $existing_project->connector_module, '%this_connector' => 'l10n_drupal_files');
      watchdog('l10n_drupal', 'An existing project under the URI %uri is already handled by %other_connector. Not possible to add it with %this_connector.', $t_args);
      drupal_set_message(t('An existing project under the URI %uri is already handled by %other_connector. Not possible to add it with %this_connector.', $t_args), 'error');
      return;
    }
  }
  else {
    db_query(
      "INSERT INTO {l10n_server_project} (uri, title, last_parsed, home_link, connector_module, status) VALUES ('%s', '%s', %d, '%s', '%s', %d)",
      $project_uri, $project_title, 0, (variable_get('l10n_server_connector_l10n_drupal_files_drupal_home', 0) ? ('http://drupal.org/project/'. $project_uri) : ''), 'l10n_drupal_files', 1
    );
  }

  // Grab updated project data and save release information.
  $project = db_fetch_object(db_query("SELECT * FROM {l10n_server_project} WHERE uri = '%s' AND connector_module = 'l10n_drupal_files'", $project_uri));
  if ($existing_release = db_fetch_object(db_query("SELECT * FROM {l10n_server_release} WHERE pid = %d AND title = '%s'", $project->pid, $release_version))) {
    if ($filedate != $existing_release->file_date) {
      // New file with possibly new date for the same release (dev snapshot
      // probably), so update data, set parsed date to past.
      db_query("UPDATE {l10n_server_release} SET download_link = '%s', file_date = %d, last_parsed = 0 WHERE rid = %d", $filepath, $filedate, $existing_release->rid);
    }
  }
  else {
    // No such release recorded yet.
    db_query("INSERT INTO {l10n_server_release} (pid, title, download_link, file_date, last_parsed) VALUES (%d, '%s', '%s', %d, 0)", $project->pid, $release_version, $filepath, $filedate);
  }
}

// = File extraction and parsing ===============================================

/**
 * Parses contents of a specific local package file.
 *
 * @param $package_file
 *   Path to the package file to be extracted and parsed.
 * @param $release
 *   Release object.
 */
function l10n_drupal_parse_package($package_file, $release) {
  if (!ini_get('safe_mode')) {
    // This could take a long time.
    @set_time_limit(0);
  }
  $error = $message = '';

  // l10n_drupal_requirements() makes sure there is a status
  // error if this is not installed.
  include_once 'Archive/Tar.php';

  // Potx module is already a dependency.
  include_once drupal_get_path('module', 'potx') .'/potx.inc';

  // Set up status messages if not in automated mode.
  potx_status('set', POTX_STATUS_MESSAGE);

  // Generate temp folder to extract the tarball. tempnam() creates a regular
  // file, thus reserving the name. But we need a directory. Because $temp_name
  // is unique, we can append '_dir', and it will still be unique.
  $temp_name = tempnam(file_directory_temp(), 'l10n_drupal_');
  $temp_path = $temp_name .'_dir';

  // Nothing to do if the file is not there or the extraction folder is taken.
  if (!file_exists($package_file)) {
    $error = t('Package to parse (%file) does not exist.', array('%file' => $package_file));
  }
  elseif (is_dir($temp_path)) {
    $error = t('Temporary directory %path already exists.', array('%path' => $temp_path));
  }

  // Extract the local file to the temporary directory.
  else {
    $obj = new Archive_Tar($package_file);
    if (!$obj->extract($temp_path)) {
      $error = t('Error on untaring %filename file.', array('%filename' => $package_file));

      // Delete the files extracted from broken archive.
      l10n_drupal_rmdir_recursive($temp_path);
    }
    else {
      // Get all source files and save strings with our callback for this release.
      $files = _potx_explore_dir($temp_path);
      l10n_drupal_save_file(array($release->pid, $release->rid));
      $version = l10n_drupal_detect_major_version($package_file);
      foreach ($files as $name) {
        _potx_process_file($name, strlen($temp_path) + 1, 'l10n_drupal_save_string', 'l10n_drupal_save_file', $version);
      }

      // Delete the extracted files.
      l10n_drupal_rmdir_recursive($temp_path);

      // Record changes of the scanned project in the database.
      $message = t('Contents of %filename have been scanned.', array('%filename' => $package_file));

      // Parsed this releases files.
      db_query("UPDATE {l10n_server_release} SET last_parsed = %d WHERE rid = %d", time(), $release->rid);

      // Update error list for this release. Although the errors are related to
      // files, we are not interested in the fine details, the file names are in
      // the error messages as text. We assume no other messages are added while
      // importing, so we can safely use drupal_get_message() to grab our errors.
      db_query("DELETE FROM {l10n_server_error} WHERE rid = %d", $release->rid);
      $messages = drupal_get_messages('error');
      if (isset($messages['error']) && is_array($messages['error'])) {
        foreach ($messages['error'] as $error_message) {
          db_query("INSERT INTO {l10n_server_error} (rid, value) VALUES (%d, '%s')", $release->rid, $error_message);
        }
      }
    }
  }

  // Remove temporary file we used to reserve the name for the directory.
  unlink($temp_name);

  $return = array();
  if ($error) {
    // WARNING: We should not do this reuse in the 6.x port, or otherwise
    // the extractor cannot pick the watchdog() strings up.
    watchdog('l10n_drupal', $error, WATCHDOG_ERROR);
    $return['error'] = $error;
  }
  if ($message) {
    watchdog('l10n_drupal', $message);
    $return['message'] = $message;
  }

  return $return;
}

/**
 * CVS revision saver callback for potx. We call it with a release id
 * if $file is not given. And we ask for a file ID (to save the string
 * with), if $revision is not given.
 *
 * This is called:
 *  - before any file parsing with (array($pid, $rid), NULL)
 *  - just as a new file is found by potx with ($revision, $file)
 *  - just as a new string is found by our own callback with (NULL, $file)
 *
 * @param $revision
 *   CVS revision information about $file. If not given, the recorded
 *   fid of $file will be returned in an array with ($pid, $rid, $fid).
 * @param $file
 *   File location in package. If not given, $revision is taken as an array
 *   with project and release id to use to store the file list.
 */
function l10n_drupal_save_file($revision = NULL, $file = NULL) {
  static $pid = 0;
  static $rid = 0;
  static $files = array();

  if (!isset($file)) {
    // We get the release number for the files.
    list($pid, $rid) = $revision;
  }
  elseif (!isset($revision)) {
    // We return data for a specific file.
    return array($pid, $rid, $files[$file]);
  }
  else {
    if ($existing_file = db_fetch_object(db_query("SELECT * FROM {l10n_server_file} WHERE rid = %d AND location = '%s'", $rid, $file))) {
      if ($existing_file->revision != $revision) {
        // Changed revision on a file.
        db_query("UPDATE {l10n_server_file} SET revision = '%s' WHERE fid = %d", $revision, $existing_file->fid);
      }
      $fid = $existing_file->fid;
    }
    else {
      // New file in this release.
      db_query("INSERT INTO {l10n_server_file} (pid, rid, location, revision) VALUES(%d, %d, '%s', '%s')", $pid, $rid, $file, $revision);
      $fid = db_result(db_query("SELECT fid FROM {l10n_server_file} WHERE rid = %d and location = '%s'", $rid, $file));
    }
    $files[$file] = $fid;
  }
}

/**
 * String saving callback for potx.
 *
 * @todo
 *   More elegant plural handling.
 * @todo
 *   Find a way to properly use POTX constants before potx.inc is
 *   loaded.
 *
 * @param $value
 *   String value to store.
 * @param $context
 *   From Drupal 7, separate contexts are supported. POTX_CONTEXT_NONE is
 *   the default, if the code does not specify a context otherwise.
 * @param $file
 *   Name of file the string occured in.
 * @param $line
 *   Number of line the string was found.
 * @param $string_type
 *   String type: POTX_STRING_INSTALLER, POTX_STRING_RUNTIME
 *   or POTX_STRING_BOTH.
 */
function l10n_drupal_save_string($value = NULL, $context = NULL, $file = NULL, $line = 0, $string_type = 2 /*POTX_STRING_RUNTIME*/) {
  static $files = array();

  // Strip all slashes from string.
  $value = stripcslashes($value);

  if (!isset($files[$file])) {
    // Get file ID for saving, locally cache.
    $files[$file] = l10n_drupal_save_file(NULL, $file);
  }

  // Value set but empty. Mark error on empty translatable string. Only trim
  // for empty string checking, since we should store leading/trailing
  // whitespace as it appears in the string otherwise.
  $check_empty = trim($value);
  if (empty($check_empty)) {
    potx_status('error', t('Empty string attempted to be localized. Please do not leave test code for localization in your source.'), $file, $line);
    return;
  }

  // If we have the file entry now, we can process adding the string.
  if (isset($files[$file])) {
    // Explode files array to pid, rid and fid.
    list($pid, $rid, $fid) = $files[$file];

    // A \0 separator in the string means we deal with a string with plural variants.
    // Unlike Drupal core, we store all in the same string, as it is easier
    // to handle later, and we don't need the individual string parts.
    if (!$sid = db_result(db_query("SELECT sid FROM {l10n_server_string} WHERE hashkey = MD5('%s')", $value . $context))) {
      // String does not exist.
      db_query("INSERT INTO {l10n_server_string} (value, context, hashkey) VALUES ('%s', '%s', MD5('%s'))", $value, $context, $value . $context);
      $sid = db_result(db_query("SELECT sid FROM {l10n_server_string} WHERE hashkey = MD5('%s')", $value . $context));
    }
    if (!db_result(db_query("SELECT fid FROM {l10n_server_line} WHERE fid = %d AND sid = %d AND lineno = %d AND type = %d", $fid, $sid, $line, $string_type))) {
      // Location does not exist with this string.
      db_query("INSERT INTO {l10n_server_line} (pid, rid, fid, sid, lineno, type) VALUES (%d, %d, %d, %d, %d, %d)", $pid, $rid, $fid, $sid, $line, $string_type);
    }
  }
}

/**
 * Delete the complete contents of a directory recursively.
 *
 * @param $dirname
 *  The directory name to be deleted.
 */
function l10n_drupal_rmdir_recursive($directory) {
  if (!is_dir($directory)) {
    return;
  }
  if (substr($directory, -1) != '/') {
    $directory .= '/';
  }
  if ($handle = opendir($directory)) {
    while ($file = readdir($handle)) {
        if ($file == '.' or $file == '..') {
          continue;
        }
        $path = $directory .'/'. $file;
        if (is_dir($path)) {
          l10n_drupal_rmdir_recursive($path);
        }
        else {
          unlink($path);
        }
    }
    rmdir($directory);
    closedir($handle);
  }
}

// = API functions =============================================================

/**
 * Detect major version number for given project file.
 *
 * @param $path
 *   Either a file name or a path to a file, containing the file name.
 * @return
 *   A number with the major version of the project file, computed from
 *   the version portion of the filename.
 *     - 4 for 4.x versions (even 4.6.x. and 4.7.x)
 *     - 5 for 5.x versions
 *     - 6 for 6.x versions
 *     - 7 for 7.x versions
 */
function l10n_drupal_detect_major_version($path) {
  // Only interested in the filename.
  $filename = basename($path);
  // The project name could not contain hyphens, as the project name equals
  // function name prefixes, and hyphens are not allowed in function names.
  list($project_name, $version) = explode('-', $filename);
  // The major number is the first digit (eg. 6 for 6.x-dev, 4 for 4.7.x).
  return (int) $version;
}

/**
 * Does the given file path point to a package with a supported major version?
 *
 * @param $path
 *   Either a file name or a path to a file, containing the file name.
 * @return
 *   TRUE if the major version is supported, FALSE otherwise.
 */
function l10n_drupal_is_supported_version($path) {
  // Only Drupal 5.x, 6.x and 7.x projects are supported.
  return in_array(l10n_drupal_detect_major_version($path), array(5, 6, 7));
}
