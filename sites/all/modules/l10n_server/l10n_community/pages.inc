<?php
// $Id: pages.inc,v 1.1.2.20.2.24.2.10 2010/03/26 09:15:42 goba Exp $

/**
 * @file
 *   Overview pages for localization community.
 */

// = Overview screens ==========================================================

/**
 * Translation status overview for all languages.
 */
function l10n_community_explore_languages() {

  // Checking whether we have languages and strings.
  if (!$languages = l10n_community_get_languages()) {
    drupal_set_message(t('No languages to list.'), 'error');
    return '';
  }
  if (!$num_source = l10n_community_get_string_count('all')) {
    drupal_set_message(t('No strings to translate.'), 'error');
    return '';
  }

  // Generate listing of all languages with summaries. The list of languages
  // is relatively "short", compared to projects, so we don't need a pager here.
  $table = array();
  $string_counts = l10n_community_get_string_count('languages');
  foreach ($languages as $langcode => $language) {
    if (empty($language->plurals)) {
      $table[] = array(
        array('data' => $language->name .' ('. $langcode .')', 'class' => 'rowhead'),
        array('data' => t('Uninitialized plural formula. Please set up the plural formula in <a href="@language-config">the langauge configuration</a> or alternatively <a href="@import-url">import a valid interface translation</a> for Drupal in this language.', array('@import-url' => url('admin/build/translate/import'), '@language-config' => url('admin/settings/language'))), 'colspan' => '3', 'class' => 'error')
      );
    }
    else {
      $table[] = array_merge(
        array(
          array('data' => l($language->name .' ('. $langcode .')', 'translate/languages/'. $langcode), 'class' => 'rowhead'),
        ),
        isset($string_counts[$langcode]) ? theme('l10n_community_progress_columns', $num_source, @$string_counts[$langcode]['translations'], @$string_counts[$langcode]['suggestions']) : theme('l10n_community_progress_columns', $num_source, 0, 0)
      );
    }
  }
  $header = theme('l10n_community_progress_headers', t('Language'));
  return theme('l10n_community_table', $header, $table) .'<div class="clear-block"></div>';
}

/**
 * Translation status overview for all projects.
 *
 * Because projects are much more numerous then languages, we need
 * a pager on this screen.
 */
function l10n_community_explore_projects() {
  $pager_setting = variable_get('l10n_community_project_per_page', 10);

  $output = '';
  if (!$projects = l10n_server_get_projects(array('pager' => $pager_setting))) {
    drupal_set_message(t('No projects found.'), 'error');
    return '';
  }
  $languages = l10n_community_get_languages('name');
  $language_count = count($languages);

  $output .= theme('pager', NULL, $pager_setting, 0);
  $table = array();
  $string_counts = l10n_community_get_string_count('projects');
  foreach ($projects as $project) {
    $table[] = array_merge(
      array(
        array('data' => l($project->title, 'translate/projects/'. $project->uri), 'class' => 'rowhead'),
      ),
      // Multiply summary count by languages handled, so we get an
      // accurate picture of completeness.
      theme('l10n_community_progress_columns', (@$string_counts[$project->pid]['count'] * $language_count), @$string_counts[$project->pid]['translations'], @$string_counts[$project->pid]['suggestions'])
    );
  }
  $output .= theme(
    'l10n_community_table',
    theme('l10n_community_progress_headers', t('Project')),
    $table,
    array('class' => 'l10n-community-overview')
  );
  $output .= theme('pager', NULL, $pager_setting, 0);
  return $output .'<div class="clear-block"></div>';
}

/**
 * Translation status page of all projects from a given language.
 *
 * @param $langcode
 *   Language code, for example 'hu', 'pt-br', 'de', 'it'.
 */
