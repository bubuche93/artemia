<?php
// $Id: translate.inc,v 1.1.2.7.2.31.2.23 2010/03/26 09:15:42 goba Exp $

/**
 * @file
 *   Translation view and editing pages for localization community.
 */

// = Filter form handling ======================================================

/**
 * Translate form filter.
 *
 * @param $langcode
 *   Language code
 * @param $filters
 *   Array of filter options.
 */
function l10n_community_filter_form(&$form_state, $langcode, $filters) {
  $projects = l10n_server_get_projects();

  $translation_options = array(
    L10N_STATUS_ALL            => '<'. t('Any') .'>',
    L10N_STATUS_UNTRANSLATED   => t('Untranslated'),
    L10N_STATUS_TRANSLATED     => t('Translated'),
    L10N_STATUS_IS_TRANSLATION => t('Is translation'),
  );
  $suggestion_options = array(
    L10N_STATUS_ALL            => '<'. t('Any') .'>',
    L10N_STATUS_NO_SUGGESTION  => t('Has no suggestion'),
    L10N_STATUS_HAS_SUGGESTION => t('Has suggestion'),
    L10N_STATUS_IS_SUGGESTION  => t('Is suggestion'),
  );

  // This form can be submitted multiple times to support AHAH flows.
  // So we set an initial default value set for the form values, if there
  // was none submitted already. Otherwise, the submitted values will take
  // on being the defaults.
  if (empty($form_state['values'])) {
    $form_state['values'] = array(
      'project' => isset($filters['project']) ? $filters['project']->title : '',
      'release' => isset($filters['release']) ? $filters['release']        : 'all',
      'context' => isset($filters['context']) ? $filters['context']        : 'all',
      'status'  => isset($filters['status'])  ? $filters['status']         : L10N_STATUS_ALL,
      'author'  => isset($filters['author'])  ? $filters['author']->name   : '',
      'search'  => isset($filters['search'])  ? $filters['search']         : '',
      'limit'   => isset($filters['limit'])   ? $filters['limit']          : 10,
    );
  }

  $form = array();
  $form['project'] = array(
    '#title' => t('Project'),
    '#default_value' => $form_state['values']['project'],
    '#ahah' => array(
      // 'event' missing, Drupal will apply the per-#type defaults.
      'path' => 'translate/releases-ahah',
      'wrapper' => 'l10n-server-releases',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => empty($form_state['values']['project']) ? 'is-default' : 'is-not-default'),
  );
  if (($count = count($projects)) <= 30) {
    // Select widget for 1-30 projects.
    $form['project']['#type'] = 'select';
    $form['project']['#options'] = array('' => t('All'));
    foreach ($projects as $project) {
      // URI used to shorten the lookup cycle in filter sanitization.
      $form['project']['#options'][$project->title] = $project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/project-autocomplete',
      '#size' => 20,
    );
  }

  // Always add releases option, so we can update it when
  // needed as the project changes with AHAH.
  $release_options = array('all' => t('All'));
  if (!empty($form_state['values']['project'])) {
    $selected_project = $projects[l10n_community_project_uri_by_title($form_state['values']['project'])];
    $releases = l10n_server_get_releases($selected_project->uri);
    foreach ($releases as $rid => $release) {
      $release_options[$rid] = $release->title;
    }
  }
  if (!isset($release_options[$form_state['values']['release']])) {
    $form_state['values']['release'] = 'all';
  }
  $form['release'] = array(
    '#title' => t('Release'),
    '#type' => 'select',
    '#options' => $release_options,
    '#default_value' => $form_state['values']['release'],
    // Wrapper used to replace release options via AHAH.
    '#prefix' => '<div id="l10n-server-releases">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => $form_state['values']['release'] == 'all' ? 'is-default' : 'is-not-default'),
  );

  if (count($contexts = l10n_community_get_contexts()) > 1) {
    $form['context'] = array(
      '#title' => t('Context'),
      '#type' => 'select',
      '#options' => array('all' => t('All')) + $contexts,
      '#default_value' => $form_state['values']['context'],
      '#attributes' => array('class' => $form_state['values']['context'] == 'all' ? 'is-default' : 'is-not-default'),
    );
  }

  $form['status'] = array(
    '#title' => t('Status'),
    '#tree' => TRUE,
    '#prefix' => '<div class="'. ($form_state['values']['status'] == L10N_STATUS_ALL ? 'is-default' : 'is-not-default') .'">',
    '#suffix' => '</div>',
    '#display_none' => $form_state['values']['status'] == L10N_STATUS_ALL,
  );
  $form['status']['translation'] = array(
    '#type' => 'select',
    '#options' => $translation_options,
    '#default_value' => $form_state['values']['status'] & (L10N_STATUS_TRANSLATED | L10N_STATUS_UNTRANSLATED | L10N_STATUS_IS_TRANSLATION),
  );
  $form['status']['suggestion'] = array(
    '#type' => 'select',
    '#options' => $suggestion_options,
    '#default_value' => $form_state['values']['status'] & (L10N_STATUS_HAS_SUGGESTION | L10N_STATUS_NO_SUGGESTION | L10N_STATUS_IS_SUGGESTION),
  );

  $form['author']= array(
    '#type' => 'textfield',
    '#title' => t('Submitted by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $form_state['values']['author'],
    '#size' => 15,
    '#attributes' => array('class' => empty($form_state['values']['author']) ? 'is-default' : 'is-not-default'),
    '#display_none' => empty($form_state['values']['author']),
  );

  $form['search'] = array(
    '#title' => t('Contains'),
    '#type' => 'textfield',
    '#default_value' => $form_state['values']['search'],
    '#size' => 20,
    '#attributes' => array('class' => empty($form_state['values']['search']) ? 'is-default' : 'is-not-default'),
    '#display_none' => empty($form_state['values']['search']),
  );

  $form['limit'] = array(
    '#type' => 'select',
    '#title' => t('Limit'),
    '#options' => drupal_map_assoc(array(5, 10, 20, 30)),
    '#default_value' => $form_state['values']['limit'],
    '#attributes' => array('class' => $form_state['values']['limit'] == 10 ? 'is-default' : 'is-not-default'),
    '#display_none' => $form_state['values']['limit'] == 10,
  );

  $form['submit'] = array(
    '#value' => t('Filter'),
    '#type' => 'submit',
  );
  $form['reset'] = array(
    '#value' => '<div class="reset-link">'. l(t('Clear all filters'), 'translate/languages/'. $langcode .'/translate', array('attributes' => array('class' => 'filter-exclude'))) .'</div>',
    '#type' => 'markup',
  );
  $form['#theme'] = 'l10n_community_filter_form';
  return $form;
}

/**
 * Theme function for l10n_community_filter_form.
 */
function theme_l10n_community_filter_form($form) {

  $output = '<div class="l10n-server-filter clear-block">';

  // Only display these elements individually.
  $elements = array('project', 'release', 'context', 'status', 'author', 'search', 'limit');
  foreach ($form as $id => &$element) {
    if (in_array($id, $elements)) {
      $title = $element['#title'];
      unset($element['#title']);

      // Bubble up a js-hide class to the container.
      $class = '';
      if (!empty($element['#display_none'])) {
        $class = ' js-hide';
      }

      $output .= '<div class="filter-widget'. $class .'"><label>'. $title .'</label>'. drupal_render($element) .'</div>';
    }
  }

  // Render the rest of the form as one filter-widget.
  $output .= '<div class="filter-submit">'. drupal_render($form) .'</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Submission handler for filtering form.
 */
function l10n_community_filter_form_submit($form, &$form_state) {

  if ($form_state['values']['op'] == t('Reset')) {
    // Just go with the redirection flow => removes URL params.
    return;
  }

  if ($form_state['values']['op'] == t('Filter')) {
    $filters = l10n_community_build_filter_values($form_state['values']);
    // Redirect keeping the relevant filters intact in the URL.
    $form_state['redirect'] = array($_GET['q'], l10n_community_flat_filters($filters));
  }
}

// = Translation form building =================================================

/**
 * Menu callback: List translations and suggestions.
 */
function l10n_community_translate_page($langcode) {
  drupal_add_css(drupal_get_path('module', 'l10n_community') .'/editor.css');
  drupal_add_js(drupal_get_path('module', 'l10n_community') .'/jquery.worddiff.js');
  drupal_add_js(drupal_get_path('module', 'l10n_community') .'/editor.js');
  drupal_add_js(drupal_get_path('module', 'l10n_community') .'/l10n_community.js');

  $language = l10n_community_get_language($langcode);
  $filters = l10n_community_build_filter_values($_GET);
  l10n_community_add_url_modifiers($langcode, $filters);
  $strings = l10n_community_get_strings($langcode, $filters, $filters['limit']);

  // Set the most appropriate title.
  if ($filters['project']) {
    drupal_set_title(t('Translate %project to @language', array('%project' => $filters['project']->title, '@language' => $language->name)));
  }
  else {
    drupal_set_title(t('Translate to @language', array('@language' => $language->name)));
  }

  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore languages'), 'translate/languages'),
      l($language->name, 'translate/languages/'. $langcode)
    )
  );

  // Add the filter form.
  $output = drupal_get_form('l10n_community_filter_form', $langcode, $filters);

  // Output the actual strings.
  if (!count($strings)) {
    drupal_set_message(t('No strings found with this filter. Try adjusting the filter options.'));
  }
  else {
    $output .= drupal_get_form('l10n_community_translate_form', $language, $filters, $strings);
  }
  return $output;
}

