<?php

/*
 * @file calback function for artwork admin management
 * 
 */


/**
 * @function call back to list all artowrk types and allow links for editing returns themed item linst
 */
function artwork_add_page() {
  $item = new stdClass();
  $items = array();
  $links = array();
  $link = array();
  $item = menu_get_item();
  $links = system_admin_menu_block($item);
  foreach( $links as $link) {
    $items[] = l($link['title'], $link['href'], $item['localized_options']) . ': ' . filter_xss_admin($link['description']);
  }
  return theme('item_list', array('items' => $items));
}

function artwork_add($type) {
  global $user;
  $types = array();
  $types = artwork_types();
  $type = isset($type) ? str_replace('-', '_', $type) : NULL;
  if(empty($types[$type])) {
    return MENU_NOT_FOUND;
  }
  $artwork = entity_get_controller('artwork')->create($type);
  drupal_set_title(t('Create @name', array('@name'=>$types[$type]->name)), PASS_THROUGH);
  return drupal_get_form($type . '_artwork_form', $artwork);
}