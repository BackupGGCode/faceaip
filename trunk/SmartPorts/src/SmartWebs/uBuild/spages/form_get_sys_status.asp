<html>
<head>
<title>SmartPorts</title>
<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
</head>

<body>
<H1>����״̬</H1>
<p>SmartPorts is a fully-featured embedded device.</p>
<hr>
<h2>������Ϣ:</h2>
<UL>
<LI>��������<% aspGetSys("sys_name"); %></li>
<LI>��ţ�<% aspGetSys("sys_product_id"); %></li>
<LI>���кţ�<% aspGetSys("sys_serial_no"); %></li>
<LI>�汾�ţ�<% aspGetSys("sys_version"); %></li>
</UL>
<p></p>

<hr>
<h2>������Ϣ:</h2>
<UL>
<LI>����ʱ�䣺<% aspGetSys("sys_start_date_time"); %></li>
<LI>����ʱ����<% aspGetSys("sys_run_period"); %> (��)</li>
</UL>
<p></p>
<hr>

<h2>������Ϣ:</h2>
<UL>
<LI>�������ӣ�<% aspGetSys("sys_internet"); %></li>
<LI>����IP��<% aspGetSys("sys_nat_ip"); %></li>
</UL>
<p></p>

<hr>
<p><% aspGetSys("sys_copyright"); %></p>
</body>
</html>