/**
 * Form callback: List translations and suggestions.
 *
 * @param $form_state
 *   The form state array.
 * @param $language
 *   A language object.
 * @param $filters
 *   An array of filters applied to the strings.
 * @param $strings
 *   The strings to render.
 */
function l10n_community_translate_form(&$form_state, $language, $filters, $strings) {
  $pager = theme('pager', NULL, $filters['limit'], 0);
  $redirect_url = $_GET;
  unset($redirect_url['q']);

  // The form will show but the submit buttons will only appear if the user has
  // permissions to submit suggestions. This allows the use of this form to review
  // strings in the database.
  $form = array(
    '#redirect' => array(
      $_GET['q'],
      $redirect_url
    ),
    'langcode' => array(
      '#type' => 'value',
      '#value' => $language->language
    ),
    'pager_top' => array(
      '#weight' => -10,
      '#value' => $pager
    ),
    'strings' => array(
      '#tree' => TRUE,
      '#theme' => 'l10n_community_translate_table'
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
      '#access' => user_access('submit suggestions')
    ),
    'pager_bottom' => array(
      '#weight' => 10,
      '#value' => $pager
    ),
  );

  // Fill in string values for the editor.
  foreach ($strings as $string) {
    $form['strings'][$string->sid] = _l10n_community_translate_form_item($form_state, $string, $language, $filters);
  }

  return $form;
}