function l10n_community_overview_language($langcode = NULL) {
  global $user;

  if (!isset($langcode)) {
    drupal_set_message(t('No language selected.'), 'error');
    return '';
  }

  $languages = l10n_community_get_languages('name');
  drupal_set_title(t('@language overview', array('@language' => $languages[$langcode])));

  $content_output = $next_action = '';
  if (module_exists('l10n_groups')) {
    // If l10n_groups available, try to load group frontpage.
    $groups = l10n_groups_get_groups();
    if (isset($groups[$langcode]) && node_access('view', $group_node = node_load($groups[$langcode]))) {
      $content_output = node_view($group_node, FALSE, TRUE);
      if (!isset($user->og_groups[$groups[$langcode]])) {
        $next_action = '<div class="l10n-next-action">'. og_subscribe_link($group_node) .'</div>';
      }
    }
    else {
      $content_output .= '<p>'. t('Group not found or not accessible. No posts to display') .'</p>';
    }
  }
  elseif (module_exists('taxonomy')) {
    if ($term = taxonomy_get_term_by_name('l10n-server-'. $langcode)) {
      // Found tag, look for posts.
      $result = taxonomy_select_nodes(array($term[0]->tid), 'or', 0, TRUE);
      while ($node = db_fetch_object($result)) {
        $content_output .= node_view(node_load($node->nid), 1);
      }
    }
    if (empty($content_output)) {
      // Either no such tag, or that tag has no posts currently.
      $output .= '<p>'. t('There are currently no posts set for display here. Mark posts to display here with the %tagname tag, using the taxonomy module.', array('%tagname' => 'l10n-server-'. $langcode)) .'</p>';
    }
  }
  else {
    // Neither l10n_groups, neither taxonomy available.
    $content_output .= '<p>'. t('No posts to display') .'</p>';
  }

  // If we don't know the next action yet, and the user can submit suggestions,
  // call for submitting those.
  if (empty($next_action) && user_access('submit suggestions')) {
    $next_action = '<div class="l10n-next-action">'. l(t('Translate'), 'translate/languages/'. $langcode .'/translate') .'</div>';
  }

  $output = '<div id="l10n-community-summary" class="admin clear-block"><div class="left clear-block">'. $content_output .'</div><div class="right clear-block">';

  $block = array(
    'title' => t('Contribute'),
    'description' => t('Review, translate, import'),
    'content' => $next_action .'<p class="info">'. t('Different tabs allow you to view existing translations, suggest new ones, import translations completed offline or export translations for use or  offline work.') .'</p>'
  );
  $output .= str_replace('class="admin-panel"', 'class="admin-panel admin-panel-contribute"', theme('admin_block', $block));

  $stats_numbers = l10n_community_get_stats($langcode);

  $block = array(
    'title' => t('Progress status'),
    'description' => t('Status overview'),
    'content' => theme('item_list', array(
      format_plural($stats_numbers['users'], '1 contributor', '@count contributors'),
      format_plural($stats_numbers['strings'], '1 string to translate', '@count strings to translate'),
      format_plural($stats_numbers['translations'], '1 translation recorded', '@count translations recorded'),
      format_plural($stats_numbers['suggestions'], '1 suggestion awaiting approval', '@count suggestions awaiting approval'),
    ))
  );
  $output .= theme('admin_block', $block);

  $people = l10n_community_get_string_count('top-people', $langcode);
  $block = array(
    'title' => t('Top contributors'),
    'description' => t('People with most approved translations'),
  );
  $list = array();
  foreach ($people as $translator) {
    // $translator can be passed to theme('username'), since it has 'name' and 'uid'.
    $list[] = format_plural($translator->sum, '!name - 1 translation', '!name - @count translations', array('!name' => theme('username', $translator)));
  }
  if ($list) {
    $block['content'] = theme('item_list', $list);
  }
  else {
    $block['content'] = '<p>'. t('Nobody contributed to this translation yet.') .'</p>';
  }
  $output .= theme('admin_block', $block);

  if (module_exists('l10n_groups')) {
    $og_block = module_invoke('og', 'block_details');
    if (!empty($og_block)) {
      $block = array(
        'title' => t('Group links'),
        'description' => t('Create content, manage your membership'),
        'content' => $og_block['content'],
      );
      $output .= theme('admin_block', $block);
    }
  }

  $output .= '</div></div>';

  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore languages'), 'translate/languages')
    )
  );

  return $output;
}

/**
 * Translation status page of all languages for a given project.
 *
 * @param $uri
 *   Project URI.
 */
