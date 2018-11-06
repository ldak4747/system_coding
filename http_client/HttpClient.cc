#include "HttpClient.h"

namespace HttpClientNs {
	std::vector<std::string> UserAgents = {
	    "User-Agent:Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50",
	    "User-Agent:Mozilla/5.0 (Windows; U; Windows NT 6.1; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50",
	    "User-Agent:Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0;",
	    "User-Agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0)",
	    "User-Agent:Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)",
	    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:2.0.1) Gecko/20100101 Firefox/4.0.1",
	    "User-Agent:Mozilla/5.0 (Windows NT 6.1; rv:2.0.1) Gecko/20100101 Firefox/4.0.1",
	    "User-Agent:Opera/9.80 (Macintosh; Intel Mac OS X 10.6.8; U; en) Presto/2.8.131 Version/11.11",
	    "User-Agent:Opera/9.80 (Windows NT 6.1; U; en) Presto/2.8.131 Version/11.11",
	    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_0) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Maxthon 2.0)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; TencentTraveler 4.0)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; The World)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 360SE)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Avant Browser)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)",
	    "User-Agent:Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",
	    "User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0; HTC; Titan)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; InfoPath.2; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; 360SE)",
	    "User-Agent:Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)",
	    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_0) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11",
	    "User-Agent: Mozilla/5.0 (Linux; U; Android 2.2.1; zh-cn; HTC_Wildfire_A3333 Build/FRG83D) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
	    "User-Agent:Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50",
	    "User-Agent:Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; TencentTraveler 4.0; .NET CLR 2.0.50727)",
	    "User-Agent: MQQBrowser/26 Mozilla/5.0 (Linux; U; Android 2.3.7; zh-cn; MB200 Build/GRJ22; CyanogenMod-7) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
	    "User-Agent:Mozilla/5.0 (Windows NT 6.1; rv:2.0.1) Gecko/20100101 Firefox/4.0.1",
	    "User-Agent: Mozilla/5.0 (Androdi; Linux armv7l; rv:5.0) Gecko/ Firefox/5.0 fennec/5.0",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; The World)",
	    "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Maxthon 2.0)",
	    "User-Agent:Opera/9.80 (Windows NT 6.1; U; en) Presto/2.8.131 Version/11.11",
	    "User-Agent: Opera/9.80 (Android 2.3.4; Linux; Opera mobi/adr-1107051709; U; zh-cn) Presto/2.8.149 Version/11.10",
	    "UCWEB7.0.2.37/28/999",
	    "NOKIA5700/ UCWEB7.0.2.37/28/999",
	    "Openwave/ UCWEB7.0.2.37/28/999",
	    "Mozilla/4.0 (compatible; MSIE 6.0; ) Opera/UCWEB7.0.2.37/28/999"
	};
	
	
	
	const std::string HttpClientRequest::CRLF = "\r\n";
	const std::string HttpClientRequest::ACCEPT_HTML = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8";
	const std::string HttpClientRequest::ACCEPT_JSON = "Accept: application/json, text/javascript, */*; q=0.01";
	const std::string HttpClientRequest::ACCEPT_IMAGE = "Accept: image/webp,image/apng,image/*,*/*;q=0.8";

    void HttpClientRequest::MakeReferer () {
        if (!lastlocate.empty()) {
            referer = lastlocate;
        }
    }

    void HttpClientRequest::MakeGmtDate () {
        char buf[50] = {0};
        time_t tm;
        time(&tm);

        struct tm *tminfo = gmtime(const_cast<time_t *>(&tm));
        strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tminfo);
        date = buf;
    }

    std::string HttpClientRequest::MakeHeader () {
        std::string header = "GET ";
        header += (locate + const_cast<std::string &>(CRLF));
        header += "Host: ";
        header += (host + const_cast<std::string &>(CRLF));

        header += "Date: ";
        MakeGmtDate();
        header += (date + const_cast<std::string &>(CRLF));
        
        header += (connection + const_cast<std::string &>(CRLF));
        header += "Accept: ";
        header += (accept + const_cast<std::string &>(CRLF));
        header += (accept_chardet + const_cast<std::string &>(CRLF));
        header += (accept_encoding + const_cast<std::string &>(CRLF));
        header += (accept_language + const_cast<std::string &>(CRLF));
        header += (accept_range + const_cast<std::string &>(CRLF));

        header += "Referer: ";
        header += (referer + const_cast<std::string &>(CRLF));
        if (!cookie.empty()) {
            header += "Cookie: ";
            header += (cookie + const_cast<std::string &>(CRLF));
        }

        header += "User-Agent: ";
        header += (set_user_agent() + const_cast<std::string &>(CRLF));
        
        header += (cache_control + const_cast<std::string &>(CRLF));
        if (!etag.empty()) {
            header += (if_none_match + etag + const_cast<std::string &>(CRLF));
        }
        if (!last_modified.empty()) {
            header += (if_modified_since + last_modified + const_cast<std::string &>(CRLF));
        }

        header += (upgrade_insecure_requests + const_cast<std::string &>(CRLF));
        if (!x_requested_with) {
            header += "x-requested-with: XMLHttpRequest\r\n";
        }

        return header;
    }

    std::string HttpClientRequest::MakeGet () {
        std::string req = MakeHeader();

        for (auto pair: query_args) {
            req += pair.first;
            req += "=";
            req += pair.second;
            req += "&";
        }
        return req.substr(0, req.length() - 1);
    }

    std::string HttpClientRequest::MakePost () {
        std::string req = MakeHeader();
        return req;
    }
};