/**
 * Creates the form fragment for a source string.
 */
function _l10n_community_translate_form_item(&$form_state, $source, $language, $filters) {

  if (!$source->translation) {
    // If there is no picked translation yet, simulate with a visible placeholder.
    $source->tid = '0';
    $source->translation = array(t('(not translated)'));
    $source->is_active = '1';
    $source->is_suggestion = '0';
  }
  else {
    $source->translation = l10n_community_unpack_string($source->translation);
  }
  $source_unpacked = $source->value;
  $source->value = l10n_community_unpack_string($source->value);

  $form = array(
    '#string' => $source,
    '#langcode' => $language->language,
    'filters_match' => array(
      '#type' => 'value',
      '#value' => ($source_match = (!empty($filters['search']) && (stripos($source_unpacked, $filters['search']) !== FALSE))),
    ),
    'source' => array(
      'string' => array('#value' => _l10n_community_translate_render_textarray($source->value)),
    ),
  );

  if (user_access('submit suggestions')) {
    $form['source']['edit'] = array(
      '#value' => t('Translate'),
      '#prefix' => '<label title="'. t('Translate') .'">',
      '#suffix' => '</label>',
    );
  }

  // Add the current string (either as approved translation or a mock object
  // for the "untranslated" string).
  $form[$source->tid] = _l10n_community_translate_translation($form_state, $source, $source, $filters, $source_match);

  // When there are suggestions, load them from the database.
  if ($source->has_suggestion) {
    $result = db_query("SELECT t.tid, t.sid, t.translation, t.uid_entered, t.time_entered, t.is_active, t.is_suggestion, u.name as username FROM {l10n_server_translation} t LEFT JOIN {users} u ON u.uid = t.uid_entered WHERE t.language = '%s' AND t.sid = %d AND t.is_active = 1 AND t.is_suggestion = 1 ORDER BY t.time_entered", $language->language, $source->sid);
    while ($suggestion = db_fetch_object($result)) {
      $suggestion->translation = l10n_community_unpack_string($suggestion->translation);
      // Add the suggestion to the list.
      $form[$suggestion->tid] = _l10n_community_translate_translation($form_state, $suggestion, $source, $filters, $source_match);
    }
  }

  // If the user may add new suggestions, display a textarea.
  if (user_access('submit suggestions')) {
    $textarea = _l10n_community_translate_translation_textarea($source, $language->plurals);
    $form[$textarea->tid] = _l10n_community_translate_translation($form_state, $textarea, $source, $filters, $source_match);
  }
  return $form;
}

