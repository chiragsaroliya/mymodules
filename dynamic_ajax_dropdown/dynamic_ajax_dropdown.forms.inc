<?php

/**
 * @file
 * Product Review Forms.
 */

/**
 * Form Callback for admin/config/powerreviews.
 */
function dynamic_ajax_dropdown__forms_powerreviews_settings_form() {
  $form = array();
  $form[DYNAMIC_AJAX_DROPDOWN_VAR_PR_APPLAUNCH_JS_ENDPOINT] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview AppLaunch JS Endpoint'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_APPLAUNCH_JS_ENDPOINT, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_VAR_PR_MERCHANT_GROUP_ID] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview Merchant Group ID'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_MERCHANT_GROUP_ID, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_VAR_PR_MERCHANT_ID] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview Merchant ID'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_MERCHANT_ID, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_VAR_PR_SITE_ID] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview Site ID'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_SITE_ID, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_VAR_PR_LOCALE] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview Locale'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_LOCALE, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_VAR_PR_SNIPPETS_ENGINE_JS] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview Snippets engine End point'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_SNIPPETS_ENGINE_JS, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_API_KEY] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview API Key'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_API_KEY, ''),
    '#required' => TRUE,
  );
  $form[DYNAMIC_AJAX_DROPDOWN_JS] = array(
    '#type' => 'textfield',
    '#title' => t('PowerReview Snippets stable End point'),
    '#default_value' => variable_get(DYNAMIC_AJAX_DROPDOWN_JS, ''),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Form Callback for product/writereview.
 */
function dynamic_ajax_dropdown_writeareview_form($form, &$form_state) {
  drupal_add_js(array('dynamic_ajax_dropdown' => array('writeareview' => '1')), array('type' => 'setting'));
  $parameters = drupal_get_query_parameters();
  $product_category_name_options = product_category_name_options();
  $flag = 0;
  if (isset($parameters['page_id']) && $flag == 0) {
    $selected = isset($parameters['page_id']) ? $parameters['page_id'] : key($product_category_name_options);
    $flag = 1;
  }
  else {
    $selected = isset($form_state['values']['product_category_name']) ? $form_state['values']['product_category_name'] : key($product_category_name_options);
  }
  $form['#prefix'] = '<div class="war-form">';
  $form['#suffix'] = '</div>';
  $form['title_name'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="war-title-name"><h1>Write a Review</h1></div>',
  );
  $form['product_category_name'] = array(
    '#type' => 'select',
    '#title' => t('Product category'),
    '#options' => $product_category_name_options,
    '#default_value' => $selected,
    '#attributes' => array('class' => array('product-category-name')),
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => 'product-variant-wrapper',
      'callback' => 'ajax_view_display_dropdown_callback',
      'method' => 'replace',
    ),
  );
  if ($flag == 1) {
    $form['product_category_name']['#disabled'] = TRUE;
  }
  if ($selected != 'none') {
    $product_variant_options = product_variant_name_options($selected);
  }
  else {
    $product_variant_options = array('' => '');
  }
  $form['product_variant_name'] = array(
    '#type' => 'select',
    '#title' => t('Product variant'),
    '#prefix' => '<div id="product-variant-wrapper">',
    '#suffix' => '</div>',
    '#options' => $product_variant_options,
    '#default_value' => isset($form_state['values']['product_variant_name']) ? $form_state['values']['product_variant_name'] : '',
    '#attributes' => array('class' => array('product-variant-name')),
  );
  $form['pr_write'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="pr-write"></div>',
  );

  return $form;
}

/**
 * Helper function to get dropdown values for product_category_name.
 */
function product_category_name_options() {
  $product_category = array('none' => '--Select--');
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term')
    ->fieldCondition('field_product_type', 'value', 'category');
  $results = $query->execute();
  foreach ($results['taxonomy_term'] as $tid => $v) {
    $term = taxonomy_term_load($v->tid);
    if (!empty($term->field_war_display_name[LANGUAGE_NONE][0]['value'])) {
      $product_category[$term->field_page_id[LANGUAGE_NONE][0]['value']] = $term->field_war_display_name[LANGUAGE_NONE][0]['value'];
    }
  }
  return $product_category;
}

/**
 * Ajax callback function.
 */
function ajax_view_display_dropdown_callback($form, $form_state) {
  return $form['product_variant_name'];
}

/**
 * Helper function to get dropdown values for product_variant_name.
 */
function product_variant_name_options($page_id) {
  $product_variant = FALSE;
  if (!empty($page_id)) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'taxonomy_term')
      ->fieldCondition('field_page_id', 'value', $page_id)
      ->fieldCondition('field_product_type', 'value', 'variant');
    $results = $query->execute();
    if (!empty($results)) {
      $product_variant = array('' => '--Select--');
      foreach ($results['taxonomy_term'] as $tid => $v) {
        $term = taxonomy_term_load($v->tid);
        $product_variant[$term->field_page_id_variant[LANGUAGE_NONE][0]['value']] = $term->name;
      }
    }
  }
  return $product_variant;
}

