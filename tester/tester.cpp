#include "curl/curl.h"
#include "global.h"
#include "tester.h"

#include <time.h>

Tester::Tester()
{
	_success_cnt = 0;
	_fail_cnt = 0;
	_total_cnt = 0;
}

Tester::~Tester()
{
}

void Tester::init_header()
{
	_headers = curl_slist_append(_headers, "Content-Type:application/x-thrift");
	_headers = curl_slist_append(_headers, "Accept:application/x-thrift");
	_headers = curl_slist_append(_headers, "User-Agent:PHP/THttpClient");
}

void Tester::perform()
{
	Global* g = Global::get_instance();
	int cnt = g->get_data_cnt();

	srand((unsigned int)time(0));
	int id = rand() % cnt;

	int len = 0;
	const char* data = g->get_send_data(id, len);

	//补上长度
	char content[64];
	init_header();
	sprintf_s(content, "Content-Length:%d", len);
	_headers = curl_slist_append(_headers, content);

	//这里测试短连接
	CURL* curl = curl_easy_init();

	const char* url = g->get_host();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, _headers);

	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	//curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

	CURLcode ret = curl_easy_perform(curl);
	if (ret == CURLE_OK)
	{
		++_success_cnt;
	}
	else
	{
		++_fail_cnt;
		fprintf(stderr, "Send fail:%s-->%d. \n", curl_easy_strerror(ret), _fail_cnt);
	}
	++_total_cnt;

	curl_slist_free_all(_headers);
	_headers = NULL;
	curl_easy_cleanup(curl);
}

void Tester::on_thread_end()
{
	Global* g = Global::get_instance();
	g->record(_success_cnt, _fail_cnt, _total_cnt);
}

DWORD Tester::thread_proc(LPVOID lpdwThreadParam)
{
	Tester* t = new Tester();
	
	Global* g = Global::get_instance();
	while(g->runing())
	{
		t->perform();
	}

	t->on_thread_end();
	delete t;

	return 0;
}