/**
 * Generate markup for an unpacked string.
 *
 * @param $textarray
 *   An array of strings as generated by l10n_community_unpack_string().
 * @param $empty
 *   Specific data to include as the data to use when empty.
 */
function _l10n_community_translate_render_textarray($textarray, $empty = '') {
  // data-empty is a proprietary attribute used in editor.css to be displayed when
  // starting to submit a new suggestion.
  $empty = !empty($empty) ? ' data-empty="'. check_plain($empty) .'"' : '';
  return "<span$empty>". implode("</span><br /><span$empty>", array_map('check_plain', $textarray)) .'</span>';
}

/**
 * Build mock object for new textarea.
 */
function _l10n_community_translate_translation_textarea($source, $nplurals) {
  global $user;

  return (object) array(
    'sid' => $source->sid,
    'tid' => 'new',
    // Fill in with as many items as required. If the source was plural, we
    // need to fill in with a number adequate for this language.
    'translation' => array_fill(0, count($source->value) > 1 ? $nplurals : 1, ''),
    'is_active' => '1',
    'is_suggestion' => '1',
    'uid_entered' => $user->uid,
  );
}

/**
 * Creates the form fragment for a translated string.
 */
function _l10n_community_translate_translation(&$form_state, $string, $source, $filters, $source_match) {
  global $user;

  $is_own = $user->uid == $string->uid_entered;
  $is_active = $string->is_active && !$string->is_suggestion;
  $is_new = $string->tid == 'new';
  $may_moderate = ($is_own ? user_access('moderate own suggestions') : user_access('moderate suggestions from others'));

  // This string is a match if it was data from the database and was entered by
  // the searched user or its text included the searched portion.
  $filters_matched = array();
  $filters_to_match = 0;
  if ((int) $string->tid > 0) {
    // If we had a real tid for this, it has a uid_entered and translation loaded.
    if (!empty($filters['author'])) {
      $filters_matched[] = $string->uid_entered == $filters['author']->uid;
      $filters_to_match++;
    }
    if (!empty($filters['search'])) {
      // Search is matched if the source matched the search and the translation
      // matched the user or the translation matched both.
      $filters_matched[] = (!empty($filters['author']) && $source_match) || (stripos(l10n_community_pack_string($string->translation), $filters['search']) !== FALSE);
      $filters_to_match++;
    }
    $filters_matched = array_filter($filters_matched);
  }

  $form = array(
    '#theme' => 'l10n_community_translate_translation',
    'original' => array(
      '#type' => 'value',
      '#value' => $string,
    ),
    'filters_match' => array(
      '#type' => 'value',
      '#value' => ($filters_to_match > 0) && (count($filters_matched) == $filters_to_match),
    ),
  );

  // Active radio box is used to pick the approved translation.
  $form['active'] = array(
    '#type' => 'radio',
    '#theme' => 'l10n_community_translate_radio',
    '#title' => _l10n_community_translate_render_textarray($string->translation, $is_new ? t('(empty)') : FALSE),
    '#return_value' => $string->tid,
    '#default_value' => $is_active ? $string->tid : NULL,
    '#parents' => array('strings', $string->sid, 'active'),
    // Let a moderator roll back to the current translation even if they
    // would otherwise not have permission to approve such a string.
    '#disabled' => !$may_moderate && !$is_active,
    '#attributes' => array('class' => 'selector'),
  );

  if ($string->tid) {
    if ($may_moderate && $string->tid != 'new') {
      $form['declined'] = array(
        '#type' => 'checkbox',
        '#title' => t('Decline'),
        '#default_value' => !($string->is_active || $string->is_suggestion),
      );
    }
    if ($string->tid == 'new') {
      // Fill in with as many textareas as required to enter translation
      // for this string.
      $form['value'] = array_fill(0, count($string->translation), array(
        '#type' => 'textarea',
        '#cols' => 60,
        '#rows' => 3,
        '#default_value' => t('<New translation>'),
      ));
    }
    else {
      if (user_access('submit suggestions')) {
        $form['edit'] = array(
          '#value' => t('Edit a copy'),
          '#prefix' => '<label title="'. t('Edit a copy') .'">',
          '#suffix' => '</label>',
        );
      }
      if (isset($string->username)) {
        $title = l10n_community_translate_byline($string);
        $form['author'] = array(
          '#value' => $title,
        );
      }
    }
  }
  return $form;
}

