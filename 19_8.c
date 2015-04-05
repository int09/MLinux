#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
CURL *curl;
CURLcode res;
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (strlen((char *) stream) + strlen((char *)ptr) > 999999)
		return 0;
	strcat(stream, (char *)ptr);
	return size * nmemb;
}

char *down_file(char *filename)
{
	static char str[10000000];
	strcpy(str, "");
	curl_easy_setopt(curl, CURLOPT_URL, filename);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);
	res = curl_easy_perform(curl);
	str[9999999] = '0';
	if (CURLE_OK != res)
		return NULL;
	return str;
}

int main()
{
	char url[200];
	curl = curl_easy_init();
	char *result;
	while(fgets(url, 200, stdin))
	{
		result = down_file(url);
		if (result)
			puts(result);
		else
			puts("Get error!");
		printf("/nPlease Input a url: ");
	}
	curl_easy_cleanup(curl);
	return 0;
}

//curl_close()		//关闭一个cURL会话
//curl_copy_handle()	//复制一个cURL句柄和它所有的选项
