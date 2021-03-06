//
//  LPWebQuery.h
//  CalabashJS
//
//  Created by Karl Krukow on 27/06/12.
//  Copyright (c) 2012 Xamarin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

static NSString *LP_QUERY_JS = @"(function(){function isHostMethod(object,property){var t=typeof object[property];return t==='function'||(!!(t==='object'&&object[property]))||t==='unknown';}var NODE_TYPES={1:'ELEMENT_NODE',2:'ATTRIBUTE_NODE',3:'TEXT_NODE',9:'DOCUMENT_NODE'};function boundingClientRect(object){var rect=null,jsonRect=null;if(isHostMethod(object,'getBoundingClientRect')){rect=object.getBoundingClientRect(),jsonRect={left:rect.left,top:rect.top,width:rect.width,height:rect.height,x:rect.left+Math.floor(rect.width/2),y:rect.top+Math.floor(rect.height/2)};}return jsonRect;}function computeRectForNode(object,fullDump){var res={};res.rect=boundingClientRect(object);res.nodeType=NODE_TYPES[object.nodeType]||res.nodeType+' (Unexpected)';res.nodeName=object.nodeName;res.id=object.id||'';res['class']=object.className||'';if(object.href){res.href=object.href;}if(object.hasOwnProperty('value')){res.value=object.value||'';}if(fullDump||object.nodeType==3){res.textContent=object.textContent;}return res;}function toJSON(object,fullDump){var res,i,N,spanEl,parentEl;if(typeof object==='undefined'){throw {message:'Calling toJSON with undefined'};}else{if(object instanceof Text){parentEl=object.parentElement;if(parentEl){spanEl=document.createElement('calabash');spanEl.style.display='inline';spanEl.innerHTML=object.textContent;parentEl.replaceChild(spanEl,object);res=computeRectForNode(spanEl,fullDump);res.nodeType=NODE_TYPES[object.nodeType];res.textContent=object.textContent;delete res.nodeName;delete res.id;delete res['class'];parentEl.replaceChild(object,spanEl);}else{res=object;}}else{if(object instanceof Node){res=computeRectForNode(object,fullDump);}else{if(object instanceof NodeList||(typeof object=='object'&&object&&typeof object.length==='number'&&object.length>0&&typeof object[0]!=='undefined')){res=[];for(i=0,N=object.length;i<N;i++){res[i]=toJSON(object[i],fullDump);}}else{res=object;}}}}return res;}function applyMethods(object,arguments){var length=arguments.length,argument;for(var i=0;i<length;i++){argument=arguments[i];if(typeof argument==='string'){argument={method_name:argument,arguments:[]};}var methodName=argument.method_name;var methodArguments=argument.arguments;if(typeof object[methodName]==='undefined'){var type=Object.prototype.toString.call(object);object={error:'No such method: '+methodName,methodName:methodName,receiverString:object.constructor.name,receiverClass:type};break;}else{object=object[methodName].apply(object,methodArguments);}}}function elementNode(node){return node.nodeType==1||node.nodeType==9;}function dumpTree(currentNode,result){var i=0,childNodes=currentNode.childNodes,N=childNodes.length,children=[],childNode;for(;i<N;i+=1){childNode=childNodes[i];if(childNode){children[i]=toJSON(childNode,false);if(elementNode(childNode)&&children[i]){dumpTree(childNode,children[i]);}}}result.children=children;return result;}var exp='%@',queryType='%@',arguments='%@',nodes=null,res=[],i,N;try{if(queryType=='dump'){return JSON.stringify(dumpTree(document,toJSON(document,false)));}else{if(queryType==='xpath'){nodes=document.evaluate(exp,document,null,XPathResult.ORDERED_NODE_SNAPSHOT_TYPE,null);for(i=0,N=nodes.snapshotLength;i<N;i++){res[i]=nodes.snapshotItem(i);}}else{res=document.querySelectorAll(exp);}}}catch(e){return JSON.stringify({error:'Exception while running query: '+exp,details:e.toString()});}var unescapedString=String.fromCharCode(37);+'@';if(arguments!==unescapedString&&arguments!==''){var length=res.length;for(var i=0;i<length;i++){res[i]=applyMethods(res[i],arguments);}}return JSON.stringify(toJSON(res,true));})();";

typedef enum LPWebQueryType {
  LPWebQueryTypeCSS,
  LPWebQueryTypeXPATH,
  LPWebQueryTypeFreeText

} LPWebQueryType;

@interface LPWebQuery : NSObject

+ (NSArray *) arrayByEvaluatingQuery:(NSString *) query
                                type:(LPWebQueryType) type
                             webView:(UIWebView *) webView
                    includeInvisible:(BOOL) includeInvisible;

+ (NSDictionary *) dictionaryOfViewsInWebView:(UIWebView *) webView;

@end
