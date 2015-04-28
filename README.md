# drush-variable-file

Custom drush command to create a new file from a distant source to Drupal, and assign it's file id to a given variable

To install it, copy the folder to your drush folder and don't forget to run a drush cc drush to have your drush detect your new function.

Usage :
drush add-variable-fid <file> <variable_name> [--folder]

Arguments :
<file> : Source file, can be a distant file with full URL.
<variable_name> : Your Drupal destination variable.


Options :
--folder : Specify a folder name in your public:// folder. If it does not exist, it will be created.