function l10n_community_overview_project($uri = NULL) {

  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore projects'), 'translate/projects')
    )
  );

  if (!isset($uri)) {
    drupal_set_message(t('No project selected.'), 'error');
    return '';
  }
  drupal_set_title(t('@project project translations', array('@project' => drupal_get_title())));

  if (!$languages = l10n_community_get_languages()) {
    drupal_set_message(t('No languages to list.'), 'error');
    return '';
  }

  $output = '<div id="l10n-community-summary" class="admin clear-block"><div class="left clear-block">';

  $project = l10n_server_get_projects(array('uri' => $uri));
  $output .= l10n_community_language_progress_for_project($project, $languages, t('Translations overview'), t('Overall status of translations'));

  $output .= '</div><div class="right clear-block">';

  $block = array(
    'title' => t('Contribute'),
    'description' => t('Export, translate, review'),
    'content' => '<p class="info">'. t('Select a language from the overview to review translations and contribute to the translation efforts. The export tab allows exporting of translation templates, but translations can also be exported on the language pages.') .'</p>'
  );
  $output .= str_replace('class="admin-panel"', 'class="admin-panel admin-panel-contribute"', theme('admin_block', $block));

  $num_source = l10n_community_get_string_count('project', $project->pid);

  // Get information on releases, and the number of parsed ones.
  $releases = l10n_server_get_releases($uri, FALSE);
  $num_parsed = 0;
  foreach ($releases as $release) {
    if ($release->last_parsed > 0) {
      $num_parsed++;
    }
  }

  // Get information on warnings.
  $num_warnings = db_result(db_query("SELECT COUNT(DISTINCT e.eid) FROM {l10n_server_project} p LEFT JOIN {l10n_server_release} r ON p.pid = r.pid LEFT JOIN {l10n_server_error} e ON r.rid = e.rid WHERE p.uri = '%s'", $uri));

  $block = array(
    'title' => t('@project summary', array('@project' => $project->title)),
    'description' => t('Some details we know about this project'),
    'content' => theme('item_list', array(
      t('Project home') .': '. (!empty($project->home_link) ? ('<a href="'. check_url($project->home_link) .'">'. check_plain($project->home_link) .'</a>') : t('Not available')),
      format_plural(count($releases), '1 release known', '@count releases known'),
      format_plural($num_parsed, '1 release parsed', '@count releases parsed'),
      format_plural($num_source, '1 source string in total', '@count source strings in total'),
      ($num_warnings == 0 ? t('No source code warnings') : format_plural($num_warnings, '1 source code warning', '@count source code warnings')),
    ))
  );
  $output .= theme('admin_block', $block);
  $output .= '</div></div>';
  return $output;
}

/**
 * Reusable renderer for language status per project.
 */
function l10n_community_language_progress_for_project($project, $languages, $title, $description) {
  $block = array(
    'title' => $title,
    'description' => $description,
  );
  $num_source = l10n_community_get_string_count('project', $project->pid);
  $string_counts = l10n_community_get_string_count('languages', $project->pid);

  $table = array();
  foreach ($languages as $langcode => $language) {
    if (empty($language->plurals)) {
      $table[] = array(
        array('data' => $language->name .' ('. $langcode .')', 'class' => 'rowhead'),
        array('data' => t('Uninitialized plural formula. Please set up the plural formula in <a href="@language-config">the langauge configuration</a> or alternatively <a href="@import-url">import a valid interface translation</a> for Drupal in this language.', array('@import-url' => url('admin/build/translate/import'), '@language-config' => url('admin/settings/language'))), 'colspan' => '3', 'class' => 'error')
      );
    }
    else {
      $table[] = array_merge(
        array(
          array('data' => l($language->name .' ('. $langcode .')', 'translate/languages/'. $langcode .'/translate', array('query' => 'project='. $project->uri)), 'class' => 'rowhead'),
        ),
        theme(
          'l10n_community_progress_columns',
          $num_source,
          @$string_counts[$langcode]['translations'],
          @$string_counts[$langcode]['suggestions']
        )
      );
    }
  }
  $block['content'] = theme(
    'table',
    theme('l10n_community_progress_headers', t('Language')),
    $table,
    array('class' => 'l10n-community-overview')
  ) .'<div class="clear-block"></div>';

  return theme('admin_block', $block);
}

/**
 * Displays a page with source code warnings for a given project.
 *
 * @param $uri
 *   Project URI.
 */
function l10n_community_project_warnings($uri = NULL) {
  if (!isset($uri)) {
    drupal_set_message(t('No project selected.'), 'error');
    return '';
  }
  drupal_set_title(t('@project project source code warnings', array('@project' => drupal_get_title())));

  if (!$warnings = l10n_community_get_warnings($uri)) {
    drupal_set_message(t('No source code warnings recorded for any of the parsed releases of this project.'));
    return '';
  }

  $output = '';
  $releases = l10n_server_get_releases($uri);
  foreach ($releases as $release) {
    $output .= '<h3>'. $release->title .'</h3>';
    if (!isset($warnings[$release->rid])) {
      $output .= '<p>'. t('No source code warnings recorded for this release.') .'</p>';
    }
    else {
      $output .= theme('item_list', $warnings[$release->rid]);
    }
  }
  return $output;
}


