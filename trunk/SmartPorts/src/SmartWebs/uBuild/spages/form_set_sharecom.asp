<html>
<!-- Copyright (C) 2011 ezlibs.com, All Rights Reserved. -->
<!-- form_set_sys.asp -->
<head>
<title>SmartPort</title>
<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
</head>
<body>

<h1>͸����������</h1>
<hr>
<h1>todo 2013-07-13 18:40:32</h1>
<!-- ------------------------------------------------------------ -->
<hr>

<table>
<form action=/ezform/formSetShareCom method=POST">
	<tr>
	<td><h2>����1:</h2></td><td> </td>
</tr>

<tr>
	<td>�����豸:</td><td><% aspGetSys("sys_name"); %></td>
</tr>
<tr>
	<td>������</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>����λ:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>У��λ:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>ֹͣλ:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>ģʽ:</td><td>TCPServer</td>
</tr>
<tr>
	<td>�˿�:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<!-- ------------------------------------------------------------ -->
<tr>
	<td><h2>����2:</h2></td><td> </td>
</tr>
<tr>
	<td>�����豸:</td><td><% aspGetSys("sys_name"); %></td>
</tr>
<tr>
	<td>������</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>����λ:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>У��λ:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>ֹͣλ:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>ģʽ:</td><td>TCPServer</td>
</tr>
<tr>
	<td>�˿�:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>

<!-- ------------------------------------------------------------ -->

<tr>
    <td></td><td ALIGN="CENTER"> <input type=submit name=ok value="Ok"> <input type=reset name=reset value="Reset"></td>
</tr>
</form>
</table>

<hr>
<p><% aspGetSys("sys_copyright"); %></p>
</body>
</html>
