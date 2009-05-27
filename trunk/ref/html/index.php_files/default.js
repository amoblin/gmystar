function resizeimageintro(obj) {
	var imageObject;
	var MaxW = 200;
	var MaxH = 10000;
	imageObject = obj;
	var oldImage = new Image();
	oldImage.src = imageObject.src;
	var dW = oldImage.width;
	var dH = oldImage.height;
	

	if (dW==0||dH==0)
	{
		obj.src=oldImage.src;
		oldImage.src=obj.src;
		var dW = oldImage.width;
		var dH = oldImage.height;
	}

	originalw=dW;
	originalh=dH;


	if (dW>MaxW || dH>MaxH) {
		a = dW/MaxW;
		b = dH/MaxH;
		if (b>a) {
			a = b;
		}
		dW = dW/a;
		dH = dH/a;
	}
	if (dW>0 && dH>0) {
		imageObject.width = dW;
		imageObject.height = dH;
	}
}
function edit_module(span_obj){
	refresh_top();
	document.getElementById("dragEdits").style.display="";
	document.getElementById("jobedit").src="ajax.php?inc="+span_obj.offsetParent.offsetParent.id;
}
function edit_other(url){
	refresh_top();
	document.getElementById("dragEdits").style.display="";
	document.getElementById("jobedit").src=url;
}
function refresh_top(){
	tops=window.document.body.clientHeight + window.document.body.scrollTop-430;
	lefts=((window.document.body.clientWidth + window.document.body.scrollWidth)/2-725)/2;
	document.getElementById("dragEdits").style.top=tops;
	document.getElementById("dragEdits").style.left=lefts;
	setTimeout( "refresh_top()" , 10 );
}
document.write('<div class="dragEdits" id="dragEdits" style="display:none;position:absolute;width:725px;height:430px;background:transparent;overflow:hidden;top:180px;z-index:9999;" ><iframe name="jobedit" id="jobedit" src="about:blank" width="100%" height="100%" scrolling="no" ></iframe></div>');