/**
 * Helper function to get power review config settings.
 */
function get_powerreiview_settings() {
  $powerreiview = array(
    'applaunch_js_endpoint' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_APPLAUNCH_JS_ENDPOINT, ''),
    'merchant_group_id' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_MERCHANT_GROUP_ID, ''),
    'merchant_id' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_MERCHANT_ID, ''),
    'site_id' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_SITE_ID, ''),
    'locale' => variable_get(DYNAMIC_AJAX_DROPDOWN_VAR_PR_LOCALE, DYNAMIC_AJAX_DROPDOWN_PR_LOCALE_DEFAULT_VAL_EN_US),
    'api_key' => variable_get(DYNAMIC_AJAX_DROPDOWN_API_KEY, ''),
    'powerreviews_endpoint_js' => variable_get(DYNAMIC_AJAX_DROPDOWN_JS, '//ui.powerreviews.com/stable/4.0/ui.js'),
  );
  drupal_add_js(array('dynamic_ajax_dropdown' => array('powerreiview' => $powerreiview)), array('type' => 'setting'));
  return $powerreiview;
}

/**
 * Helper function to get nested tree of vocabulary.
 *
 * @param int $terms
 *   $terms : Vocabulary id
 *   $max_depth : Maximum depth of an array.
 *
 * @return nested_terms
 *   Parent->child Nested array for given vocabulary.
 */
function taxonomy_get_nested_tree($terms = array(), $max_depth = NULL, $parent = 0, $parents_index = array(), $depth = 0) {
  if (is_numeric($terms)) {
    $terms = taxonomy_get_tree($terms);
  }
  foreach ($terms as $term) {
    foreach ($term->parents as $term_parent) {
      if ($term_parent == $parent) {
        $nested_terms[$term->tid] = $term;
      }
      else {
        $parents_index[$term_parent][$term->tid] = $term;
      }
    }
  }
  foreach ($nested_terms as &$term) {
    if (isset($parents_index[$term->tid]) && (is_null($max_depth) || $depth < $max_depth)) {
      $term->children = taxonomy_get_nested_tree($parents_index[$term->tid], $max_depth, $term->tid, $parents_index, $depth + 1);
    }
  }
  return $nested_terms;
}

/**
 * Helper function to get powerreview pageids of all subcategory in array.
 *
 * @return pr_mapping
 *   Mapping of pageid->subcategory_name.
 */
function get_subcategory_pageids() {
  $vocabulary = taxonomy_vocabulary_machine_name_load('power_review_category_and_variant_mapping');
  $tree = taxonomy_get_nested_tree($vocabulary->vid, 10);
  foreach ($tree as $prodkey => $prodval) {
    if (isset($prodval->children) && !empty($prodval->children)) {
      foreach ($prodval->children as $catkey => $catval) {
        $term = taxonomy_term_load($catval->tid);
        $category_page_id = $term->field_page_id[LANGUAGE_NONE][0]['value'];
        $pr_mapping[$prodval->name][$category_page_id] = $catval->name;
      }
    }
  }
  variable_set('subcategory_pageids_mapping', $pr_mapping);
  return $pr_mapping;
}

/**
 * Helper function to get powerreview pageid for given subcategory.
 *
 * Used in views-view--products--block-6.tpl.php.
 *
 * @param string $subcat_name
 *   $subcat_name : Name of subcategory
 *   $subcat_tid : (optional)subcategory term id.
 *
 * @return page_id
 *   powerreview pageid for given subcategory.
 */
function get_mapped_pageid_from_termid($subcat_tid, $subcat_name = FALSE) {
  if (isset($subcat_tid)) {
    $mainproduct_name = taxonomy_get_parents($subcat_tid);
    if (empty($subcat_name)) {
      $subcategory = taxonomy_term_load($subcat_tid);
      $subcat_name = $subcategory->name;
    }
    foreach ($mainproduct_name as $key => $val) {
      $parentname = $val->name;
    }
    if (isset($parentname) && !empty($parentname)) {
      $pr_mapping = variable_get('subcategory_pageids_mapping', get_subcategory_pageids());
      foreach ($pr_mapping as $pr_mappingkey => $pr_mappingval) {
        if ($pr_mappingkey == $parentname) {
          foreach ($pr_mappingval as $key => $val) {
            if ($val == $subcat_name) {
              $page_id = $key;
              return $page_id;
            }
          }
        }
      }
    }
    return FALSE;
  }
}
