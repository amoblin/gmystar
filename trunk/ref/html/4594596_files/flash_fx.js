function killErrorsss() {return true;}
window.onerror = killErrorsss;
var allyesagt=navigator.userAgent.toLowerCase();
var allyesie = ((allyesagt.indexOf("msie") != -1) && (allyesagt.indexOf("opera") == -1) && (allyesagt.indexOf("omniweb") == -1));
function alysxc(src,height,width,flashId,mode){
	if(typeof(flashVarsdb)=="undefined")flashVarsdb="errorDB";
	if(typeof(floatFlashVars)=="undefined") floatFlashVars="";
	var pAry=mode.split("=");
	if(allyesie){
		var winstr="<object id='"+flashId+"' name='hsn' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0'width="+width+" height="+height+">";
		winstr+="<param name='allowScriptAccess' value='always' />";
		winstr+="<param name='movie' value='"+src+"' />";
		winstr+="<param name='quality' value='high' />";
		if(typeof(pAry[1])!="undefined")
			winstr+="<param name='scale' value='noscale' />";
		if(typeof(floatFlashVars)!="undefined")
			winstr+="<param name='FlashVars' value='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+floatFlashVars+"' />";
		else
			winstr+="<param name='FlashVars' value='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' />";
		winstr+="<param name='wmode' value='"+pAry[0]+"' />";

		winstr+="</object>";
}else
{
	var winstr="<embed id='"+flashId+"' src='"+src+"' allowScriptAccess='always' quality='high' ";
	winstr+="pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'";
	winstr+=" width='"+width+"' height='"+height+"' swliveconnect='true' ";
	if(typeof(pAry[1])!="undefined")
		winstr+=" scale='noscale' ";
	if(typeof(floatFlashVars)!="undefined")
		winstr+=" FlashVars='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+floatFlashVars+"' ";
	else
		winstr+="FlashVars='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' ";
	winstr+=" wmode='"+pAry[0]+"' ";
	winstr+=" name='"+flashId+"' >";
	winstr+="</embed>";
}
document.write(winstr);
}
function writeDoc(temp,str){temp.open();temp.write(str);temp.close();}
function popStatus(flashsrc1,flashsrc2){
var winstr="<div id='flashid1' style='cursor:hand;display:none'>";
winstr+="<object id='hsn1' name='hsn' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0' width='100%' height='100%'>";
winstr+="<param name='allowScriptAccess' value='always' />";
winstr+="<param name='movie' value='"+flashsrc1+"' />";
winstr+="<param name='quality' value='high' />";
winstr+="<param name='salign' value='LB' />";
winstr+="<param name='FlashVars' value='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' />";
winstr+="<param name='wmode' value='Opaque' />";
winstr+="<embed id='emhsn' src='"+flashsrc1+"' allowScriptAccess='always' quality='high' ";
winstr+="pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'";
winstr+=" width='100%' height='100%' swliveconnect='true' ";
winstr+=" scale='LB' ";
winstr+=" FlashVars='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' ";
winstr+=" wmode='Opaque' ";
winstr+=" name='emhsn' >";
winstr+="</embed></object></div>";
winstr+="<div id='flashid2' onmouseover='parent.show2(1);' style='cursor:hand;display:none'><object id='hsn2' name='hsn2' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0' width='100%' height='100%'>";
winstr+="<param name='allowScriptAccess' value='always' />";
winstr+="<param name='movie' value='"+flashsrc2+"' />";
winstr+="<param name='quality' value='high' />";
winstr+="<param name='salign' value='LB' />";
winstr+="<param name='FlashVars' value='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' />";
winstr+="<param name='wmode' value='Opaque' />";
winstr+="<embed  id='emhsn2' src='"+flashsrc2+"' allowScriptAccess='always' quality='high' ";
winstr+="pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'";
winstr+=" width='100%' height='100%' swliveconnect='true' ";
winstr+=" scale='LB' ";
winstr+=" FlashVars='postallyesid="+escape(temclickadd)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' ";
winstr+=" wmode='Opaque' ";
winstr+=" name='emhsn2' >";
winstr+="</embed></object></div>";

winstr+="<div ID='divnameimgdis' name='divnameimgdis' style='position:absolute;z-index:100000;left:"+parent.leftPos+";top:"+parent.TopPos+";width:10; height:10'><table border=0><tr><td><img border=0 src='"+parent.clogifadd+"' style='cursor:hand' title='close' width=10 height=10 onclick='parent.disdiv()'></td></tr></div>";
op.document.body.innerHTML = winstr;
}
function alysxc_parent(divId,src,height,width,flashId,mode,mouseMethod,click){
	if(typeof(flashVarsdb)=="undefined")
		flashVarsdb="errorDB";
	if(typeof(frameIndex)=="undefined")
		frameIndex=0;
	var pAry=mode.split("=");
	if(allyesie){
		var winstr="<object id='"+flashId+"' name='hsn' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0'width="+width+" height="+height+">";
		winstr+="<param name='allowScriptAccess' value='always' />";
		winstr+="<param name='movie' value='"+src+"' />";
		winstr+="<param name='quality' value='high' />";
		if(typeof(pAry[1])!="undefined")
			winstr+="<param name='scale' value='noscale' />";

		if(typeof(floatFlashVars)!="undefined")
			winstr+="<param name='FlashVars' value='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&frameIndex="+escape(frameIndex)+"&db="+escape(flashVarsdb)+floatFlashVars+"' />";
		else
			winstr+="<param name='FlashVars' value='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&frameIndex="+escape(frameIndex)+"&db="+escape(flashVarsdb)+"' />";
		winstr+="<param name='wmode' value='"+pAry[0]+"' />";

		winstr+="</object>";
}else
{
	var winstr="<embed id='"+flashId+"' src='"+src+"' allowScriptAccess='always' quality='high' ";
	winstr+="pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'";
	winstr+=" width='"+width+"' height='"+height+"' swliveconnect='true' ";
	if(typeof(pAry[1])!="undefined")
		winstr+=" scale='noscale' ";
	
	if(typeof(floatFlashVars)!="undefined")
		winstr+=" FlashVars='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&frameIndex="+escape(frameIndex)+"&db="+escape(flashVarsdb)+floatFlashVars+"' ";
	else
		winstr+="FlashVars='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&frameIndex="+escape(frameIndex)+"&db="+escape(flashVarsdb)+"' ";
	
	winstr+=" wmode='"+pAry[0]+"' ";
	winstr+=" name='"+flashId+"' >";
	winstr+="</embed>";
}
if(allyesie){
	b = "<div id="+divId+ " "+mouseMethod+" style=\'display:none;z-index:99999;position:absolute;width:"+width+"px;height:"+height+"px\'>";
	b += winstr;
	b += "</div>";
	parent.document.body.insertAdjacentHTML("afterBegin",b);
} else {
	b = "<div id="+divId+ " "+mouseMethod+" style=\'z-index:9999999;display:;position:absolute;width:0px;height:0px\'>";
	b += winstr;
	b += "</div>";
	var oDiv=document.createElement("DIV");
	
	parent.document.body.appendChild(oDiv);
	oDiv.innerHTML = b;
}
}
function wf(bgfile,divId,pointList,w,h){
var _html = "<div id='"+divId+"' style='z-index:99999;display:none;position:absolute;left:0px;top:0px;width:"+w+"px;height:"+h+"px'><object id='smflashID' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,0,0' width='100%' height='100%' align='middle'><param name='allowScriptAccess' value='always' /><param name='movie' value='"+bgfile+"' /><param name='quality' value='high' /><param name='wmode' value='transparent' /><param name='salign' value='T' /><param name='menu' value='false' /><param name='scale' value='noscale' /><param name='FlashVars' value='postallyesid="+escape(temclickadd)+pointList+"&postpath="+escape(temadfhost)+"&dbName="+escape(flashVarsdb)+"' /></object></div>";
document.write(_html);
}
function dospan_sohu0413() { 
	for(var i=0;i<window.document.getElementsByTagName("div").length;i++) 
	{
		if(window.document.getElementsByTagName("div")[i].className=="banner")
		{
			document.getElementsByTagName("div")[i].innerHTML = document.getElementById("53cd434212add4603e92487faf64fb47").innerHTML;
		} 
	} 
}
function wf_163(bgfile,pointList,w,h,click){
var _html = "<object id='smflashID' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,0,0' width='100%' height='100%' align='middle'><param name='allowScriptAccess' value='always' /><param name='movie' value='"+bgfile+"' /><param name='quality' value='high' /><param name='wmode' value='transparent' /><param name='salign' value='T' /><param name='menu' value='false' /><param name='scale' value='noscale' /><param name='FlashVars' value='postallyesid="+escape(click)+pointList+"&postpath="+escape(temadfhost)+"&dbName="+escape(flashVarsdb)+"' /></object>";
return _html;
}
function addEelment(bgfile,pointList,w,h,div_id,click)
{
if(typeof div_id =="undefined")
	var div_id ="bg_68";
var _item = document.createElement("DIV");
document.body.children(0).insertAdjacentElement("BeforeBegin",_item);
_item.id = div_id;
_item.style.width = w+"px";
_item.style.height = h+"px";
_item.innerHTML = wf_163(bgfile,pointList,w,h,click);
}
function writeFlashDiv(str) {
document.write(str);
}
function alysxc_2(src,height,width,flashId,mode,click){
	if(typeof(flashVarsdb)=="undefined")flashVarsdb="errorDB";
	if(typeof(floatFlashVars)=="undefined") floatFlashVars="";
	var pAry=mode.split("=");
	if(allyesie){
		var winstr="<object id='"+flashId+"' name='hsn' classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000' codebase='http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0'width="+width+" height="+height+">";
		winstr+="<param name='allowScriptAccess' value='always' />";
		winstr+="<param name='movie' value='"+src+"' />";
		winstr+="<param name='quality' value='high' />";
		if(typeof(pAry[1])!="undefined")
			winstr+="<param name='scale' value='noscale' />";
		if(typeof(floatFlashVars)!="undefined")
			winstr+="<param name='FlashVars' value='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+floatFlashVars+"' />";
		else
			winstr+="<param name='FlashVars' value='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' />";
		winstr+="<param name='wmode' value='"+pAry[0]+"' />";

		winstr+="</object>";
}else
{
	var winstr="<embed id='"+flashId+"' src='"+src+"' allowScriptAccess='always' quality='high' ";
	winstr+="pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'";
	winstr+=" width='"+width+"' height='"+height+"' swliveconnect='true' ";
	if(typeof(pAry[1])!="undefined")
		winstr+=" scale='noscale' ";
	if(typeof(floatFlashVars)!="undefined")
		winstr+=" FlashVars='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+floatFlashVars+"' ";
	else
		winstr+="FlashVars='postallyesid="+escape(click)+"&postpath="+escape(temadfhost)+"&db="+escape(flashVarsdb)+"' ";
	winstr+=" wmode='"+pAry[0]+"'  allowScriptAccess='always' ";
	winstr+=" name='"+flashId+"' >";
	winstr+="</embed>";
}
document.write(winstr);
}
function writeInnerHtml(div, html)
{
div.innerHTML = html;
}
function writeInnerHtml_child(div, html,ids)
{
document.write(html);
for (var i = 0; i < ids.length; i++)
	div.appendChild(document.getElementById(ids[i]));
}
function free_sh()
{
	if(typeof(_smid)!="undefined")
	{
		if(eval("_sh_"+_smid)==1)
		{
			if(eval("_f_"+_smid)==1)
			{
				window.setTimeout("free_sh()",1000);
			}else
			{
			eval("sc_"+_smid).s = 2;
			}
			
		}
	}
}
if(typeof(FQT_main)=="function")window.setTimeout("free_sh()",5000);
