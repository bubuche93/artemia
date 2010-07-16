// $Id: README.txt,v 1.13 2010/05/30 23:39:47 quiptime Exp $

The Gallery Assist Comments (GAC) module allow to add comments to
Gallery Assist (GA) items.


Requirements
--------------------------------------------------------------------------------
- This module is written for Drupal 6.0+.

- The version 6.x-1.12 or higher of the GA module.

  The requirement check is date based.
  Alternatively to version gallery_assist-6.x-1.12 can also use the development 
  version with an adequate release date.

Installation
--------------------------------------------------------------------------------
Copy the GAC module folder to your module directory and then enable on the
admin modules page.

Uninstallation
--------------------------------------------------------------------------------
An uninstall of the GAC module delete any comments.

Administration
--------------------------------------------------------------------------------
1. Administer input formats.

   This is an important point to configure GAC.

   Create an extra Input format! E. g. "Simple comment" as name.

   As filters use only:
   - HTML filter
   - Line break converter

     Configure the HTML filter settings and reduce the Allowed HTML tags to a
     absolutely needed minimum!

     Remember:
     The comment function of the modules should follow the simple principle.

   For different content types can be used different Input formats.

   Input formats can not be changed by gallery owners.

   Note:
   Have anonymous users the permission "post gallery assist comments" and the
   GAC settings allow "Anonymous commenting", then choose the role
   "anonymous user" to use the extra filter formats.

2. Administer the GAC permissions.

3. Administer the GAC settings.

   - Enable the comment function for each GA content type and configure the
     comment options which should be to use.
   - Use the extra Input formats.

   - "Allow user settings"?
     With this option it is possible to configure the comment function for
     every gallery.

Theming
--------------------------------------------------------------------------------
GAC comes with 10 templates to modify the display of the comments.
Copy the needed template files from the module theme folder to the folder of the
current theme.

Internationalization
--------------------------------------------------------------------------------
The module is fully translatable.

 - Help

   We need your help to have module translations into other languages.

   http://localize.drupal.org/translate/projects/gallery_assist_comments

   Thanks

Limitations
--------------------------------------------------------------------------------
- GAC does not provide a pager for the comments.
- GAC has no administration for the management of the comments.
- IE 6: Confused layout for display.

Troubleshooting
--------------------------------------------------------------------------------
- Please check the permissions of the GAC module.

- Please check the settings of the GAC configuration option
  "Allow user settings".

- The comment form display has no input format information.

  This is not a bug but a functionality.

Documentation
--------------------------------------------------------------------------------
http://simple.puntolatinoclub.de/docu/gallery-assist/optional-modules

Maintainers
--------------------------------------------------------------------------------
The GAC module was originally developped by:
Juan Carlos Morejon Caraballo

- Maintainer
  Quiptime Group
  Juan Carlos Morejon Caraballo
  jcmc [ dot ] devel [ at ] gmail [ dot ] com

- Comaintainer
  Quiptime Group
  Siegfried Neumann
  www.quiptime.com
  quiptime [ at ] gmail [ dot ] com
