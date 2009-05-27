function showlogin(rs){
	document.getElementById("loginDiv").style.display = "none";
	login_div.innerHTML=rs
}
function searchpost(){
	if(document.formsearch.keyword.value==''){
		alert('关键字不能为空');
		return false;
	}
	if(document.formsearch.keyword.value=='请输入关键字'){
		alert('请输入关键字');
		return false;
	}
	document.formsearch.submit();
}
function copyTXT(obj) 
{
	var rng = document.body.createTextRange();
	rng.moveToElementText(obj);
	rng.select();
	rng.execCommand('Copy');
	alert('网址复制成功');
}
//document.onclick=handle_btn_click;

function handle_btn_click(evt){
    if(evt==null)evt=window.event;//IE
    //处理事件.
}
/*
document.onclick = function() {
	if (window.event.srcElement.getAttribute("Author") == "Goodu") {
		document.getElementById("loginDiv").style.display = "block";
	}else if(window.event.srcElement.getAttribute("Author") == "Goodu_showLogin"){
		document.getElementById("loginDiv").style.display = document.getElementById("loginDiv").style.display == "none" ? "block":"none";
	}else{
		document.getElementById("loginDiv").style.display = "none";
	}
}
*/

document.write('<div id="Toolbar"><div style="FLOAT: left; padding-top:4px;">　<a href="'+blog_url+'/blog.php"  title="网站首页"><img src="images/default/menulogo.gif" alt="网站首页" height="18" border="0"></a></div><div style="FLOAT: left; padding-top:7px;"><span id="login_div">');

if(lfjid!=''){
	document.write('　欢迎你回来:&nbsp;<span style="color:red;">'+lfjid+'</span>&nbsp;&nbsp;<a href="'+blog_url+'/?uid='+lfjuid+'">我的博客</a>&nbsp;&nbsp; | <a href="'+www_url+'/login.php?action=quit"  >安全退出</a>');
} else {
	//document.write('  　<a href="javascript:void(0);" Author="Goodu_showLogin">登陆</a>　|　<a href="'+www_url+'/reg.php" target=_blank>注册</a>');
	document.write('  　<a href="'+www_url+'/login.php">登陆</a>　|　<a href="'+www_url+'/reg.php" target=_blank>注册</a>');
}
document.write('&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:" onclick="copyTXT(this)" title="点击复制此网址">'+ThisBlogUrl+'</a></span></div><form name="formsearch" method="post" action="'+www_url+'/search.php?searchTable=article&type=title" target="_blank"><div style="FLOAT: right;padding-top:10px;">　<a href="#" onclick=searchpost()>搜索</a>&nbsp;&nbsp;</div> <div style="FLOAT: right;padding-top:5px;">　    <input name="keyword" type="text" class="notice" value="请输入关键字" onFocus="if(this.value==\'请输入关键字\')this.value=\'\';" onMouseOut="if(this.value==\'\')this.value=\'请输入关键字\'" /></div></form></div>');

//document.write('<div id="loginDiv" style="display: none;position:absolute;top:24px;left:53px;z-index:9999; width:280px; height:110px;background-color:#FFFFFF; border:1px solid #CCCCCC; padding:3px;" Author="Goodu">  <table width="100%" border="0" cellspacing="2" cellpadding="0" Author="Goodu" id="ToolbarForm"> <form name="form1" method="post" action="'+www_url+'/login.php"  >    <tr>      <td nowrap Author="Goodu" style="COLOR: #f03309;"><label for="username"  Author="Goodu">用户名</label></td>      <td Author="Goodu"><input style="BORDER-RIGHT: #cecece 1px solid; BORDER-TOP: #cecece 1px solid; BORDER-LEFT: #cecece 1px solid; BORDER-BOTTOM: #cecece 1px solid; width:176px; height:18px;" name="username" type="text" id="loginname" value="" Author="Goodu"/>         </td>    </tr>    <tr>     <td nowrap  style="COLOR: #f03309;"  Author="Goodu"><label for="passwd" Author="Goodu">密　码</label></td>      <td Author="Goodu"><input style="BORDER-RIGHT: #cecece 1px solid; BORDER-TOP: #cecece 1px solid; BORDER-LEFT: #cecece 1px solid; BORDER-BOTTOM: #cecece 1px solid; width:176px; height:18px;" name="password" type="password" id="passwd" value="" Author="Goodu"/></td>    </tr>    <tr>      <td Author="Goodu"><input type="hidden" value="2" name="step"><input type="hidden" value="0" name="ajax"></td>      <td Author="Goodu"><input name="Submit" id="Submit" type="submit" style="BORDER-RIGHT: #cecece 1px solid; PADDING-RIGHT: 5px; BORDER-TOP: #cecece 1px solid; PADDING-LEFT: 5px; PADDING-BOTTOM: 0px; BORDER-LEFT: #cecece 1px solid; CURSOR: pointer; COLOR: white; LINE-HEIGHT: 18px; PADDING-TOP: 0px; BORDER-BOTTOM: #cecece 1px solid; HEIGHT: 20px; BACKGROUND-COLOR: #f03309;"  value=" 登  录 " Author="Goodu"/>        你是新人吗？<a href="'+www_url+'/register.php" Author="Goodu">立刻申请</a><br />        </td>    </tr>   <tr>     <td colspan="2" Author="Goodu" align="left"><label for="reme" Author="Goodu">      <input type="checkbox" id="reme" value="315360000"  Author="Goodu" name="cktime"/>        选择下次自动登录</label></td>    </tr></form>  </table></div>');