/**
 * Generates the byline containing meta information about a string.
 */
function l10n_community_translate_byline($string) {
  $params = array(
    '!author' => theme('username', (object)array('name' => $string->username, 'uid' => $string->uid_entered)),
    '@date' => format_date($string->time_entered),
    '@ago' => t('@time ago', array('@time' => format_interval(time() - $string->time_entered))),
  );
  if (!empty($string->uid_approved) && $string->uid_entered === $string->uid_approved && $string->time_entered === $string->time_approved) {
    return t('translated and approved by !author <span title="@ago">on @date</span>', $params);
  }
  else {
    $title = t('suggested by !author <span title="@ago">on @date</span>', $params);
    if (!empty($string->uid_approved)) {
      $title .= '<br />'. t('approved by !author <span title="@ago">on @date</span>', array(
        '!author' => theme('username', (object)array('name' => $string->username_approved, 'uid' => $string->uid_approved)),
        '@date' => format_date($string->time_approved),
        '@ago' => t('@time ago', array('@time' => format_interval(time() - $string->time_approved))),
      ));
    }
    return $title;
  }
}

// = Translation form theming ==================================================

/**
 * Main theme function for translation table.
 */
function theme_l10n_community_translate_table($element) {
  $header = array(
    t('Source Text'),
    t('Translations'),
  );

  $rows = array();

  foreach (element_children($element) as $key) {
    $rows[] = array(
      array('class' => 'source', 'data' => theme('l10n_community_translate_source', $element[$key])),
      array('class' => 'translation', 'data' => theme('l10n_community_translate_translation_list', $element[$key])),
    );
  }

  return theme('table', $header, $rows, array('class' => 'l10n-table'));
}

/**
 * Theme function for a source cell in the translation table.
 */
function theme_l10n_community_translate_source($element) {
  $output = theme('l10n_community_translate_actions', $element['source']);
  $output .= '<label class="l10n-string'. ($element['filters_match']['#value'] ? ' filter-match' : ' filter-no-match') .'">'. drupal_render($element['source']['string']) .'</label>';
  $output .= theme('l10n_community_in_context', $element['#string']);
  $output .= '<div class="l10n-usage"><a href="'. url('translate/details/'. $element['#langcode'] .'/'. $element['#string']->sid) .'" class="l10n-more-link" title="'. t('Show list of projects and releases where this text is used.') .'">'. t('Show related projects') .'</a><div class="l10n-more-info"></div></div>';
  return $output;
}

/**
 * Theme appropriate actions for the given string element.
 */
function theme_l10n_community_translate_actions($element) {
  $actions = '';
  foreach (array('declined', 'edit') as $type) {
    if (isset($element[$type])) {
      $actions .= '<li class="'. $type .'">'. drupal_render($element[$type]) .'</li>';
    }
  }
  if (!empty($actions)) {
    return '<ul class="actions">'. $actions .'</ul>';
  }
  else {
    return '';
  }
}

/**
 * Theme context information for source strings.
 *
 * @param $string
 *   Source string object.
 */
function theme_l10n_community_in_context($string) {
  if (!empty($string->context)) {
    return '<div class="string-context">'. t('in context: @context', array('@context' => $string->context)) .'</div>';
  }
  return '';
}

/**
 * Theme a suggestion/translation.
 */
function theme_l10n_community_translate_translation($element) {
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = '';
  }
  $element['#attributes']['class'] .= ' clearfix translation';

  // Add is-selectable and is-declinable helpers for JS.
  if (!$element['active']['#disabled']) {
    $element['#attributes']['class'] .= ' is-selectable';
  }
  if (isset($element['declined'])) {
    $element['#attributes']['class'] .= ' is-declinable';
  }

  // Add information on whether this matched the filter.
  if ((int) $element['active']['#return_value'] > 0) {
    $element['#attributes']['class'] .= ($element['filters_match']['#value']) ? ' filter-match' : ' filter-no-match';
  }

  switch ($element['active']['#return_value']) {
    case 'new':
      $element['#attributes']['class'] .= ' new-translation';
      break;
    case '0':
      $element['#attributes']['class'] .= ' no-translation';
      // Fallthrough.
    default:
      if ($element['active']['#value'] !== '') {
        $element['#attributes']['class'] .= ' is-active default';
      }
  }
  $element['#attributes']['class'] = trim($element['#attributes']['class']);

  $output = '<li'. drupal_attributes($element['#attributes']) .'>';
  $output .= theme('l10n_community_translate_actions', $element);
  // Add the radio box to pick the active translation.
  $output .= drupal_render($element['active']);

  if (isset($element['author'])) {
    $output .= '<div class="author">'. drupal_render($element['author']) .'</div>';
  }

  if (isset($element['value'])) {
    $output .= drupal_render($element['value']);
  }
  return $output  .'</li>';
}

