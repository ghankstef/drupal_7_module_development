<?php

/*
 * @file artwork.controller.inc
 * extends DrupalDefaultEntityController class with minimal load functions  (a la node_load/entity_load)
 */

class ArtworkController extends DrupalDefaultEntityController {
  /*
   * Create an artwork object setting aid, title and  type
   */

  public function create($type = '') {
    return (object) array(
          'aid' => '',
          'type' => $type,
          'title' => '',
    );
  }

  public function save($artwork) {
    $transaction = db_transaction();
    try {
      global $user;
      // Determine if we will be inserting a new artwork.
      $artwork->is_new = empty($artwork->aid);
      // Set the timestamp fields.
      if (empty($artwork->created)) {
        $artwork->created = REQUEST_TIME;
      }
      $artwork->changed = REQUEST_TIME;
      $artwork->revision_timestamp = REQUEST_TIME;
      $update_artwork = TRUE;
      // Give modules the opportunity to prepare field data for
      // saving.
      field_attach_presave('artwork', $artwork);
      if (!$artwork->is_new && !empty($artwork->revision) &&
          $artwork->vid) {
        $artwork->old_vid = $artwork->vid;
        unset($artwork->vid);
      }
      // If this is a new artwork...
      if ($artwork->is_new) {
        // Save the new artwork.
        drupal_write_record('artwork', $artwork);
        // Save the initial revision.
        $this->saveRevision($artwork, $user->uid);
        $op = 'insert';
      }
      else {
        // Save the updated artwork.
        drupal_write_record('artwork', $artwork, 'aid');
        if (!empty($artwork->revision)) {
          $this->saveRevision($artwork, $user->uid);
        }
        else {
          $this->saveRevision($artwork, $user->uid, TRUE);
          $update_artwork = FALSE;
        }
        $op = 'update';
      }
// If the revision ID is new or updated, save it to the artwork.
      if ($update_artwork) {
        db_update('artwork')
            ->fields(array('vid' => $artwork->vid))
            ->condition('aid', $artwork->aid)
            ->execute();
      }
      // Save fields.
      $function = 'field_attach_' . $op;
      $function('artwork', $artwork);
      module_invoke_all('entity_' . $op, $artwork, 'artwork');
      // Clear internal properties.
      unset($artwork->is_new);
      // Ignore slave server temporarily to give time for the saved
      // order to be propagated to the slave.
      db_ignore_slave();
      return $artwork;
    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('artwork', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  function saveRevision($artwork, $uid, $update = FALSE) {
    // Hold on to the artwork's original creator_uid but swap
    // in the revision's creator_uid for the momentary write.
    $temp_uid = $artwork->uid;
    $artwork->uid = $uid;
    if ($update) {
      drupal_write_record('artwork_revision', $artwork, 'vid');
    }
    else {
      drupal_write_record('artwork_revision', $artwork);
    }
    // Reset the order's creator_uid to the original value.
    $artwork->uid = $temp_uid;
  }

  public function delete($aids) {
    if (!empty($aids)) {
      $artworks = $this->load($aids, array());
      $transaction = db_transaction();
      try {
        db_delete('artwork')
            ->condition('aid', $aids, 'IN')
            ->execute();
        db_delete('artwork_revision')
            ->condition('aid', $aids, 'IN')
            ->execute();
        foreach ($artworks as $artwork_id => $artwork) {
          field_attach_delete('artwork', $artwork);
        }
        db_ignore_slave();
      } catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('artwork', $e, NULL, WATCHDOG_ERROR);
        return FALSE;
      }
      module_invoke_all('entity_delete', $artwork, 'artwork');
      // Clear the page and block and artwork caches.
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

}
