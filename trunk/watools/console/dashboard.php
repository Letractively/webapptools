<?
require_once('./sessions.php');
require_once('./themes.php');
require_once('./usermgmt.php');

// todo check ACL for access
//if (!CheckACL('dashboard')) {
//    PrintNoAccess();
//    exit(0);
//}
$db = GetDbConnection();
$isSystem = false;
if (!is_null($r)) {
    // check for admin group
    if (in_array(0, $gUser['groups']) || $gUser['id'] == 0) {
        $isSystem = true;
        // put system information to dashboard
/*        $rInfo = $r->info();
        $smarty->assign('redisVersion', $rInfo['redis_version']);
        $smarty->assign('redisUptime', $rInfo['uptime_in_days']);
        $smarty->assign('redisRole', $rInfo['role']);
        $smarty->assign('redisMemory', $rInfo['used_memory']);
        $smarty->assign('redisKeys', $rInfo['db' . $gRedisDB]);*/
		$dbdrv = $db->getAttribute(PDO::ATTR_DRIVER_NAME);
		$dbver = $db->getAttribute(PDO::ATTR_CLIENT_VERSION);
		$smarty->assign('sqlDriver', $dbdrv);
        $smarty->assign('sqlVersion', $dbver);
    }
}

$table = GetTableName("users");
$users = GetSingleRow($db, "SELECT count(id) FROM $table");
$smarty->assign('sysUsers', $users[0]);
$table = GetTableName("sessions");
$usersOnline = GetSingleRow($db, "SELECT count(id) FROM $table WHERE client_id>-1");
$smarty->assign('sysUsersOnline', $usersOnline[0]); // . ' ' . print_r($usersOnline, true));
$sess = $users = GetSingleRow($db, "SELECT count(id) FROM $table");
$smarty->assign('sysSessions', $sess[0]);

$smarty->assign('osName', php_uname());
$cpuStat = gettext('Unknown');
$ramStat = gettext('Unknown');
$dskStat = gettext('Unknown');
if (strtoupper(substr(PHP_OS, 0, 3)) == 'WIN') {
    // winduuuuws logooooo, ha-ha-ha...
}
else {
    $sadf = exec('whereis sadf');
    $sadf = substr($sadf, 6);
    if ($sadf != '') {
        $info = exec("sadf -d -t -- -u | tail -n 1");
        $res = explode(';', $info);
        $data = $res[9];
        if (is_numeric($data)) {
            $cpuStat = 100 - $data;
            $cpuStat .= '%';
        }
        $info = exec("sadf -d -t -- -r | tail -n 1");
        $res = explode(';', $info);
        $data = $res[5];
        if (is_numeric($data)) {
            $ramStat = $data;
            $ramStat .= '%';
        }
        // todo: add disk statistic
        $info = exec("df -k -P -T | awk '{if ($7 == \"/\"){print $6}}'");
        if ($info != "") {
            $dskStat = $info;
        }
    }
}

$smarty->assign('cpuStat', $cpuStat);
$smarty->assign('ramStat', $ramStat);
$smarty->assign('dskStat', $dskStat);

$smarty->assign('UserName', $gUser[0]);
$smarty->assign('UserDesc', $gUser[1]);
$smarty->assign('SysUser', $isSystem);
DisplayThemePage('dashboard.html');
?>
