<html>
<head>
    <title>��������� Web "A" Tools</title>
</head>
<body>
    <h1>Welcome</h1>
    <div>���� ������ �������� ��� ����� ������� ���������.</div>
    <form action="index.php" method="POST">
        <table border="0">
            <tr>
                <td>�������� ���� </td>
                <td><select name="lang">[{html_options values=$langs output=$lnNames selected=$defLang}]</select></td>
            </tr>
            <tr>
                <td>&nbsp;</td>
                <td><input type="submit" value="����� >"></td>
            </tr>
        </table>
    </form>
</body>
</html>