<html>
<!-- Copyright (C) 2011 ezlibs.com, All Rights Reserved. -->
<!-- form_set_sys.asp -->
<head>
<title>SmartPort</title>
<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
</head>
<body>

<h1>��������</h1>
<table>
<form action=/ezform/formSetSys method=POST">

<tr>
	<td>������:</td><td><input type=text name=sys_name size=50 value="<% aspGetSys("sys_name"); %>"/></td>
</tr>
<tr>
	<td>���:</td><td><input type=text name=sys_product_id size=50 value="<% aspGetSys("sys_product_id"); %>"/></td>
</tr>
<tr>
	<td>���к�:</td><td><% aspGetSys("sys_serial_no"); %></td>
</tr>
<tr>
	<td>�汾��:</td><td><% aspGetSys("sys_version"); %></td>
</tr>

<tr>
    <td></td>
      <td ALIGN="CENTER"> <input type=submit name=ok value="Ok"> <input type=reset name=reset value="Reset"></td>
</tr>
</form>
</table>

<hr>	
<!-- ------------------------------------------------------------ -->
<h1>���ں�ʱ��</h1>
<table>
<form action=/ezform/formSetDateTime method=POST">
<tr>
	<td>�Զ�ȷ�����ں�ʱ��:</td><td><input type="checkbox" id=ch name=ntp_enable <% aspGetDateTime("ntp_enable"); %>"/>����ѡ��ʹ����������ں�ʱ��</td>
</tr>
<tr>
	<td><h3>�߼�:</h3></td><td></td>
</tr>
<tr>
	<td>__ʱ�������:</td><td><input type=text name=ntp_server size=40 value="<% aspGetDateTime("ntp_server"); %>"/></td>
</tr>
<tr>
	<td>__�������:</td><td><input type=text name=ntp_checkperiod size=40 value="<% aspGetDateTime("ntp_checkperiod"); %>"/>�루>=5��</td>
</tr>

<!--
<tr>
	<td>�Զ�ȷ��ʱ��:</td><td><input type="checkbox" id=ch name=autotimezone <% aspGetDateTime("autotimezone"); %>"/></td>
</tr>
-->

<tr>
	<td>��������ʱ��:</td><td><input type=text name=sys_date_time size=40 value="<% aspGetDateTime("sys_date_time"); %>"/></td>
</tr>
<tr>
	<td>ѡ��ʱ��:</td><td><input type=text name=sys_timezone size=40 value="<% aspGetDateTime("sys_timezone"); %>"/></td>
</tr>
<!--
<tr>
	<td>ѡ�����ڸ�ʽ:</td><td><input type=text name=sys_date_format size=50 value="<% aspGetDateTime("sys_date_format"); %>"/></td>
</tr>
-->
<tr>
    <td></td>
      <td ALIGN="CENTER"> <input type=submit name=ok size=20 value="Ok"> <input type=reset name=reset size=20 value="Reset"></td>
</tr>
</form>
</table>

<hr>
<h1>ϵͳά��</h1>

<table>
<form action=/ezform/formSetSys method=POST>
<tr>
    <td><input type="hidden" name="sys_reboot" value="now" />����ϵͳ��</td>
    <td ALIGN="LEFT"> <input type=submit name=ok value="����"></td>
</tr>
</form>

<form action=/ezform/formSetSys method=POST>
<tr>
    <td><input type="hidden" name="sys_restore" value="factory" />�ָ�����״̬��</td>
    <td ALIGN="LEFT"> <input type=submit name=ok value="�ָ�"></td>
</tr>
</form>

<form action=/ezform/formSetSys method=POST>
<tr>
    <td><input type="hidden" name="sys_net_console" value="factory" />Զ��ά�����ܣ�</td>
    <td ALIGN="LEFT"> <input type=submit name=ok value="�����У�����ر�"></td>
</tr>
</form>

<form action=/ezform/formSetSys method=POST>
<tr>
    <td><input type="hidden" name="sys_watch_dog" value="factory" />���Ź���</td>
    <td ALIGN="LEFT"> <input type=submit name=ok value="�����У�����ر�"></td>
</tr>
</form>

</table>

<hr>
<p><% aspGetSys("sys_copyright"); %></p>
</body>
</html>
