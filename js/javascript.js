(function() {
    var pageTitle = $('div.pageTitle').attr('id');
    var pageAuthorTwitter = $('div.pageAuthorTwitter').attr('id');
    var siteUrl = $('div.siteUrl').attr('id');
    var pageUrl = $('div.pageUrl').attr('id');

if(/iPhone|iPad|iPod/i.test(navigator.userAgent)) {
    $('a#twttr').attr('href', 'twitter://post?message=' + pageTitle + '%20via%20' + '@' + pageAuthorTwitter + '%20' + siteUrl + pageUrl);
    $('a#fb').attr('href', 'fb://publish/profile/me?text=' + pageTitle + '%20' + siteUrl + pageUrl);
    $('a#gplus').attr('href', 'gplus://share?url=' + siteUrl + pageUrl);
}})();