var AJAX={
	http_request:false,
	DivObj:null,
	waitstate:null,
	success:null,
	get:function (divid,url,type) {
		AJAX.http_request = false;
		AJAX.DivObj = document.getElementById(divid);
		if(window.XMLHttpRequest) { //Mozilla 浏览器
			AJAX.http_request = new XMLHttpRequest();
			if (AJAX.http_request.overrideMimeType) {//设置MiME类别
				AJAX.http_request.overrideMimeType('text/xml');
			}
		}else if (window.ActiveXObject) { // IE浏览器
			try {
				AJAX.http_request = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
				try {
					AJAX.http_request = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {}
			}
		}
		if (!AJAX.http_request) {
			window.alert("不能创建XMLHttpRequest对象实例.");
			return false;
		}
		AJAX.http_request.onreadystatechange = AJAX.processRequest;
		var step=(type==1)?true:false;
		AJAX.http_request.open("GET", url+"&"+Math.random(), step);
		AJAX.http_request.send(null);
	},
    processRequest:function () {
        if (AJAX.http_request.readyState == 4) {
            if (AJAX.http_request.status == 200) {
				if(AJAX.DivObj!=null){
					AJAX.DivObj.innerHTML=AJAX.http_request.responseText;
				}
            } else {
                alert("您所请求的页面有异常。");
            }
        }else{
			if(AJAX.DivObj!=null){
				AJAX.DivObj.innerHTML='<hr>页面正加载中,请等稍候...<hr>';
			}
		}
    }
}

function ShowEditMenu(url,w,h){
	var evt = (evt) ? evt : ((window.event) ? window.event : "");
	if (evt) {
		 ao = (evt.target) ? evt.target : evt.srcElement; //当前事件发生元素
	}
	position=get_position(ao);
	DivId="ajaxdiv"+w+h+url.replace(/([^\?]+)\?([^=]+)=([^&]+)(.*)/g, '$3');
	obj=document.getElementById(DivId);
	if(obj==null){
		obj=document.createElement("div");
		obj.innerHTML=document.getElementById('AjaxEditTable').outerHTML;
		objs=obj.getElementsByTagName("TD");
		//obj.id=DivId;
		objs[1].id=DivId;
		//obj.className="Editdiv";
		obj.style.Zindex='999';
		obj.style.position='absolute';
		obj.style.top=position.bottom;
		obj.style.left=position.left
		obj.style.height=h;
		obj.style.width=w;
		document.body.appendChild(obj);
		//obj.innerHTML='以下是显示内容...';
		AJAX.get(DivId,url,1);
	}else{
		fobj=obj.offsetParent.offsetParent;
		if(fobj.style.display=='none'){
			fobj.style.display='';
		}else{
			fobj.style.display='none';
		}
	}
}

function get_position(o){//取得坐标
	var to=new Object();
	to.left=to.right=to.top=to.bottom=0;
	var twidth=o.offsetWidth;
	var theight=o.offsetHeight;
	while(o!=document.body){
		to.left+=o.offsetLeft;
		to.top+=o.offsetTop;
		o=o.offsetParent;
	}
	to.right=to.left+twidth;
	to.bottom=to.top+theight;
	return to;
}

function choose_stylefile(cssfile){
	AJAX.get('',"ajax.php?inc=ol_style&step=2&style="+cssfile,1);
	
	Mycss.styleSheet.cssText="";
	var head = document.getElementsByTagName('HEAD').item(0); 
	if((cssobj=document.getElementById('Testcss'))!=null){
		cssobj.styleSheet.cssText="";
		head.removeChild(cssobj);
	}
	var style = document.createElement('link'); 
	style.href = "images/skin/"+cssfile+"/css.css"; 
	style.rel = 'stylesheet'
	style.type = 'text/css';
	style.id = 'Testcss'; 
	head.appendChild(style); 
	//本来以下两句简单的话,就能处理了.但是浏览器总死掉
	//Testcss.styleSheet.cssText="";
	//Testcss.styleSheet.addImport("images/skin/"+cssfile+"/css.css"+'?'+Math.random());
}

function choose_layout(num){
	AJAX.get('',"ajax.php?inc=ol_layout&step=2&layout="+num,1);
	window.location.reload();
}

function choose_module(o,moduleid){
	
	ob=document.getElementById(moduleid);
	if(o.checked==true){
		if(ob!=null){
			ob.style.display='';
		}else{
			obj=document.createElement("div");
			obj.id='_'+moduleid;
			document.getElementById("MainTd_0").appendChild(obj);
			//Fobj=document.getElementById("MainTd_0").getElementsByTagName("table");
			//alert(Fobj[0].id)
			//document.getElementById("MainTd_0").insertBefore(obj,Fobj[0]);
			AJAX.get(obj.id,"ajax.php?inc=ol_module&step=2&moduleid="+moduleid,0);
		}
	}else{
		ob.style.display='none';
	}
	
	move_module(); //模块移动
	Drag.inint(); //重新初始化可移动的层
	
}
function ShowDoEdit(){
	obj=document.getElementById('EditMenuTable');
	if(obj.style.display=='none'){
		setCookie('showDoEdit',1);
		obj.style.display='';
	}else{
		setCookie('showDoEdit',0);
		obj.style.display='none'
	}
}









/**** 拖拽效果  *****/
	
var Drag={dragged:false,
		ao:null,
		tdiv:null,
dragStart:function(evt){
	var evt = (evt) ? evt : ((window.event) ? window.event : "");
	if (evt) {
		 Drag.ao = (evt.target) ? evt.target : evt.srcElement; //当前事件发生元素
	}
	
	
	if((Drag.ao.tagName=="TD")||(Drag.ao.tagName=="TR")){
		Drag.ao=Drag.ao.offsetParent;
		Drag.ao.style.zIndex=100;
	}else
		return;
	Drag.dragged=true;
	Drag.tdiv=document.createElement("div");
	//
	oo=Drag.ao.getElementsByTagName("span");
	Drag.tdiv.innerHTML=oo[0].innerHTML;
	//Drag.tdiv.innerHTML=Drag.ao.outerHTML;
	//Drag.ao.style.border="5px dashed red";
	Drag.tdiv.style.display="block";
	Drag.tdiv.style.position="absolute";
	Drag.tdiv.style.filter="alpha(opacity=70)";
	//Drag.tdiv.style.cursor="move";
	//Drag.tdiv.style.border="5px solid red";
	Drag.tdiv.className='DragDiv';
	Drag.tdiv.style.width=Drag.ao.offsetWidth;
	Drag.tdiv.style.height=Drag.ao.offsetHeight;
	Drag.tdiv.style.top=Drag.getInfo(Drag.ao).top;
	Drag.tdiv.style.left=Drag.getInfo(Drag.ao).left;
	document.body.appendChild(Drag.tdiv);
	Drag.lastMouseX=evt.clientX+document.body.scrollLeft;
	Drag.lastMouseY=evt.clientY+document.body.scrollTop;
	Drag.lastLeft=Drag.tdiv.style.left;
	Drag.lastTop=Drag.tdiv.style.top;

}
,
 draging:function(tevt){//重要:判断MOUSE的位置
	if(!Drag.dragged||Drag.ao==null)return;
	var tevt = (tevt) ? tevt : ((window.event) ? window.event : "");
	var tX=tevt.clientX+document.body.scrollLeft;
	var tY=tevt.clientY+document.body.scrollTop;

	Drag.tdiv.style.left=parseInt(Drag.lastLeft)+tX-Drag.lastMouseX;
	Drag.tdiv.style.top=parseInt(Drag.lastTop)+tY-Drag.lastMouseY;
	var MainTable=document.getElementById("MainTable");
	for(var r=0;r<MainTable.rows.length;r++){//循环出 MainTable 有多少行
		for(var i=0;i<MainTable.rows[r].cells.length;i++){//循环出 MainTable 有多少列
			var parentCell=Drag.getInfo(MainTable.rows[r].cells[i]);
			if(tX>=parentCell.left&&tX<=parentCell.right&&tY>=parentCell.top&&tY<=parentCell.bottom){
				var subTables=MainTable.rows[r].cells[i].getElementsByTagName("table");
				if(subTables.length==0){
					if(tX>=parentCell.left&&tX<=parentCell.right&&tY>=parentCell.top&&tY<=parentCell.bottom){
						MainTable.rows[r].cells[i].appendChild(Drag.ao);
					}
					break;
				}
				for(var j=0;j<subTables.length;j++){
					var subTable=Drag.getInfo(subTables[j]);
					if(tX>=subTable.left&&tX<=subTable.right&&tY>=subTable.top&&tY<=subTable.bottom){
						MainTable.rows[r].cells[i].insertBefore(Drag.ao,subTables[j]);
						break;
					}else{
						MainTable.rows[r].cells[i].appendChild(Drag.ao);
					}	
				}
			}
		}
	}
}
,
 dragEnd:function(){
	if(!Drag.dragged)return;
	Drag.dragged=false;
	Drag.mm=Drag.repos(150,15);
/*	Drag.ao.style.borderWidth="0px";
	Drag.ao.style.borderTop="1px solid #3366cc";

	Drag.tdiv.style.borderWidth="0px";*/
	/*修改，防止拖动后无边框*/
	
	//Drag.ao.style.border="1px solid #006666";
	Drag.ao.style.zIndex=1;
	move_module(); //模块移动
},
getInfo:function(o){//取得坐标
	var to=new Object();
	to.left=to.right=to.top=to.bottom=0;
	var twidth=o.offsetWidth;
	var theight=o.offsetHeight;
	while(o!=document.body){
		to.left+=o.offsetLeft;
		to.top+=o.offsetTop;
		o=o.offsetParent;
	}
		to.right=to.left+twidth;
		to.bottom=to.top+theight;
	return to;
},
repos:function(aa,ab){
	var o=Drag.tdiv.style.opacity;
	var f=Drag.tdiv.style.filter;
	var tl=parseInt(Drag.getInfo(Drag.tdiv).left);
	var tt=parseInt(Drag.getInfo(Drag.tdiv).top);
	var kl=(tl-Drag.getInfo(Drag.ao).left)/ab;
	var kt=(tt-Drag.getInfo(Drag.ao).top)/ab;
	var kf=f/ab;
	return setInterval(function(){if(ab<1){
							clearInterval(Drag.mm);
							Drag.tdiv.parentNode.removeChild(Drag.tdiv)
							Drag.ao=null;
							return;
						}
					ab--;
					tl-=kl;
					tt-=kt;
					f-=kf;
					Drag.tdiv.style.left=parseInt(tl)+"px";
					Drag.tdiv.style.top=parseInt(tt)+"px";
					Drag.tdiv.style.opacity=o;
					Drag.tdiv.style.filter=f;
				}
,aa/ab)
},
 inint:function(){//初始化
	//alert('sssf');
	var MainTable=document.getElementById("MainTable");
	for(var r=0;r<MainTable.rows.length;r++){
		for(var i=0;i<MainTable.rows[r].cells.length;i++){
			var subTables=MainTable.rows[r].cells[i].getElementsByTagName("table");
			for(var j=0;j<subTables.length;j++){
				if(subTables[j].className!="dragTable")break;
				subTables[j].rows[0].style.cursor="move";
				if (document.all) { //For IE
					subTables[j].rows[0].attachEvent("onmousedown",Drag.dragStart);
				}else{ //For Mozilla
					subTables[j].rows[0].addEventListener("onmousedown".substr(2,"onmousedown".length-2),Drag.dragStart,true);
				}
			}
		
			document.onmousemove=Drag.draging;
			document.onmouseup=Drag.dragEnd;
		}
	}
	/*
	var s=document.getElementsByTagName("tr");
	for(var i=0;i<s.length;i++){
		if(s[i].className=='dragTR'){
			if (document.all) { //For IE
				s[i].attachEvent("onmousedown",Drag.dragStart);
			}else{ //For Mozilla
				s[i].addEventListener("onmousedown".substr(2,"onmousedown".length-2),Drag.dragStart,true);
			}
			document.onmousemove=Drag.draging;
			document.onmouseup=Drag.dragEnd;
		}	
	}*/
//end of Object Drag
}
}



function listid(del_id){
	var list='';
	var isdrogtable=0;
	var MainTable=document.getElementById("MainTable");
	for(var r=0;r<MainTable.rows.length;r++){
		for(var i=0;i<MainTable.rows[r].cells.length;i++){
			if(isdrogtable!=0)list+="|";
			var jj=0;
			isdrogtable++;
			var subTables=MainTable.rows[r].cells[i].getElementsByTagName("table");
			for(var j=0;j<subTables.length;j++){
				if(subTables[j].className!="dragTable")continue;//break;
				vid=subTables[j].id;
				if(del_id==vid||subTables[j].style.display=='none')continue;
				jj++;
				if(jj==1){
					list+=vid;
				}else{
					list+=","+vid;
				}
				
				//subTables[j].rows[0].className="dragTR";
			}
		}
	}
	if(del_id==1){
		alert(list);
	}else{
		return list;
	}
	
}



//删除模块
function delete_module(span_obj){
	var isdel=confirm("你确实要禁止显示此模块吗?");
	if(isdel==true){
		table_obj=span_obj.offsetParent.offsetParent;
		table_obj.style.display='none';
		document.formi.module.value=listid(table_obj.id);
		document.formi.submit();
	}
}
//移动模块
function move_module(){
	old_list=document.formi.module.value;
	new_list=listid('');
	if(old_list!="" && old_list!=new_list){
		//var ifchange=confirm("已移动是否确定修改\n"+old_list+"\n"+new_list);
		//if(ifchange==true){
			document.formi.module.value=new_list;
			document.formi.submit();
			//alert('修改成功');
		//}
	}
}
document.write('<form id="formi" name="formi" method="post" action="ajax.php?inc=change_module" target="phpframe"><input type="hidden" name="module" value="<?=$VlogCfg[module]?>"/></form>');
document.write('<iframe id="phpframe" name="phpframe" marginwidth="0" src="about:blank" frameborder="0" height="0" width="0"></iframe>');



var change_tagName={
	tagid:null,
	init:function(){
		var MainTable=document.getElementById("MainTable");
		oo=MainTable.getElementsByTagName("SPAN");
		for(var i=0;i<oo.length;i++){
			if(oo[i].className=='Tag'){
				//txt=txt+oo[i].innerHTML+oo[i].offsetParent.offsetParent.id+';';
				oo[i].title='点击可以修改这里的文字';
				if (document.all) { //For IE
					oo[i].attachEvent("onmousedown",change_tagName.showdiv);
					oo[i].attachEvent("onmouseover",change_tagName.showstyle);
					oo[i].attachEvent("onmouseout",change_tagName.hidestyle);
				}else{ //For Mozilla
					oo[i].addEventListener("onmousedown".substr(2,"onmousedown".length-2),change_tagName.showdiv,true);
				}
			}
		}
	},
	showstyle:function(evt){
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		ao.style.border='1px dotted red';
		ao.style.cursor='hand';
	},
	hidestyle:function(evt){
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		ao.style.border='0px dotted red';
	},
	showdiv:function(evt){
		var w=100;
		var h=100;
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		
		txt=ao.innerHTML;
		oid=ao.offsetParent.offsetParent.id;
		ao.id="TagId_"+oid;	//方便处理即时显示修改后的内容
		//获取坐标的函数头部有定义
		position=get_position(ao);
		DivId="ChangTagName_"+oid;
		url="ajax.php?inc=ol_ChangTagName&TagValue="+txt+"&TagId="+oid+"&DivId="+DivId;
		obj=document.getElementById(DivId);
		if(obj==null){
			obj=document.createElement("div");
			obj.innerHTML=document.getElementById('AjaxEditTable').outerHTML;
			objs=obj.getElementsByTagName("TD");
			objs[1].id=DivId;
			//obj.id=DivId;
			//obj.className="Editdiv";
			obj.style.Zindex='999';
			obj.style.position='absolute';
			obj.style.top=position.bottom;
			obj.style.left=position.left
			obj.style.height=h;
			obj.style.width=w;
			document.body.appendChild(obj);
			//obj.innerHTML='以下是显示内容...';
			AJAX.get(DivId,url,1);
		}else{
			fobj=obj.offsetParent.offsetParent;
			if(fobj.style.display=='none'){
				fobj.style.display='';
			}else{
				fobj.style.display='none';
			}
		}
	},
	saveTag:function(divid,oid,va){
		//GET方式提交内容,如果有空格的话.会有BUG
		document.getElementById("TagId_"+oid).innerHTML=va;	//即时显示,不过没判断是否保存成功也显示了
		AJAX.get(divid,"ajax.php?inc=ol_ChangTagName&step=2&TagId="+oid+"&DivId="+divid+"&Tagvalue="+va,1);
	},
	cancelTag:function(divid){
		//document.getElementById(divid).style.display='none';
		document.getElementById(divid).offsetParent.offsetParent.style.display='none';
	}
}


var EditModule={
	tableid:null,
	init:function(){
		var MainTable=document.getElementById("MainTable");
		oo=MainTable.getElementsByTagName("SPAN");
		for(var i=0;i<oo.length;i++){
			if(oo[i].className=='eidtmodule'&&oo[i].getAttribute("onclick")==null){
				oo[i].title='点击可以修改这里的设置';
				if (document.all) { //For IE
					oo[i].attachEvent("onmousedown",EditModule.showdiv);
				}else{ //For Mozilla
					oo[i].addEventListener("onmousedown".substr(2,"onmousedown".length-2),EditModule.showdiv,true);
				}
			}
		}
	},
	showdiv:function(evt){
		var w=150;
		var h=100;
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		oid=ao.offsetParent.offsetParent.id;
		//获取坐标的函数头部有定义
		position=get_position(ao);
		DivId="EditModule_"+oid;
		url="ajax.php?inc=ol_EditModule&TagId="+oid+"&DivId="+DivId;
		obj=document.getElementById(DivId);
		if(obj==null){
			obj=document.createElement("div");

			obj.innerHTML=document.getElementById('AjaxEditTable').outerHTML;
			objs=obj.getElementsByTagName("TD");
			objs[1].id=DivId;
			//obj.id=DivId;
			//obj.className="Editdiv";
			obj.style.Zindex='999';
			//obj.style.display='';
			obj.style.position='absolute';
			obj.style.top=position.bottom;
			obj.style.left=position.left-w+30;
			obj.style.height=h;
			obj.style.width=w;
			document.body.appendChild(obj);
			//obj.innerHTML='以下是显示内容...';
			AJAX.get(DivId,url,1);
		}else{
			fobj=obj.offsetParent.offsetParent;
			if(fobj.style.display=='none'){
				fobj.style.display='';
			}else{
				fobj.style.display='none';
			}
		}
	},
	save:function(divid,oid,va){
		//alert(oid)
		//GET方式提交内容,如果有空格的话.会有BUG
		//即时显示,不过没判断是否保存成功也显示了
		if(oid=='intro'&&upFileIframe.document.form1!=null){
			if(upFileIframe.document.form1.postfile.value!=''){
				//upFileIframe.document.form1.submit();
				alert("你刚才选择了一张图片,请先上传");
				return false;
			}
		}//alert("ajax.php?inc=ol_EditModule&step=2&TagId="+oid+"&DivId="+divid+"&va="+va);
		va=va.replace(/(\n)/g,"@BR@");
		AJAX.get(divid,"ajax.php?inc=ol_EditModule&step=2&TagId="+oid+"&DivId="+divid+"&va="+va,0);
		/*
		//以下转了一个弯是因为table不能使用innerHTML
		fobj=document.getElementById(oid).offsetParent;
		Newobj=document.createElement("div");
		Newobj.id="New_"+oid;
		fobj.insertBefore(Newobj,document.getElementById(oid));
		fobj.removeChild(document.getElementById(oid));
		Newobj.id=oid;
		AJAX.get(oid,"ajax.php?inc=ol_module&step=2&moduleid="+oid+"&ajax=1",0);
		*/

		fs=document.getElementById(oid).getElementsByTagName("TD");
		fs[1].id="TD_"+oid;
		
		AJAX.get(fs[1].id,"ajax.php?inc=ol_module&step=2&moduleid="+oid+"&ajax=1",0);

		//change_tagName.init();	//重新初始化
		//EditModule.init();	//重新初始化
		//move_module(); //模块移动.可不用,防止而已
		//Drag.inint(); //重新初始化可移动的层
	},
	cancel:function(divid){
		document.getElementById(divid).offsetParent.offsetParent.style.display='none';
	},
	upfile:function(url,name,size,va){
		document.getElementById("introImgInput").value=url;
	}
}

var clickEdit={
	tableid:null,
	init:function(){
		oo=document.body.getElementsByTagName("A");
		for(var i=0;i<oo.length;i++){
			if(oo[i].getAttribute("editurl")!=null){
				if(oo[i].getAttribute("href")!=null)oo[i].href='javascript:';
				oo[i].title='点击可以修改这里的设置';
				if (document.all) { //For IE
					oo[i].attachEvent("onmousedown",clickEdit.showdiv);
					oo[i].attachEvent("onmouseover",clickEdit.showstyle);
					oo[i].attachEvent("onmouseout",clickEdit.hidestyle);
				}else{ //For Mozilla
					oo[i].addEventListener("onmousedown".substr(2,"onmousedown".length-2),clickEdit.showdiv,true);
				}
			}
		}
	},
	showstyle:function(evt){
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		ao.style.border='1px dotted red';
		ao.style.cursor='hand';
	},
	hidestyle:function(evt){
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		ao.style.border='0px dotted red';
	},
	showdiv:function(evt){
		var w=150;
		var h=100;
		var evt = (evt) ? evt : ((window.event) ? window.event : "");
		if (evt) {
			 ao = (evt.target) ? evt.target : evt.srcElement;
		}
		//oid=ao.offsetParent.offsetParent.id;
		//获取坐标的函数头部有定义
		position=get_position(ao);
		
		//alert(oid);
		url=ao.getAttribute("editurl");
		oid=url.replace(/(\.|=|\?|&)/g,"_");
		ao.id=oid;
		DivId="clickEdit_"+oid;
		url=url + "&TagId=" + oid;
		obj=document.getElementById(DivId);
		if(obj==null){
			obj=document.createElement("div");

			obj.innerHTML=document.getElementById('AjaxEditTable').outerHTML;
			objs=obj.getElementsByTagName("TD");
			objs[1].id=DivId;
			//obj.id=DivId;
			//obj.className="Editdiv";
			obj.style.Zindex='999';
			//obj.style.display='';
			obj.style.position='absolute';
			obj.style.top=position.bottom;
			obj.style.left=position.left;
			obj.style.height=h;
			obj.style.width=w;
			document.body.appendChild(obj);
			//obj.innerHTML='以下是显示内容...';
			AJAX.get(DivId,url,1);
		}else{
			fobj=obj.offsetParent.offsetParent;
			if(fobj.style.display=='none'){
				fobj.style.display='';
			}else{
				fobj.style.display='none';
			}
		}
	},
	save:function(oid,job,va){
		divid="clickEdit_"+oid;
		//alert(oid)
		//GET方式提交内容,如果有空格的话.会有BUG
		//即时显示,不过没判断是否保存成功也显示了
		document.getElementById(oid).innerHTML=va;
		va=va.replace(/(\n)/g,"@BR@");
		AJAX.get(divid,"ajax.php?inc="+job+"&step=2&TagId="+oid+"&va="+va,0);
	},
	cancel:function(divid){
		document.getElementById(divid).offsetParent.offsetParent.style.display='none';
	}
}

function getlength(str){
	var Goodureg = /^[\u4E00-\u9FA5]*$/;
	var GooduLength = 0;
	var GooduString = str;
	for(var i = 0 ; i < str.length; i ++){
		if(Goodureg.test(GooduString.charAt(i)) == true){
			GooduLength = GooduLength + 2;
		}else{
			GooduLength = GooduLength + 1;
		}
	}
	return GooduLength;
}

//导航菜单的自动适应长度
function _navigation(){
	obj=document.getElementById("navigation").getElementsByTagName("A");
	for(var i=0;i<obj.length;i++){
		//alert(getlength(obj.innerText));
		leng=getlength(obj[i].innerText);
		width=leng*7+20;
		obj[i].style.width=width;
	}
}

function makesmallpic(obj,w,h){
	var srcImage = new Image();
	srcImage.src=obj.src;
	var srcW=srcImage.width;		
	var srcH=srcImage.height;

	if (srcW==0)
	{
		obj.src=srcImage.src;
		srcImage.src=obj.src;
		var srcW=srcImage.width;		
		var srcH=srcImage.height;
	}
	if (srcH==0)
	{
		obj.src=srcImage.src;
		srcImage.src=obj.src;
		var srcW=srcImage.width;		
		var srcH=srcImage.height;
	}

	if(srcW>srcH){
		if(srcW>w){
			obj.width=newW=w;
			obj.height=newH=(w/srcW)*srcH;
		}else{
			obj.width=newW=srcW;
			obj.height=newH=srcH;
		}
	}else{
		if(srcH>h){
			obj.height=newH=h;
			obj.width=newW=(h/srcH)*srcW;
		}else{
			obj.width=newW=srcW;
			obj.height=newH=srcH;
		}
	}
	if(newW>w){
		obj.width=w;
		obj.height=newH*(w/newW);
	}else if(newH>h){
		obj.height=h;
		obj.width=newW*(h/newH);
	}
}