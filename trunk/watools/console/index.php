<? 
// put full path to Smarty.class.php
require('./smarty/Smarty.class.php');
$smarty = new Smarty();

$baseDir = "/var/www/";
$smarty->template_dir = '.'; //'./smarty/templates';
$smarty->compile_dir = './smarty/templates_c';
$smarty->cache_dir = './smarty/cache';
$smarty->config_dir = './smarty/configs';

$smarty->assign('name', 'Ned');
$smarty->display('index.tpl');

?>
