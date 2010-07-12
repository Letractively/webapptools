<?
// put full path to Smarty.class.php
require_once('./globals.php');
require_once('./sessions.php');
require_once('./smarty/Smarty.class.php');
$smarty = new Smarty();

$r = GetDbConnection();

if (is_null($r) || is_null($gSession)) {
    header("HTTP/1.0 302 Found");
    header("Location: /maintance.php");
    exit(0);
}


$themeName = $gSession['theme'];
$themeLang = $gSession['lang'];
$themeDir = $gBaseDir . '/' . $themeName;
// set language translation
setlocale( LC_MESSAGES, $themeLang);
putenv("LANG=".$themeLang);
bindtextdomain('watconsole', $themeDir);
textdomain('watconsole');
bind_textdomain_codeset('watconsole', 'UTF-8');
$themeLangExt = $themeLang;
if ($themeLangExt == 'en') {
    $themeLangExt = '';
}

$smarty->left_delimiter = '[{';
$smarty->right_delimiter = '}]';
$smarty->template_dir = $themeDir;
$smarty->compile_dir = $themeDir . '/templates_c';
$smarty->cache_dir = $themeDir . '/cache';
$smarty->config_dir = './smarty/configs';

function PrintNoAccess()
{
    global $smarty, $gUser, $themeName, $themeLangExt;
    
    $smarty->assign('UserName', $gUser['login']);
    $smarty->assign('messageTitle', gettext('Access Denied'));
    $smarty->assign('messageText', gettext('Access denied for user ') . $gUser['login'] . gettext('!<br>Contact your system administrator!'));
    $smarty->assign('messageIcon', 'warning.png');
    $smarty->assign('messageButton', 'OK');
    DisplayThemePage('messageBox.html');
}

function DisplayThemePage($page, $lang)
{
    global $smarty, $themeName, $themeLangExt, $themeDir, $gBaseDir;
    
    header("Content-Type: text/html; charset=utf-8");
    $smarty->assign('theme', $themeName);
    $ext = $themeLangExt;
    if ($ext != "") {
        if (file_exists($themeDir . '/' . $page . '.' . $themeLangExt)) {
            $smarty->display($page . '.' . $themeLangExt);
            return;
        }
        else {
            $ext = "";
        }
    }
    if ($ext == "") {
        if (file_exists($themeDir . '/' . $page )) {
            $smarty->display($page);
            return;
        }
    }
    // try to search in the default theme
    $smarty->template_dir = $gBaseDir . '/theme';
    $smarty->compile_dir = $gBaseDir . '/theme/templates_c';
    $smarty->cache_dir = $gBaseDir . '/theme/cache';
    $ext = $themeLangExt;
    if ($ext != "") {
        if (file_exists($gBaseDir . '/theme/' . $page . '.' . $themeLangExt)) {
            $smarty->display($page . '.' . $themeLangExt);
            return;
        }
        else {
            $ext = "";
        }
    }
    if ($ext == "") {
        if (file_exists($gBaseDir . '/theme/' . $page )) {
            $smarty->display($page);
            return;
        }
    }
    // somthing wrong...
    $smarty->assign('messageTitle', gettext('Visualisation broken'));
    $smarty->assign('messageText', gettext('Can\'t display page ') . $page . gettext(' in theme ') . $themeName .
                    gettext('!<br>Contact your system administrator!'));
    $smarty->assign('messageIcon', 'warning.png');
    $smarty->assign('messageButton', 'OK');
    $smarty->display('messageBox.html');
}
?>
