<?php
/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function variable_fid_drush_command() {
  $items = array();
  $items['add-variable-fid'] = array(
    'description' => "Add a file to Drupal and assign its file id to a variable ",
    'arguments' => array(
      'url' => dt('File url to fetch.'),
      'variable_name' => dt('Variable name to assign the file id to.'),
    ),
    'options' => array(
      'folder' => 'Public subfolder name.',
    ),
    'aliases' => array('avfid'),
  );
  return $items;
}

/**
 * Add a language
 */
function drush_variable_fid_add_variable_fid() {
  $args = func_get_args();
  if (count($args) != 2) {
    drush_set_error(dt('Usage: drush add-variable-fid [--folder] <url> <variable-name> '));
    return;
  }

  $file_source = $args[0];
  $variable_destination = $args[1];

  $folder = drush_get_option('folder', '');
  $file_name = basename($file_source);
  $temporary = file_directory_temp() . "/$file_name";

  // Download and copy the file in the system
  $file_fetched = copy($file_source, $temporary); // Copy the file in the temporary

  if (!$file_fetched) {
    drush_die('Could not fetch the file from ' . $file_source);
    return "";
  }
  // Make sure the destination folder is writable
  $directory = 'public://' . $folder;
  $writable_directory = file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  if (!$writable_directory) {
    drush_die('Destination file folder is not writable - Could not make it writable : ' . $directory);
  }

  // Put the file in public folder
  $uri = file_unmanaged_move(
    $temporary,
    "public://{$folder}/$file_name",
    FILE_EXISTS_RENAME
  );

  // Create a file object.
  $file = new stdClass();
  $file->fid = NULL;
  $file->uri = $uri;
  $file->filename = $file_name;
  $file->filemime = file_get_mimetype($file->uri);
  $file->status = FILE_STATUS_PERMANENT;
  $file = file_save($file);

  if (isset($file->fid) && !empty($file->fid)) {
    // Assign the file id to the variable
    variable_set($variable_destination, $file->fid);
    drush_print('File was correctly saved and assigned to variable ' . $variable_destination . ' // fid : ' . $file->fid);
  }
  else {
    drush_die('Could not save the file in Drupal');
  }

  return;
}