/**
 * Theme a radio button to pick the active translation.
 */
function theme_l10n_community_translate_radio($element) {
  _form_set_class($element, array('form-radio'));
  $output = '<input type="radio" ';
  $output .= 'id="'. $element['#id'] .'" ';
  $output .= 'name="'. $element['#name'] .'" ';
  $output .= 'value="'. $element['#return_value'] .'" ';
  $output .= (check_plain($element['#value']) == $element['#return_value']) ? ' checked="checked" ' : ' ';
  $output .= drupal_attributes($element['#attributes']) .' />';

  if (isset($element['#title'])) {
    $output .= '<label class="l10n-string" for="'. $element['#id'] .'">'. $element['#title'] .'</label>';
  }
  return $output;
}

/**
 * Theme a list of suggestions for a string.
 */
function theme_l10n_community_translate_translation_list($element) {
  $output = '<ul>';
  foreach (element_children($element) as $child) {
    if (is_numeric($child) || $child == 'new') {
      $output .= drupal_render($element[$child]);
    }
  }
  $output .= '</ul>';
  return $output;
}

// = Translation form submission ===============================================

/**
 * Form submit callback for l10n_community_translate_form().
 *
 * @see l10n_community_translate_form().
 */
function l10n_community_translate_form_submit($form, &$form_state) {
  global $user;

  $langcode = $form_state['values']['langcode'];

  // Iterate outer structure built in l10n_community_translate_form().
  foreach ($form_state['values']['strings'] as $sid => $string) {

    // Iterate inner structure built in _l10n_community_translate_form_item().
    // Form items have numeric $tid values and other keys here.
    foreach ($string as $tid => $options) {

      // Store new suggestion.
      $empty_values = 0;
      // $options['value'] is the result of (a series of) textareas.
      if (isset($options['value']) && is_array($options['value'])) {
        foreach ($options['value'] as $key => $value) {
          if ($value === t('<New translation>')) {
            $options['value'] = '';
            $empty_values++;
          }
        }
        // If we had value in any of the textareas, add new suggestion.
        if ($tid === 'new' && count($options['value']) > $empty_values) {
          $tid = l10n_community_add_suggestion($sid, l10n_community_pack_string($options['value']), $langcode, $user->uid);
          if ($tid) {
            if ($string['active'] === 'new') {
              // This new string was selected to be approved, so remember $tid
              // for later, so we can save this as an approved translation.
              $string['active'] = $tid;
              l10n_community_counter(L10N_COUNT_ADDED);
            }
            else {
              l10n_community_counter(L10N_COUNT_SUGGESTED);
            }
          }
          elseif ($tid === FALSE) {
            // We found this as an active string already in the DB.
            l10n_community_counter(L10N_COUNT_DUPLICATE);
          }
        }
      }

      if (is_numeric($tid) && $tid > 0) {
        if ($tid == $string['active']) {
          if ($options['original']->is_suggestion) {
            // $tid is a suggestion that was made active.
            l10n_community_approve_string($langcode, $sid, $tid);
            l10n_community_counter(L10N_COUNT_APPROVED);
          }
        }
        elseif (!empty($options['declined'])) {
          // The decline checkbox for this suggestion was checked.
          l10n_community_counter($options['original']->is_suggestion ? L10N_COUNT_SUGGESTION_DECLINED : L10N_COUNT_DECLINED);
          l10n_community_decline_string($langcode, $sid, $tid);
        }
      }
    }
  }

  // Tell the user what happened.
  l10n_community_update_message();
}

// = API functions =============================================================

/**
 * Get strings under some conditions.
 *
 * @param $langcode
 *   Language code to use for the lookup.
 * @param $filters
 *   - 'project'
 *     Project object to look up strings for.
 *   - 'status'
 *     Filter strings by status. See L10N_STATUS_ALL,
 *     L10N_STATUS_UNTRANSLATED, L10N_STATUS_HAS_SUGGESTION and
 *     L10N_STATUS_TRANSLATED.
 *   - 'release'
 *     Release id of the particular project release to filter with.
 *     Use NULL to not filter on releases.
 *   - 'search'
 *     Substring to search for in all source and translation strings.
 *   - 'context'
 *     From Drupal 7, separate contexts are supported. POTX_CONTEXT_NONE is
 *     the default, if the code does not specify a context otherwise.
 * @param $pager
 *   Number of strings to be returned in a pager. Should be NULL if
 *   no pager should be used.
 * @return
 *   An array of string records from database.
 */
