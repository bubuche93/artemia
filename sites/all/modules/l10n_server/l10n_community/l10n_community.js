// $Id: l10n_community.js,v 1.1.2.8.2.6.2.3 2010/03/10 07:45:03 goba Exp $

Drupal.behaviors.replaceL10nServerURLs = function(context) {
  // Act on links if we have URL replacements to do.
  if (Drupal.settings.l10nServerURLs) {
    for (var url in Drupal.settings.l10nServerURLs) {
      // Look for links with this exact URL and replace with extended version.
      // This ensures we keep filter values while switching tabs.
      $('a[href=' + url +']', context).not('.filter-exclude').attr('href', Drupal.settings.l10nServerURLs[url]);
    }
  }
}
