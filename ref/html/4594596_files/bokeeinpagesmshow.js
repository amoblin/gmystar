	function getCookiesc(name){var search, offset;search = name + "=";offset = document.cookie.indexOf(search);if (offset != -1){offset += search.length;end = document.cookie.indexOf(";", offset);if (end == -1){end = document.cookie.length;}return unescape(document.cookie.substring(offset, end));}else return false;}function setCookiesc(name, value, expires){var expdate = new Date();var argv = setCookiesc.arguments;var argc = setCookiesc.arguments.length;var expires = (argc > 2) ? argv[2] : null;var path = (argc > 3) ? argv[3] : null;var numdian = document.domain.split(".");var numdianleng = numdian[0].length;var domain = document.domain.substring(numdianleng+1);var secure = (argc > 5) ? argv[5] : false;document.cookie = name + "=" + escape (value) +((expires == null) ? "" : ("; expires="+ expires.toGMTString()))+((path == null) ? "" : ("; path=" + path)) +((domain == null) ? "" : ("; domain=" + domain))+((secure == true) ? "; secure" : "");var numfen = document.cookie.split(";");if(document.cookie == "" || numfen.length==1){document.cookie = name + "=" + escape(value) + "; expires=" + expires.toGMTString() +  "; path=/";}}
	var tem11 = 'AllyeSCviewtvExpsmartmedia_448';
	function creativeCookie(){setCookiesc(tem11, AllyesviewtvCookieVal, AllyesviewtvExpDate);}
	var AllyesviewtvCookieVal = getCookiesc(tem11);
	var AllyesviewtvExpDate=  new Date();
	AllyesviewtvExpDate.setTime(AllyesviewtvExpDate.getTime() + 60000*0);
	if (!AllyesviewtvCookieVal || AllyesviewtvCookieVal <= 1)
	{
		if(!AllyesviewtvCookieVal) AllyesviewtvCookieVal=1;
		AllyesviewtvCookieVal = parseInt(AllyesviewtvCookieVal)+1;
		document.write("<SCRIPT LANGUAGE='JavaScript1.1' CHARSET='GBK'  SRC='http://smartmedia.allyes.com/main/s?user=bokee|inpage|sm&db=smartmedia&border=0&local=yes&js=ie&kv="+kv+"'></SCR"+"IPT>");
	}