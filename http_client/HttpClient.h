#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__
#include "common.h"

/*
 *  FileName:       HTTP_CLIENT
 *  Author:         ldak47
 *  Version:        1.0
 *  Data:           20181105
 *  Description:    http是基于tcp传输层协议的应用层协议, 所以本质上还是tcp网络传输报文, 但包含http应用层的协议内容
 * */

namespace HttpClientNs {

    extern std::vector<std::string> UserAgents;
	const std::string Content_Type = "Content-Type: ";
	const std::string Content_Type_html  = Content_Type + static_cast<std::string>("text/html");
	const std::string Content_Type_plain = Content_Type + static_cast<std::string>("text/plain");
	const std::string Content_Type_jpeg  = Content_Type + static_cast<std::string>("text/jpeg");
	const std::string Content_Type_zip   = Content_Type + static_cast<std::string>("text/zip");
	
	struct HttpClientRequest {
	    static const std::string CRLF;
	    std::string content_type = "text/html";
	    std::string content_length;
	
	    /* request line */
	    std::string method;
	    std::string host;
	    std::string locate;
	    std::string version = "HTTP/1.1";
	    void set_version (const std::string &_v) { version = _v; }
	
	    /* request header */
	    const std::vector<std::string> &user_agent = std::cref(UserAgents);
	    std::string set_user_agent() {
            std::random_device rd;
            return user_agent[rd() % user_agent.size()];
        }
	
	    //notice server where I was at before request now. should be setup referer on the webpage-url where the resource on.
	    std::string lastlocate = "";
	    std::string referer = "";
        void MakeReferer();
	
	    std::string date;
        void MakeGmtDate();
	
	    //accept should be between follow three selections in other case
	    static const std::string ACCEPT_HTML;
	    static const std::string ACCEPT_JSON ;
	    static const std::string ACCEPT_IMAGE;
	    std::string accept = const_cast<std::string &>(ACCEPT_HTML);
	    std::string accept_chardet = "Accept-Chardet: utf-8,gbk";
	    //crwaler should not setup this item
	    std::string accept_encoding = "";
	    std::string accept_language = "Accept-Language: zh-CN,zh;q=0.9";
	    std::string accept_range = "Accept-Range: bytes";
	
	    //use for when need authored
	    std::string authorization;
	    //means that client could automaticly upgrade from http to https
	    std::string upgrade_insecure_requests = "Upgrade-Insecure-Requests: 1";
	
	    std::string connection = "Connection: keep-alive";
	
	    //to maintain session status, 
	    std::string cookie = "";
	    
	    //for cache-contril, maybe use follows:
	    //1. no-cache: in fact, no-cache use caches, server will judge whether web object changed, if changed, send the newest content and http-200 to client, if not changed, send http-304 to client.
	    //2. no-store: in fact, no-store means not use cache, server always send the newest content and http-200 to client.
	    //3. max-age: set how many seconds client could trust its cache
	
	    //cache-control is more prioritier than expires
	    std::string cache_control = "Cache-Control: no-cache, no-store";
	    //pragma is familiar to cache-control
	    std::string pragma = "Pragma: no-cache";
	    std::string expires = "";
	
	    //ETag is 'tag'(hash) of a web object, if web object changed, ETag will change. ETag is created by server, when client first reqeust the server and get ETag, then client could send ETag
	    //to server when request server again, if web object not change yet, server would return http-304 to client means that web object not change, or return http-200 if web object changed.
	    //ETag is more prioritier than last-modified
	    //server may return to client: 301: Move Permanently, 302: Found, 304: Not Modified
	    //for client, how to send ETag to server:  it should send to server "If-None-Match: {ETag}" or "If-Match: {ETag}" to server when request server after first request
	    std::string etag = "";
	    std::string if_none_match = "If-None-Match: ";
	    //this field is recved from server's response, such as "Last-Modified: Fri, 12 Oct 2018 08:53:24 GMT"
	    //it means, before 'last_modified', client could trust its cache
	    //for client, how to judge if modified: it should send to server "If-Modified-Since: {A GMT TIME}" to server when request server after first request
	    std::string last_modified = "";
	    std::string if_modified_since = "If-Modified-Since: ";
	
        //if ajax request, setup "x-requested-with: XMLHttpRequest"
	    bool x_requested_with = false;

        /* query args */
        std::map<std::string, std::string> query_args;
        int GetDataLen() {
            
        }
	
	    explicit HttpClientRequest(const std::string &_host, const std::string &_locate, const std::string &_method = "GET", \
	                              const std::string &_version = "HTTP/1.1", const std::string &_connection = "Keep-Alive"): host(_host), method(_method), locate(_locate), version(_version), connection(_connection) {
	        std::transform(method.begin(), method.end(), method.begin(), toupper);
	    }
	    ~HttpClientRequest() = default;
	
        std::string MakeHeader();
        std::string MakeGet();
        std::string MakePost();
	    std::string MakeRequest() {
	        if (method == "GET") {
	            return MakeGet();
	        } else if (method == "POST") {
	            return MakePost();
	        } else {
	            return "";
	        }
	    }
	};
	
	
	
	    
	
	
	
};
#endif
