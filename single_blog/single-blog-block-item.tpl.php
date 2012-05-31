<?php

/**
* @file 
* this template file is for theming  a single blog list item 
*
* Variables available
*
* $classes - a string of CSS classes  avaialble. It can be changed in the $classes_array variable in presprocess functions.
* 
* $date - a formatted date string 
* $title - a render array that provides a link to the node 
* $name - a formatted user name 
*
*  Other variables
*  $node - a partial node object  that may contain unsafe data - use check_plain
*  $user -  a user object
*  $classes_array -  an array of CSS classes
*  $created -  a unix timestamp for the node creation date
*  @see template_preprocess_single_blog_block_item()
*/
?>
<div class="<?php print $classes; ?>"> 
  <div class="date"><?php print $date; ?></div>
  <h4 <?php print $title_attributes; ?>> <?php print render($title); ?> </h4>
  <div class="name"><?php print t('by !username', array('!username' => $name)); ?></div>
</div>