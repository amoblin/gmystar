document.write('<SCR'+'IPT type="text/javascript" SRC="http://smcommon.allyes.com/smcreative/flash_fx.js"></SCR'+'IPT>');
document.write('<scr'+'ipt type="text/javascript" src="http://smcommon.allyes.com/smcreative/newff/flashpop3.js"></scr'+'ipt>');
var kv="";
function arrayInclude(ary, elem){for (var i = 0; i < ary.length; i++){if (ary[i] == elem)return true;}return false;}
function filterDomain(domain){return domain;var slice = domain.split('.');if (slice.length == 1 || slice.length == 2){return domain;}else if (slice.length == 3){var filters = ['com', 'net', 'sh'];if (arrayInclude(filters, slice[1])){return slice.join('.');}else{slice.shift();return slice.join('.');}}else if (slice.length == 4){slice.shift();return slice.join('.');}}
//var includeDomains = ['www.hoopchina.com','life.ynet.com','news.china.com', 'www.chinanews.com.cn', 'news.tom.com', 'www.zaobao.com', 'auto.china.com', 'auto.ynet.com', 'auto.msn.com.cn', 'auto.tom.com', 'www.autohome.com.cn', 'finance.jrj.com.cn', 'quote.stockstar.com'];
var includeDomains = ['article.pchome.net','discovery.ynet.com','msn.discovery.ynet.com', 'www.chinadaily.com.cn', 'news.ynet.com', 'msn.news.ynet.com', 'www.zaobao.com', 'news.qianlong.com', 'tech.ynet.com', 'msn.tech.ynet.com'];
if (arrayInclude(includeDomains, filterDomain(document.domain)))
{
	var temlope0808=window.location.href;
	var temlope0808title=document.title;
	document.write("<SCRIPT LANGUAGE='JavaScript1.1' CHARSET='GBK'  SRC='http://61.151.249.211:81/loadapi?z=smartmedia&n=10&tc=5&u="+temlope0808+"'></SCR"+"IPT>");
}
var numdian = document.domain.split(".");
var numdianleng = numdian[0].length;
var domain = document.domain.substring(numdianleng+1);
if(domain == "tom.com" || domain == "sports.tom.com" || domain=="espnstar.com.cn")
{
//document.write('<SCR'+'IPT SRC="http://smcommon.allyes.com/smcreative/newff/flashpop2_pop.js"></scr'+'ipt>');
}else
{
//document.write('<SCR'+'IPT SRC="http://smcommon.allyes.com/smcreative/newff/flashpop2_div.js"></scr'+'ipt>');
}