// = Theme functions ===========================================================

/**
 * Progress bar and table column display for translation status information.
 *
 * @param $sum
 *   The number of all translatable strings to count with.
 * @param $translated
 *   Number of strings translated (and without outstanding
 *   suggestions) out of the $sum.
 * @param $has_suggestion
 *   Number of strings which are not translated yet (or are
 *   translated) and have at least one suggestion attached.
 * @return
 *   An indexed array with four elements, first being the progress bar,
 *   and the other three being calculated and percentages.
 */
function theme_l10n_community_progress_columns($sum, $translated, $has_suggestion) {
  if ($sum == 0) {
    // We handle a project without any source strings available for translation.
    return array(
      array('data' => t('No data available yet.'), 'colspan' => 3),
    );
  }

  // Compute numbers, percentages and provide alternate text titles.
  $done_percent = round($translated / $sum * 100, 2);
  $status = array(
    'translated'     => array((int) $translated, $done_percent, t('!percent translated')),
    'untranslated'   => array($sum - $translated, 100 - $done_percent, t('!percent untranslated')),
    // suggestions are not in the bar as they overlap with both translated and
    // untranslated strings, so we cannot display them here.
  );

  // Visual summary with a progress bar.
  $bar = '<div class="l10n-community-progress">';
  foreach ($status as $key => $values) {
    if ($values[1] > 0) {
      $bar .= '<div class="l10n-community-progress-'. $key .'" style="width:'. $values[1] .'%;" title="'. strtr($values[2], array('!percent' => $values[1] .'%')) .'"></div>';
    }
  }
  $bar .= '</div>';

  return array(
    $bar,
    ($sum - $translated),
    (int) $has_suggestion
    //t('@num untranslated', array('@num' => ($sum - $translated))),
    //($has_suggestion ? format_plural($has_suggestion, '@count has suggestions', '@count have suggestions') : t('no suggestions'))
  );
}

/**
 * Header columns for the progress data.
 */
function theme_l10n_community_progress_headers($mainhead) {
  return array(
    array('data' => $mainhead, 'class' => 'rowhead'),
    t('Overall status'),
    t('Untranslated'),
    t('Suggestions'),
  );
}

/**
 * Generate a summary table for l10n_community screens.
 *
 * @param $headers
 *   An array of table headers.
 * @param $table
 *   An array of table rows.
 */
function theme_l10n_community_table($header, $table) {
  if (($row_count = count($table)) > 1) {
    $output = theme('table', $header, array_slice($table, 0, ceil($row_count/2)), array('class' => 'l10n-community-overview'));
    $output .= theme('table', $header, array_slice($table, ceil($row_count/2)), array('class' => 'l10n-community-overview'));
    return $output;
  }
  else {
    return theme('table', $header, $table, array('class' => 'l10n-community-overview'));
  }
}

// = API functions =============================================================

/**
 * Get string counts for summaries.
 *
 * @param $type
 *   Type of string count to return:
 *     - all:        count of all strings
 *     - project:    count of strings in one project (identified by $id)
 *     - languages:  array of the count of translated strings and suggestions by langcode
 *     - projects:   array of the count of translated strings and suggestions by pid
 *     - top-people: array of counts for top translators for a given language
 * @param $id
 *   The project id when the 'project' type is used. The result set can be
 *   restricted by an ID from the oppposing type for the later two types.
 *   Eg. 'projects' summaries can be restricted to one specific language, or
 *   'languages' summeries can be restricted to one specific project. This
 *   id represents the identifier (pid or langcode) of the restricting item.
 *   For the 'all' type, this value is discarded.
 *
 * @todo
 *   These queries are *slooow*. The query cache helps a lot with caching the
 *   result, so the slowness only shows for the first run, but still it would
 *   be good to look into optimizing these.
 */
