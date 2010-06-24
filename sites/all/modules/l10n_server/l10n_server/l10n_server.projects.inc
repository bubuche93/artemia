<?php
// $Id: l10n_server.projects.inc,v 1.1.2.3 2010/05/04 09:44:41 goba Exp $

/**
 * @file
 *   Administrative functions for localization server projects.
 */

/**
 * Number of projects to display on admin pages.
 */
define('L10N_SERVER_ADMIN_PROJECTS_PER_PAGE', 30);

// = Main project admin pages ==================================================

/**
 * Menu callback for projects administration listing.
 */
function l10n_server_admin_projects() {
  if (!$projects = l10n_server_get_projects(array('pager' => L10N_SERVER_ADMIN_PROJECTS_PER_PAGE, 'all' => TRUE))) {
    drupal_set_message(t('No projects found.'), 'error');
    return '';
  }
  return drupal_get_form('l10n_server_admin_projects_form', $projects);
}

/**
 * Form builder for projects administration listing.
 */
function l10n_server_admin_projects_form(&$form_state, $projects) {
  $form = array();
  $form['pager'] = array(
    '#value' => theme('pager', NULL, L10N_SERVER_ADMIN_PROJECTS_PER_PAGE, 0),
  );
  $form['orphans'] = array(
    '#type' => 'value',
    '#value' => ($orphans = l10n_server_orphan_strings()),
  );
  $form['projects'] = array(
    '#tree' => TRUE,
  );

  $table = array();
  foreach ($projects as $project) {
    $form['projects'][$project->pid] = array(
      'title' => array(
        '#value' => $project->title .'<br /><small>'. (!empty($project->home_link) ? '<a href="'. $project->home_link .'">'. $project->home_link .'</a>' : t('No link available')) .'</small>',
      ),
      'status' => array(
        '#type' => 'checkbox',
        '#default_value' => $project->status,
      ),
      'reset' => array(
        '#value' => l(t('Start over'), 'admin/l10n_server/projects/reset/'. $project->uri),
      ),
      'delete' => array(
        '#value' => $orphans ? '' : l(t('Delete'), 'admin/l10n_server/projects/delete/'. $project->uri),
      ),
      'releases' => array(
        '#value' => l(t('Releases'), 'admin/l10n_server/projects/releases/'. $project->uri),
      ),
      'edit' => array(
        '#value' => l(t('Edit'), 'admin/l10n_server/projects/edit/'. $project->uri),
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes')
  );

  $form['#theme'] = 'l10n_server_admin_projects_form';
  return $form;
}

/**
 * Form theme for projects administration listing.
 */
function theme_l10n_server_admin_projects_form($form) {
  $output = ($pager = drupal_render($form['pager']));
  $table = array();
  foreach ($form['projects'] as $pid => &$project_element) {
    if (!is_int($pid)) {
      // Skip non-project elements.
      continue;
    }
    $table[] = array(
      drupal_render($project_element['status']),
      drupal_render($project_element['title']),
      drupal_render($project_element['edit']),
      drupal_render($project_element['releases']),
      drupal_render($project_element['reset']),
      drupal_render($project_element['delete']),
    );
  }
  $headers = array(t('Enabled'), t('Project'), array('data' => t('Operations'), 'colspan' => 4));
  $output .= theme('table', $headers, $table);
  $output .= $pager;
  $output .= drupal_render($form);

  if ($form['orphans']['#value']) {
    drupal_set_message(format_plural($form['orphans']['#value'], 'There is @count source string orphaned.', 'There are @count source strings orphaned.') .' '. t('Deleting projects will not be available until you <a href="@cleanup">clean up the database</a> or rescan projects (within connectors you use) if you have started over with some of them.', array('@cleanup' => url('admin/l10n_server/projects/cleanup'))));
  }
  return $output;
}

/**
 * Submission handler for l10n_server_admin_projects_form().
 */
function l10n_server_admin_projects_form_submit($form, &$form_state) {
  $disabled = array();
  foreach ($form_state['values']['projects'] as $pid => $project) {
    db_query('UPDATE {l10n_server_project} SET status = %d WHERE pid = %d', $project['status'], $pid);
  }
  drupal_set_message(t('Localization server project changes saved.'));
}

// = Delete a project from the database ========================================

/**
 * Form callback to display a confirm form for deleting projects.
 */
function l10n_server_admin_projects_delete(&$form_state, $project_uri) {
  $project = l10n_server_get_projects(array('uri' => $project_uri));
  $form = array(
    'project' => array(
      '#type' => 'value',
      '#value' => $project,
    ),
  );
  return confirm_form($form, t('Are you sure you would like to delete project %project from this Localization server?', array('%project' => $project->title)), 'admin/l10n_server/projects', t('This would remove all data associated with the project, including releases, files, parsing errors, and all source strings and translations which are not applicable to remaining projects. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for deleting projects.
 */
function l10n_server_admin_projects_delete_submit($form, &$form_state) {
  l10n_server_delete_project($form_state['values']['project']->pid);
  drupal_set_message(t('The %project was deleted from the Localization server.', array('%project' => $form_state['values']['project']->title)));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = Reset a project in the database ===========================================

/**
 * Form callback to display a confirm form for resetting projects.
 */
function l10n_server_admin_projects_reset(&$form_state, $project_uri) {
  $project = l10n_server_get_projects(array('uri' => $project_uri));
  $form = array(
    'project' => array(
      '#type' => 'value',
      '#value' => $project,
    ),
  );
  return confirm_form($form, t('Are you sure you would like to start over with the project %project on this Localization server?', array('%project' => $project->title)), 'admin/l10n_server/projects', t('This would remove almost all data associated with the project: releases, files, line and parsing error information. Source strings found within this project and translations for them are temporarily kept. If you clean up the database before this project is parsed again, source strings and translations will be lost. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for resetting projects.
 */
function l10n_server_admin_projects_reset_submit($form, &$form_state) {
  l10n_server_delete_project($form_state['values']['project']->pid, TRUE);
  drupal_set_message(t('Project data for %project was deleted from the Localization server. Make sure to parse releases for this project again before deleting a project or cleaning up the database, or you will loose existing translation data.', array('%project' => $form_state['values']['project']->title)));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = Show list of releases for a project =======================================

/**
 * Menu callback for projects administration listing.
 */
function l10n_server_admin_releases($project_uri) {
  $project = l10n_server_get_projects(array('uri' => $project_uri));
  drupal_set_title(t('Releases of project @project', array('@project' => $project->title)));
  if (!$project_releases = l10n_server_get_releases($project_uri, FALSE)) {
    drupal_set_message(t('No releases found for this project.'), 'error');
    return '';
  }
  return drupal_get_form('l10n_server_admin_releases_form', $project, $project_releases);
}

/**
 * Display a form with list of releases for a given project.
 *
 * @todo
 *   Just a list at the moment. Make it a form with useful tools.
 */
function l10n_server_admin_releases_form(&$form_state, $project, $releases) {
  $form = array(
    'releases' => array(
      '#tree' => TRUE,
    ),
  );

  list($connector, $source) = l10n_server_split_connector($project->connector_module);
  foreach ($releases as $release) {
    $form['releases'][$release->rid] = array(
      'title' => array(
        '#value' => $release->title,
      ),
      'download_link' => array(
        '#value' => (!empty($release->download_link) ? l(basename($release->download_link), $release->download_link) : t('none')),
      ),
      'file_date' => array(
        '#value' => (!empty($release->file_date) ? format_date($release->file_date) : t('n/a')),
      ),
      'is_parsed' => array(
        '#value' => (!empty($release->last_parsed) ? theme('image', 'misc/watchdog-ok.png', t('parsed'), t('parsed')) : ''),
      ),
      'last_parsed' => array(
        '#value' => (!empty($release->last_parsed) ? format_date($release->last_parsed) : t('n/a')),
      ),
      'operations' => array(
        '#value' => array(
          ($source == 'uploads' ? l(t('Edit'), 'admin/l10n_server/projects/releases/'. $project->uri .'/edit/'. $release->rid) : ''),
          l(t('Start over'), 'admin/l10n_server/projects/releases/'. $project->uri .'/reset/'. $release->rid),
          l(t('Delete'), 'admin/l10n_server/projects/releases/'. $project->uri .'/delete/'. $release->rid),
        ),
      ),
    );
  }

  $form['#theme'] = 'l10n_server_admin_releases_form';
  return $form;
}

/**
 * Form theme for release administration listing.
 */
function theme_l10n_server_admin_releases_form($form) {
  $table = array();
  $output = '';
  $max_operations = 1;
  foreach ($form['releases'] as $rid => &$release_element) {
    if (!is_int($rid)) {
      // Skip non-project elements.
      continue;
    }
    $row = array(
      drupal_render($release_element['is_parsed']),
      drupal_render($release_element['title']),
      drupal_render($release_element['download_link']),
      drupal_render($release_element['file_date']),
      drupal_render($release_element['last_parsed']),
    );
    $max_operations = max($max_operations, count($release_element['operations']));
    foreach ($release_element['operations']['#value'] as $operation) {
      $row[] = $operation;
    }
    $table[] = $row;
    // Mark as rendered, already included its output above.
    drupal_render($release_element['operations']);
  }
  $headers = array(array('data' => t('Release'), 'colspan' => 2), t('Download link'), t('File date'), t('Last parsed'), array('data' => t('Operations'), 'colspan' => $max_operations));
  $output .= theme('table', $headers, $table);
  $output .= drupal_render($form);
  return $output;
}

// = Source strings cleanup ====================================================

function l10n_server_admin_projects_cleanup() {
  if ($orphans = l10n_server_orphan_strings()) {
    return drupal_get_form('l10n_server_admin_projects_cleanup_form', $orphans);
  }
  drupal_set_message(t('You have no orphan source strings in your database to clean up.'));
  return '';
}

/**
 * Form callback to display a confirm form for cleaning up strings.
 */
function l10n_server_admin_projects_cleanup_form(&$form_state, $orphans) {
  return confirm_form(array(), format_plural($orphans, 'Are you sure you would like to clean up @count orphan?', 'Are you sure you would like to clean up @count orphans?'), 'admin/l10n_server/projects', t('There is at least one orphan source string in your database. Orphan source strings are not tied to any release of a project managed on your Localization server. It might be safe to remove these if you are not in the middle of rescanning some projects. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for cleaning up strings.
 */
function l10n_server_admin_projects_cleanup_form_submit($form, &$form_state) {
  l10n_server_delete_orphans();
  drupal_set_message(t('Orphan strings and their translations were removed.'));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = Edit/add a project ========================================================

/**
 * Project editing and submission form.
 */
function l10n_server_admin_projects_edit_form(&$form_state, $project_uri = NULL) {
  if (!isset($project_uri)) {
    $connector_list = l10n_server_get_add_enabled_project_connectors();
    // Provide sensible default for project if adding new one.
    $project = (object) array('pid' => NULL, 'uri' => '', 'title' => '', 'home_link' => '', 'connector_module' => array_shift(array_keys($connector_list)), 'status' => 1);
  }
  else {
    $project = l10n_server_get_projects(array('uri' => $project_uri));
    $connector_list = l10n_server_get_project_connectors();
  }
  list($connector, $source) = l10n_server_split_connector($project->connector_module);

  $form = array();
  $form['previous_project'] = array(
    '#type' => 'value',
    '#value' => $project
  );
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Project URI'),
    '#description' => t('Short name of project used in paths. This will appear in paths like <em>translate/projects/uri</em> at the end. Suggested to use lowercase only.'),
    '#default_value' => $project->uri,
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Project name'),
    '#description' => t('Human readable name of project.'),
    '#default_value' => $project->title,
    '#required' => TRUE,
  );
  $form['home_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Home link'),
    '#description' => t('Link to home page of project, displayed on project pages.'),
    '#default_value' => $project->home_link,
  );
  $form['connector_module'] = array(
    '#type' => 'radios',
    '#title' => t('Connector handling project data'),
    '#description' => t('Data and source handler for this project. Cannot be modified later.'),
    '#options' => $connector_list,
    '#default_value' => $project->connector_module,
    '#disabled' => !empty($project->pid)
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save project')
  );

  if (!empty($project->pid)) {
    if ($source != 'uploads') {
      $form['uri']['#type'] = 'item';
      $form['uri']['#value'] = check_plain($form['uri']['#default_value']);
    }
    $form['connector_module']['#type'] = 'item';
    $form['connector_module']['#value'] = check_plain($connector_list[$form['connector_module']['#default_value']]);
  }
  return $form;
}

/**
 * Ensure project URI is unique.
 */
function l10n_server_admin_projects_edit_form_validate($form, &$form_state) {
  if (($existing = l10n_server_get_projects(array('uri' => $form_state['values']['uri']))) && ($form_state['values']['previous_project']->pid != $existing->pid)) {
    form_set_error('uri', t('A project titled %title already exists with URI @uri. The URI should be unique.', array('%title' => $existing->title, '@uri' => $existing->uri)));
  }
}

/**
 * Form submission function for project editing/submission.
 */
function l10n_server_admin_projects_edit_form_submit($form, &$form_state) {
  $new_project = $form_state['values']['previous_project'];
  foreach (array('uri', 'title', 'home_link', 'connector_module') as $property) {
    if (isset($form_state['values'][$property])) {
      $new_project->$property = $form_state['values'][$property];
    }
  }
  drupal_write_record('l10n_server_project', $new_project, !empty($new_project->pid) ? 'pid' : array());
  drupal_set_message(t('Project data saved.'));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = Edit/add a release ========================================================

/**
 * Release editing/submission form.
 */
function l10n_server_admin_projects_release_edit_form(&$form_state, $project_uri, $release_rid = NULL) {
  $project = l10n_server_get_projects(array('uri' => $project_uri));
  if (empty($release_rid)) {
    $release = (object) array('pid' => $project->pid, 'title' => '', 'rid' => NULL, 'download_link' => '');
  }
  else {
    $releases = l10n_server_get_releases($project_uri, FALSE);
    $release = $releases[$release_rid];
  }

  $form = array();
  $form['project'] = array(
    '#type' => 'value',
    '#value' => $project
  );
  $form['previous_release'] = array(
    '#type' => 'value',
    '#value' => $release
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Release version'),
    '#description' => t('Version name or code name of release.'),
    '#default_value' => $release->title,
    '#required' => TRUE,
  );
  $form['download_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Download link'),
    '#description' => t('Download link for this release of the software.'),
    '#default_value' => $release->download_link,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save release'),
    '#weight' => 200,
  );
  return $form;
}

/**
 * Form submission function for release editing/submission.
 */
function l10n_server_admin_projects_release_edit_form_submit($form, &$form_state) {
  $new_release = $form_state['values']['previous_release'];
  foreach (array('title', 'download_link') as $property) {
    if (isset($form_state['values'][$property])) {
      $new_release->$property = $form_state['values'][$property];
    }
  }

  drupal_write_record('l10n_server_release', $new_release, !empty($new_release->rid) ? 'rid' : array());
  drupal_set_message(t('Release data saved.'));

  // Save the new release for any successive form handlers
  // (such as an uploads source handler).
  $form_state['values']['new_release'] = $new_release;

  $form_state['redirect'] = 'admin/l10n_server/projects/releases/'. $form_state['values']['project']->uri;
}

// = Delete a release from the database ========================================

/**
 * Form callback to display a confirm form for deleting releases.
 */
function l10n_server_admin_projects_release_delete(&$form_state, $project_uri, $release_rid) {
  $project = l10n_server_get_projects(array('uri' => $project_uri));
  $releases = l10n_server_get_releases($project_uri, FALSE);
  $release = $releases[$release_rid];
  $form = array(
    'project' => array(
      '#type' => 'value',
      '#value' => $project,
    ),
    'release' => array(
      '#type' => 'value',
      '#value' => $release,
    ),
  );
  return confirm_form($form, t('Are you sure you would like to delete %project %release from this Localization server?', array('%project' => $project->title, '%release' => $release->title)), 'admin/l10n_server/projects/releases/'. $project->uri, t('This would remove all data associated with the release, including files, parsing errors, and all source strings and translations which are not applicable to remaining releases/projects. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for deleting releases.
 */
function l10n_server_admin_projects_release_delete_submit($form, &$form_state) {
  l10n_server_delete_release($form_state['values']['release']->rid);
  drupal_set_message(t('%project %release was deleted from the Localization server.', array('%project' => $form_state['values']['project']->title, '%release' => $form_state['values']['release']->title)));
  $form_state['redirect'] = 'admin/l10n_server/projects/releases/'. $form_state['values']['project']->uri;
}

// = Reset a release in the database ===========================================

/**
 * Form callback to display a confirm form for resetting releases.
 */
function l10n_server_admin_projects_release_reset(&$form_state, $project_uri, $release_rid) {
  $project = l10n_server_get_projects(array('uri' => $project_uri));
  $releases = l10n_server_get_releases($project_uri, FALSE);
  $release = $releases[$release_rid];
  $form = array(
    'project' => array(
      '#type' => 'value',
      '#value' => $project,
    ),
    'release' => array(
      '#type' => 'value',
      '#value' => $release,
    ),
  );
  return confirm_form($form, t('Are you sure you would like to start over with %project %release on this Localization server?', array('%project' => $project->title, '%release' => $release->title)), 'admin/l10n_server/projects/releases/'. $project->uri, t('This would remove almost all data associated with the release: files, line and parsing error information. Source strings found within this project and translations for them are temporarily kept. If you clean up the database before this release is parsed again, source strings and translations will be lost. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for resetting projects.
 */
function l10n_server_admin_projects_release_reset_submit($form, &$form_state) {
  l10n_server_delete_release($form_state['values']['release']->rid, TRUE);
  drupal_set_message(t('Release data for %project %release was deleted from the Localization server. Make sure to parse this release data again before deleting a project or cleaning up the database, or you will loose existing translation data.', array('%project' => $form_state['values']['project']->title, '%release' => $form_state['values']['release']->title)));
  $form_state['redirect'] = 'admin/l10n_server/projects/releases/'. $form_state['values']['project']->uri;
}

// = API functions =============================================================

/**
 * API function to delete or "reset" projects.
 *
 * Deleting removes all data associated with the project and cleans up any
 * orphan strings in the database. Resetting a project almost the same, but
 * orphaned strings are purposefully kept.
 *
 * @param $pid
 *   Project identifier.
 * @param $keep_strings_and_project
 *   If TRUE, the project itself, source strings and translations are kept. Used
 *   for the "Start over" feature which lets regeneration of project data without
 *   loosing the actual translations.
 */
function l10n_server_delete_project($pid, $keep_strings_and_project = FALSE) {

  // Drop errors stored for releases of this project.
  db_query('DELETE FROM {l10n_server_error} WHERE rid IN (SELECT rid FROM {l10n_server_release} WHERE pid = %d)', $pid);

  // Almost all other related data has the pid associated directly.
  db_query('DELETE FROM {l10n_server_line} WHERE pid = %d', $pid);
  db_query('DELETE FROM {l10n_server_file} WHERE pid = %d', $pid);
  db_query('DELETE FROM {l10n_server_release} WHERE pid = %d', $pid);

  if (!$keep_strings_and_project) {
    db_query('DELETE FROM {l10n_server_project} WHERE pid = %d', $pid);
    l10n_server_delete_orphans();
  }
  else {
    db_query('UPDATE {l10n_server_project} SET last_parsed = 0 WHERE pid = %d', $pid);
  }
}

/**
 * Delete all orphan source strings and their associated translations.
 */
function l10n_server_delete_orphans() {
  // Drop all stale source strings, translations and status flags.
  db_query('DELETE FROM {l10n_server_string} WHERE sid NOT IN (SELECT sid FROM {l10n_server_line})');
  db_query('DELETE FROM {l10n_server_translation} WHERE sid NOT IN (SELECT sid FROM {l10n_server_line})');
  db_query('DELETE FROM {l10n_server_status_flag} WHERE sid NOT IN (SELECT sid FROM {l10n_server_line})');
}

/**
 * Detect the count of orphan strings in the database, if any.
 */
function l10n_server_orphan_strings() {
  return db_result(db_query('SELECT COUNT(*) FROM {l10n_server_string} s LEFT JOIN {l10n_server_line} l ON s.sid = l.sid WHERE l.sid IS NULL'));
}

/**
 * API function to delete or "reset" a release.
 *
 * Deleting removes all data associated with the release and cleans up any
 * orphan strings in the database. Resetting a release almost the same, but
 * orphaned strings are purposefully kept.
 *
 * @param $rid
 *   Release identifier.
 * @param $keep_strings_and_release
 *   If TRUE, the release itself, source strings and translations are kept. Used
 *   for the "Start over" feature which lets regeneration of release data without
 *   loosing the actual translations.
 */
function l10n_server_delete_release($rid, $keep_strings_and_release = FALSE) {

  // Drop errors stored for releases of this project.
  db_query('DELETE FROM {l10n_server_error} WHERE rid = %d', $rid);

  // Almost all other related data has the rid associated directly.
  db_query('DELETE FROM {l10n_server_line} WHERE rid = %d', $rid);
  db_query('DELETE FROM {l10n_server_file} WHERE rid = %d', $rid);

  if (!$keep_strings_and_release) {
    db_query('DELETE FROM {l10n_server_release} WHERE rid = %d', $rid);
    l10n_server_delete_orphans();
  }
  else {
    db_query('UPDATE {l10n_server_release} SET last_parsed = 0 WHERE rid = %d', $rid);
  }
}
