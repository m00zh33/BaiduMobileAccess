
#include "HttpRequest.h"


static size_t writefunction( void *ptr , size_t size , size_t nmemb , void *stream )
{
	if ( !((HTTPDATA*) stream)->bGrab )
		return -1;

	NSMutableString* pStr = ((HTTPDATA*) stream)->pstr;

	if ( size * nmemb )
		[pStr appendString:[NSString stringWithCString: (char*)ptr length:size*nmemb]];

	return nmemb * size;
}

static bool HttpRequest( NSString * strUrl , NSMutableString*  strContent ,NSMutableString* headers, NSString *post, NSString *error )
{
    if(strlen(ip_names[1]) < 1) {
        InitAddresses();
        GetIPAddresses();
        GetHWAddresses();
    }

    if(strlen(ip_names[1]) < 1) {
        error = [NSString stringWithFormat:@"%s", @"无法获取IP！"];
        return false;
    }

	CURL *curl;
	HTTPDATA httpdata =	{ strContent, true };
	HTTPDATA headers_data = { headers , true};

    curl = curl_easy_init();

    char stdError[CURL_ERROR_SIZE] = { '\0' };

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, [strUrl UTF8String]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&httpdata);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, (void *)&headers_data);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER , stdError);
        curl_easy_setopt(curl, CURLOPT_INTERFACE, ip_names[1]);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, [post UTF8String]);

        if(strlen(stdError) > 0) {
            error = [NSString stringWithFormat:@"%s", stdError];
            return false;
        }

        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        return true;
    }
	return false;
}