function l10n_community_get_strings($langcode, $filters, $pager = NULL) {
  $join = $join_args = $where = $where_args = array();
  $sql = $sql_count = '';

  $select = "SELECT DISTINCT s.sid, s.value, s.context, t.tid, t.language, t.translation, t.uid_entered, t.uid_approved, t.time_entered, t.time_approved, t.is_suggestion, t.is_active, u.name as username, u2.name as username_approved, ts.has_suggestion, ts.has_translation FROM {l10n_server_string} s";
  $select_count = "SELECT COUNT(DISTINCT(s.sid)) FROM {l10n_server_string} s";
  $join[] = "LEFT JOIN {l10n_server_status_flag} ts ON s.sid = ts.sid AND ts.language = '%s'";
  $join_args[] = $langcode;
  $join[] = "LEFT JOIN {l10n_server_translation} t ON ts.sid = t.sid AND ts.language = t.language AND t.is_active = 1";
  $join[] = "LEFT JOIN {users} u ON u.uid = t.uid_entered";
  $join[] = "LEFT JOIN {users} u2 ON u2.uid = t.uid_approved";

  // Add submitted by condition
  if (!empty($filters['author'])) {
    $where[] = "t.uid_entered = %d";
    $where_args[] = $filters['author']->uid;
  }

  // Release restriction.
  $release = empty($filters['release']) || $filters['release'] === 'all' ? NULL : $filters['release'];
  $project = $filters['project'];
  if ($release || $project) {
    $join[] = "INNER JOIN {l10n_server_line} l ON s.sid = l.sid";
    // If we have a release we ignore the project
    if ($release) {
      // Release restriction.
      $where_args[] = $release;
      $where[] = 'l.rid = %d';
    }
    elseif ($project) {
      $where[] = "l.pid = %d";
      $where_args[] = $project->pid;
    }
  }

  // Context based filtering.
  if (isset($filters['context']) && $filters['context'] != 'all') {
    // We use 'none' for no context, so '' can be the defaut (for all contexts).
    $where_args[] = $filters['context'] == 'none' ? '' : $filters['context'];
    $where[] = "s.context = '%s'";
  }

  if (!empty($filters['search'])) {
    // Search in the source or target strings.
    $where_args[] = $filters['search'];
    $where_args[] = $filters['search'];
    $where[] = "(s.value LIKE '%%%s%%' OR t.translation LIKE '%%%s%%')";
  }

  // Restriction based on string status by translation / suggestions.
  if (isset($filters['status'])) {
    if ($filters['status'] & L10N_STATUS_UNTRANSLATED) {
      $where[] = "(ts.has_translation = 0 OR ts.has_translation IS NULL)";
    }
    elseif ($filters['status'] & L10N_STATUS_TRANSLATED) {
      $where[] = "ts.has_translation = 1";
    }
    elseif ($filters['status'] & L10N_STATUS_IS_TRANSLATION) {
      $where[] = "t.is_suggestion = 0";
    }

    if ($filters['status'] & L10N_STATUS_HAS_SUGGESTION) {
      $where[] = "ts.has_suggestion = 1";
    }
    elseif ($filters['status'] & L10N_STATUS_NO_SUGGESTION) {
      $where[] = "(ts.has_suggestion = 0 OR ts.has_suggestion IS NULL)";
    }
    elseif ($filters['status'] & L10N_STATUS_IS_SUGGESTION) {
      $where[] = "t.is_suggestion = 1";
    }
  }

  // Build the queries
  $sql_args = array_merge($join_args, $where_args);
  $sql_where = implode(' ', $join) . (count($where) ? (' WHERE '. implode(' AND ', $where)) : '');
  $sql = $select .' '. $sql_where;
  $sql_count = $select_count .' '. $sql_where;

  // We either need a pager or a full result.
  if (isset($pager)) {
    $strings = pager_query($sql, $pager, 0, $sql_count, $sql_args);
  }
  else {
    $strings = db_query($sql, $sql_args);
  }
  $result = array();
  while ($string = db_fetch_object($strings)) {
    if ($string->is_suggestion) {
      // This string is not a translation, but we need that as a "parent" to display.
      if ($string->has_translation && ($translation = db_fetch_array(db_query("SELECT t.tid, t.translation, t.uid_entered, t.uid_approved, t.time_entered, t.time_approved, t.is_suggestion, t.is_active, u.name as username, u2.name as username_approved FROM {l10n_server_translation} t LEFT JOIN {users} u ON u.uid = t.uid_entered LEFT JOIN {users} u2 ON u2.uid = t.uid_approved WHERE t.language = '%s' AND t.sid = %d AND t.is_active = 1 AND t.is_suggestion = 0", $string->language, $string->sid)))) {
        // It does have a translation however, so let's load it, and override.
        foreach ($translation as $key => $value) {
          $string->$key = $value;
        }
      }
      else {
        // No parent translation. Pretend this does not exist.
        // The display code will call for the suggestions.
        $string->translation = '';
      }
    }
    $result[] = $string;
  }
  return $result;
}

