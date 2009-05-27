function killErrors() {return true;}
window.onerror = killErrors;
var agt=navigator.userAgent.toLowerCase();
var ie = ((agt.indexOf("msie") != -1) && (agt.indexOf("opera") == -1) && (agt.indexOf("omniweb") == -1));
var kuantongtitle = "SmartCreative";
var winpopflag = 1;
var showdoshowflag = 1;
var flvadd = "";
function getCookiesc(name)
{
	var search, offset;
	search = name + "=";
	offset = document.cookie.indexOf(search);
	if (offset != -1)
	{
		offset += search.length;
		end = document.cookie.indexOf(";", offset);
		if (end == -1)
			end = document.cookie.length;
		return unescape(document.cookie.substring(offset, end));
	}else
		return false;
}
function setCookiesc(name, value, expires)
{
	var expdate = new Date();
	var argv = setCookiesc.arguments;
	var argc = setCookiesc.arguments.length;
	var expires = (argc > 2) ? argv[2] : null;
	var path = (argc > 3) ? argv[3] : null;
	var numdian = document.domain.split(".");
	var numdianleng = numdian[0].length;
	var domain = document.domain.substring(numdianleng+1);
	var secure = (argc > 5) ? argv[5] : false;
	document.cookie = name + "=" + escape (value) +((expires == null) ? "" : ("; expires="+ expires.toGMTString()))
	+((path == null) ? "" : ("; path=" + path)) +((domain == null) ? "" : ("; domain=" + domain))
	+((secure == true) ? "; secure" : "");
	var numfen = document.cookie.split(";");
	if(document.cookie == "" || numfen.length==1)
	{
		document.cookie = name + "=" + escape(value) + "; expires=" + expires.toGMTString() +  "; path=/";
	}
}
function popmsg(){
var winstr="<div ID='divnameimgdis' name='divnameimgdis' style='position:absolute; z-index:1;left:"+(popwidth-tclosegifx)+";top:"+tclosegify+" ;width:10; height:10'><img src='"+clogifadd+"' style='cursor:hand' title='close' width=10 height=10 onclick='parent.disdiv();' /></div><object id='hotson' name='hotson' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0' width="+popwidth+" height="+popheight+">";
winstr+="<param name='allowScriptAccess' value='always' />";
winstr+="<param name='movie' value='"+bannerswfadd+"' />";
winstr+="<param name='quality' value='high' />";
winstr+="<param name='allowFullScreen' value='true' />";
winstr+="<param name='tagCode' value='69' />";
winstr+="<param name='tagLength' value='4' />";
winstr+="<param name='FlashVars' value='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&loadfinish=1&targetflag=_self&stopflag=play&adftrack_ref="+adftrack_ref+flash_var_list+"&flvPath="+escape(flvadd)+"' />";
winstr+="<param name='tagContent' value='0x00000001' />";
winstr+="<param name='wmode' value='Opaque' />";
winstr+="</object>";
oPopup.document.body.innerHTML = winstr;
videoWin = oPopup;
	 load100();

}
function divpopmsg()
{
	creativeCookie();
	if(ie)
	{
		var winstr="<iframe name='swfifram' id='swfifram' src='about:blank' scrolling='no' frameborder='0' style='position:absolute;width:" + popwidth + "px;height:"+ popheight +"px;top:"+flash_top2006+"px;left:"+flash_left2006+"px;border:none;display:block;z-index:0'></iframe><div ID='divnameimgdis' name='divnameimgdis' style='position:absolute; z-index:10002;left:"+(flash_left2006+popwidth-tclosegifx)+"px;top:"+(flash_top2006+tclosegify)+"px ;width:10px; height:10px'><img id='imgcoltem' src='"+clogifadd+"' style='cursor:pointer' width=10px height=10px onclick='disdiv()' /></div>";
	}else
	{
		var winstr="<div ID='divnameimgdis' name='divnameimgdis' style='position:absolute; z-index:10002;left:"+(flash_left2006+popwidth-tclosegifx)+"px;top:"+(flash_top2006+tclosegify)+"px ;width:10px; height:10px'><img id='imgcoltem' src='"+clogifadd+"' style='cursor:pointer' width=10px height=10px onclick='disdiv()' /></div>";
	}
	winstr+="<div id='divname1' name='divname1' style='position:absolute; z-index:10001;left:"+flash_left2006+"px;top:"+flash_top2006+"px; width:" + popwidth + "px; height: "+ popheight +"px'><object id='hotson' name='hotson' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0' width='"+popwidth+"px' height='"+popheight+"px'>";
	winstr+="<param name='allowScriptAccess' value='always' />";
	winstr+="<param name='movie' value='"+bannerswfadd+"' />";
	winstr+="<param name='quality' value='high' />";
	winstr+="<param name='allowFullScreen' value='true' />";
	winstr+="<param name='FlashVars' value='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&loadfinish=1&targetflag=_blank&stopflag=play&adftrack_ref="+adftrack_ref+flash_var_list+"&flvPath="+escape(flvadd)+"' />";
	winstr+="<param name='wmode' value='Opaque' />";
	winstr+="<embed type='application/x-shockwave-flash' PLUGINSPAGE='http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash' src='"+bannerswfadd+"' id='divnameimgdis2' name='divnameimgdis2' allowFullScreen='true' allowScriptAccess='always' quality='high' width='"+popwidth+"px' height='"+popheight+"px' swLiveConnect='true' wmode='Opaque' FlashVars='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&loadfinish=1&targetflag=_blank&stopflag=play&adftrack_ref="+adftrack_ref+flash_var_list+"&flvPath="+escape(flvadd)+"'></embed></object></div>";
	document.getElementById("spankt").innerHTML=winstr;
	tttt=setInterval("movelook()",50);
}