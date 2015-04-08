/*
 ADOBE CONFIDENTIAL
 ___________________

 Copyright 2011 Adobe Systems Incorporated
 All Rights Reserved.

 NOTICE:  All information contained herein is, and remains
 the property of Adobe Systems Incorporated and its suppliers,
 if any.  The intellectual and technical concepts contained
 herein are proprietary to Adobe Systems Incorporated and its
 suppliers and may be covered by U.S. and Foreign Patents,
 patents in process, and are protected by trade secret or copyright law.
 Dissemination of this information or reproduction of this material
 is strictly forbidden unless prior written permission is obtained
 from Adobe Systems Incorporated.
*/
(function(a){a.fn.museOverlay=function(b){var c=a.extend({autoOpen:!0,offsetLeft:0,offsetTop:0,$overlaySlice:a(),$overlayWedge:a(),duration:300,overlayExtraWidth:0,overlayExtraHeight:0,$elasticContent:a()},b);return this.each(function(){var d=a(this).data("museOverlay");if(d&&d[b]!==void 0)return d[b].apply(this,Array.prototype.slice.call(arguments,1));var f=a("<div></div>").appendTo("body").css({position:"absolute",top:0,left:0,zIndex:100001}).hide(),g=a("<div></div>").append(c.$overlaySlice).appendTo(f).css({position:"absolute",
top:0,left:0}),k=a(this).css({position:"absolute",left:0,top:0}).appendTo(f),h=a(window),j=h.data("scrollWrapper"),i,l,m=null,p=c.$elasticContent,q=p.length?parseInt(p.css("padding-left"))+parseInt(p.css("padding-right"))+parseInt(p.css("border-left-width"))+parseInt(p.css("border-right-width")):0,o=p.length?parseInt(p.css("padding-top"))+parseInt(p.css("padding-bottom"))+parseInt(p.css("border-top-width"))+parseInt(p.css("border-bottom-width")):0,n={isOpen:!1,open:function(){if(!n.isOpen)i=h.width(),
l=h.height(),n.positionContent(i,l),f.show(),g.bind("click",n.close),g.css({opacity:0}).stop(!0),k.css({opacity:0}).stop(!0),g.bind("click",n.close).animate({opacity:0.99},{queue:!1,duration:c.duration,complete:function(){g.css({opacity:""});k.animate({opacity:1},{queue:!1,duration:c.duration,complete:function(){k.css({opacity:""});n.applyPageDimensions()}})}}),a(document).bind("keydown",n.onKeyDown),n.doLayout(i,l),n.isOpen=!0,h.bind("resize",n.onWindowResize)},close:function(){a(".Container",k).each(function(){Muse.Utils.detachIframesAndObjectsToPauseMedia(a(this))});
g.unbind("click",n.close);h.unbind("resize",n.onWindowResize);a(document).unbind("keydown",n.onKeyDown);if(c.onClose)c.onClose();g.css({opacity:0.99}).stop(!0);k.css({opacity:0.99}).stop(!0);k.animate({opacity:0},{queue:!1,duration:c.duration,complete:function(){g.animate({opacity:0},{queue:!1,duration:c.duration,complete:function(){f.hide();k.css({opacity:""});g.css({opacity:""})}})}});n.isOpen=!1},onKeyDown:function(a){a.keyCode===27&&n.close()},onWindowResize:function(){var a=h.width(),b=h.height();
(i!=a||l!=b)&&m==null&&(m=setTimeout(function(){n.doLayout(h.width(),h.height());n.positionContent(h.width(),h.height());m=null},10))},doLayout:function(a,b){f.css({width:0,height:0});c.$overlayWedge.css({width:0,height:0});var d=a-q,g=b-o;p.length&&p.hasClass("fullwidth")&&(p.width(d),c.resizeSlidesFn&&c.resizeSlidesFn(d,g));n.applyPageDimensions()},applyPageDimensions:function(){var b=a(document),d=b.width(),b=b.height(),g=document.documentElement||document.body;g.clientWidth!=g.offsetWidth&&(d=
g.scrollWidth-1);g.clientHeight!=g.offsetHeight&&b<g.scrollHeight&&(b=g.scrollHeight-1);f.css({width:d,height:b});c.$overlayWedge.css({width:d-c.overlayExtraWidth,height:b-c.overlayExtraHeight})},positionContent:function(a,b){var d=(j||h).scrollLeft()+Math.max(0,a/2+c.offsetLeft),g=(j||h).scrollTop()+Math.max(0,b/2+c.offsetTop);k.css({top:g,left:d});p.length&&p.hasClass("fullwidth")&&p.css("left",-d);d=a-q;g=b-o;p.length&&(p.width(d),p.hasClass("fullscreen")&&p.height(g),c.resizeSlidesFn&&c.resizeSlidesFn(d,
g))}};k.data("museOverlay",n);c.autoShow&&n.open()})}})(jQuery);
;(function(){if(!("undefined"==typeof Muse||"undefined"==typeof Muse.assets)){var a=function(a,b){for(var c=0,d=a.length;c<d;c++)if(a[c]==b)return c;return-1}(Muse.assets.required,"jquery.museoverlay.js");if(-1!=a){Muse.assets.required.splice(a,1);for(var a=document.getElementsByTagName("meta"),b=0,c=a.length;b<c;b++){var d=a[b];if("generator"==d.getAttribute("name")){"2014.2.1.284"!=d.getAttribute("content")&&Muse.assets.outOfDate.push("jquery.museoverlay.js");break}}}}})();