function l10n_community_get_string_count($type, $id = NULL) {
  switch ($type) {
    case 'all':
      // Return a count of all strings.
      return db_result(db_query("SELECT COUNT(sid) FROM {l10n_server_string}"));

    case 'project':
      // Return a count of all strings in this project, id required.
      return db_result(db_query('SELECT COUNT(DISTINCT s.sid) FROM {l10n_server_line} l INNER JOIN {l10n_server_string} s ON l.sid = s.sid WHERE l.pid = %d', $id));

    case 'languages':
      if ($stats = cache_get('l10n:count:languages:'. $id, 'cache')) {
        return $stats->data;
      }
      else {
        // Summeries based on language codes, restricted to a specific project if $id is set.
        $sums = array();
        if (!isset($id)) {
          // Simple count query if we are not filtering by project.
          $count1_sql = "SELECT COUNT(sid) AS translation_count, language FROM {l10n_server_status_flag} WHERE has_translation = 1 GROUP BY language";
          $count2_sql = "SELECT COUNT(sid) AS translation_count, language FROM {l10n_server_status_flag} WHERE has_suggestion = 1 GROUP BY language";
          $count_args = array();
        }
        else {
          // More complex joins if we also need to factor the project in.
          $count1_sql = "SELECT COUNT(DISTINCT ts.sid) AS translation_count, ts.language FROM {l10n_server_line} l INNER JOIN {l10n_server_status_flag} ts ON l.sid = ts.sid WHERE l.pid = %d AND ts.has_translation = 1 GROUP BY ts.language";
          $count2_sql = "SELECT COUNT(DISTINCT ts.sid) AS translation_count, ts.language FROM {l10n_server_line} l INNER JOIN {l10n_server_status_flag} ts ON l.sid = ts.sid WHERE l.pid = %d AND ts.has_suggestion = 1 GROUP BY ts.language";
          $count_args = array($id);
        }
        $result = db_query($count1_sql, $count_args);
        while ($row = db_fetch_object($result)) {
          $sums[$row->language]['translations'] = $row->translation_count;
        }
        $result = db_query($count2_sql, $count_args);
        while ($row = db_fetch_object($result)) {
          $sums[$row->language]['suggestions'] = $row->translation_count;
        }
        cache_set('l10n:count:languages:'. $id, $sums, 'cache', CACHE_PERMANENT);
        return $sums;
      }
      break;

    case 'projects':
      // Get summaries by projects. Restricted to a specific language, if $id is set.

      // General community statistics.
      if ($stats = cache_get('l10n:count:projects:'. $id, 'cache')) {
        return $stats->data;
      }
      else {
        // First get the count of strings available for translation.
        $sums = $count_args = array();
        $result = db_query("SELECT COUNT(DISTINCT sid) AS string_count, pid FROM {l10n_server_line} GROUP BY pid");
        while ($row = db_fetch_object($result)) {
          $sums[$row->pid] = array('count' => $row->string_count);
        }
        // Get the count of distinct strings translated per project.
        $count_sql = "SELECT COUNT(DISTINCT ts.sid) AS translation_count, l.pid FROM {l10n_server_line} l LEFT JOIN {l10n_server_status_flag} ts ON l.sid = ts.sid WHERE ts.has_translation = 1 ";
        if (isset($id)) {
          // Limit to language if desired.
          $count_sql .= "AND ts.language = '%s' ";
          $count_args[] = $id;
        }
        $count_sql .= 'GROUP BY l.pid';
        $result = db_query($count_sql, $count_args);
        while ($row = db_fetch_object($result)) {
          $sums[$row->pid]['translations'] = $row->translation_count;
        }
        // Get the count of distinct strings having suggestions per project.
        $count_sql = "SELECT COUNT(DISTINCT ts.sid) AS translation_count, l.pid FROM {l10n_server_line} l LEFT JOIN {l10n_server_status_flag} ts ON l.sid = ts.sid WHERE ts.has_suggestion = 1 ";
        if (isset($id)) {
          // Limit to language if desired.
          $count_sql .= "AND ts.language = '%s' ";
          $count_args[] = $id;
        }
        $count_sql .= 'GROUP BY l.pid';
        $result = db_query($count_sql, $count_args);
        while ($row = db_fetch_object($result)) {
          $sums[$row->pid]['suggestions'] = $row->translation_count;
        }
        cache_set('l10n:count:projects:'. $id, $sums, 'cache', CACHE_PERMANENT);
        return $sums;
      }
      break;

    case 'top-people':
      // Get summaries of people having most active translations per language.
      // Skip anonymous since that is used for placeholders when there was
      // no prior translations for a suggestion.
      $result = db_query_range("SELECT COUNT(DISTINCT t.sid) AS sum, u.name, u.uid FROM {l10n_server_translation} t LEFT JOIN {users} u ON t.uid_entered = u.uid WHERE t.is_active = 1 AND t.is_suggestion = 0 AND t.language = '%s' AND u.uid != 0 GROUP BY t.uid_entered ORDER BY sum DESC", $id, 0, 10);
      $accounts = array();
      while ($account = db_fetch_object($result)) {
        $accounts[] = $account;
      }
      return $accounts;
  }
}
