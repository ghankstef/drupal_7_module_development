<?php

/*
 * @file artwork.controller.inc
 * extends DrupalDefaultEntityController class with minimal load functions  (a la node_load/entity_load)
 */


class ArtworkController extends DrupalDefaultEntityController {
  
  /**
   * @function method for loading and artwork entity
   * @param type $aid  and artwork id 
   * @param type $vid -  artwork version id
   * @param type $reset - TRUE or FALSE
   * @return type an artwork  entity
   */
  function artwork_load($aid = NULL, $vid = NULL, $reset = FALSE ) {
    $aids = (isset($aid) ? array($aid) : $array());
    $conditions = (isset($vid) ? array('vid' => $vid) : array());
    $artwork = artwork_load_multiple($aids, $conditions, $reset);
    return $artwork ? reset($artwork) : FALSE;
  }
  
  /**
   * @function method for loading multiple artwork entities
   * @param type $aids  and array of artwork ids
   * @param type $conditions - array of artwork version ids
   * @param type $reset 
   * @return type  an array of artwork  entities
   */
  function artwork_load_multiple($aids = array(), $conditions = array(), $reset = FALSE) {
    return entity_load('artwork', $aids, $conditions, $reset);
  }
}