/**
 * Unpacks a string as retrieved from the database.
 *
 * Creates an array out of the string. If it was a single string, the array
 * will have one item. If the string was a plural string, the array will have
 * as many items as the language requires (two for source strings).
 *
 * @param $string
 *   The string with optional separation markers (NULL bytes)
 * @return
 *   An array of strings with one element for each plural form in case of
 *   a plural string, or one element in case of a regular string. This
 *  is called a $textarray elsewhere.
 */
function l10n_community_unpack_string($string) {
  return explode("\0", $string);
}

/**
 * Packs a string for storage in the database.
 *
 * @param $string
 *   An array of strings.
 * @return
 *   A packed string with NULL bytes separating each string.
 */
function l10n_community_pack_string($strings) {
  return implode("\0", $strings);
}

// = AJAX callbacks ============================================================

/**
 * Return a HTML list of projects, releases and counts of where strings
 * appear in the managed projects.
 *
 * We could provide much more information, but usability should also be kept
 * in mind. It is possible to investigate hidden information sources though,
 * like tooltips on the release titles presented.
 *
 * This callback is invoked from JavaScript and is used as an AHAH provider.
 *
 * @param $langcode
 *   Language code.
 * @param $sid
 *   Source string id.
 * @param $ahah
 *   Whether the request came through AHAH, in which case no page
 *   theming should be applied.
 */
function l10n_community_string_details($langcode = NULL, $sid = 0, $ahah = 0) {

  $output = '';
  if ($ahah) {
    // Prevent devel module information if doing AHAH.
    $GLOBALS['devel_shutdown'] = FALSE;
  }
  else {
    // Get data about this source string.
    $string = db_fetch_object(db_query('SELECT value, context FROM {l10n_server_string} WHERE sid = %d', $sid));
    $unpacked = l10n_community_unpack_string($string->value);
    $output .= '<h3>'. t('Source string') .'</h3>'. theme('item_list', $unpacked);
    $output .= '<h3>'. (empty($string->context) ? t('Used at the following places') : t('Used at the following places in context %context', array('%context' => $string->context))) .'</h3>';
  }

  // List of project releases, where this string is used.
  $result = db_query('SELECT l.pid, p.title project_title, l.rid, r.title release_title, COUNT(l.lineno) as occurrence_count FROM {l10n_server_line} l INNER JOIN {l10n_server_project} p ON l.pid = p.pid INNER JOIN {l10n_server_release} r ON l.rid = r.rid WHERE l.sid = %d AND p.status = 1 GROUP BY l.rid ORDER BY l.pid, l.rid', $sid);

  $version_list = array();
  $project_list = array();
  $previous_project = '';
  while ($instance = db_fetch_object($result)) {
    $release_info = $instance->release_title .' <span title="'. format_plural($instance->occurrence_count, 'Appears once in this release.', 'Appears @count times in this release.') .'">('. $instance->occurrence_count .')</span>';
    if ($instance->project_title != $previous_project) {
      if (!empty($version_list)) {
        $project_list[] = join(', ', $version_list);
      }
      $version_list = array('<em>'. $instance->project_title .':</em> '. $release_info);
    }
    else {
      $version_list[] = $release_info;
    }
    $previous_project = $instance->project_title;
  }
  $project_list[] = join(', ', $version_list);
  $usage_list = theme('item_list', $project_list);
  if ($ahah) {
    print $usage_list;
    exit;
  }
  else {
    $output .= $usage_list;
    return $output;
  }
}
