<?php
/**
 * @file
 * Variables for mysite_migrate module.
 */

/**
 * mysite_migrate module variables
 */
function mysite_migrate_variable_info($options) {
  $variables['password_inc'] = array(
    'type' => 'string',
    'title' => t('Password INC file path', array(), $options),
    'group' => 'mysite_password',
    'default' => 'sites/default/modules/custom/mysite_migrate/includes/password.inc',
  );
  return $variables;
}

/**
 * mysite_migrate module variable groups
 */
function mysite_migrate_variable_group_info() {
  $groups['mysite_password'] = array(
    'title' => t('mysite password settings'),
    'description' => t('mysite password settings'),
    'access' => 'administer site configuration',
  );
  return $groups;
}
