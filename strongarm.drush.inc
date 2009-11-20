<?php
// $Id$

/**
 * @file
 * Drush integration for Strongarm. Provides commands to export variables.
 */

/**
 * Implementation of hook_drush_help().
 */
function strongarm_drush_help($section) {
  switch ($section) {
    case 'drush:strongarm':
      return dt("Export Drupal variables. Without any arguments a list of all variables will be shown. If arguments are specified they'll be var_exported to stdout.");
  }
}

/**
 * Implementation of hook_drush_command().
 */
function strongarm_drush_command() {
  $items['strongarm'] = array(
    'callback' => 'strongarm_drush_export',
    'description' => 'Export variable(s).',
  );
  return $items;
}

// Callback for strongarm command.
function strongarm_drush_export() {
  $vars = func_get_args();
  if (empty($vars)) {
    ctools_include('export');
    $rows = array(array('Variable name', 'Storage'));
    $vars = ctools_export_load_object('variable');
    ksort($vars);
    foreach ($vars as $name => $variable) {
      $default = ctools_get_default_object('variable', $name);
      if ($variable->export_type & EXPORT_IN_CODE) {
        $storage = ($variable->value != $default->value) ? 'Overridden' : 'Default';
      }
      else {
        $storage = ''; // We could print 'Normal' here but this will just add visual noise.
      }
      $rows[] = array($name, $storage);
    }
    drush_print_table($rows, TRUE);
  }
  else {
    $exports = array();
    foreach ($vars as $v) {
      // We use the very unlikely 'DRUSH_STRONGARM_NO_VALUE_HERE' as the 
      // default value which *should* be ok, right?
      $variable = variable_get($v, 'DRUSH_STRONGARM_NO_VALUE_HERE');
      if ($variable !== 'DRUSH_STRONGARM_NO_VALUE_HERE') {
        $exports[$v] = $variable;
      }
    }
    drush_print(var_export($exports));
  }
}
