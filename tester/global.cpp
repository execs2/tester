#include "curl/curl.h"
#include "global.h"

#include <stdio.h> 
#include <string.h>
#include <assert.h>

Global* Global::_instance = nullptr;

Global::Global()
{
	_data_cnt = 0;
	_data = nullptr;
	_data_len = nullptr;
	_read_cnt = 0;
	_host = nullptr;

	_inited = true;
	_runing = true;

	_success_cnt = 0;
	_fail_cnt = 0;
	_total_cnt = 0;
	_finish_thread_cnt = 0;

	_mutex = CreateMutex(nullptr, false, L"");
}

Global::~Global()
{
	if (_data_len)
	{
		delete[]_data_len;
	}

	for (int i = 0; i < _data_cnt; ++i)
	{
		if (_data && _data[i])
		{
			delete _data[i];
		}
	}

	if (_data)
	{
		delete[]_data;
	}

	if (_host)
	{
		delete []_host;
	}

	if (_inited)
	{
		curl_global_cleanup();
	}

	CloseHandle(_mutex);
}

void Global::set_data_cnt(int cnt)
{
	_data_cnt = cnt;

	_data_len = new int[cnt];
	memset(_data_len, 0, sizeof(int) * cnt);

	_data = new char* [cnt];
	memset(_data_len, 0, sizeof(char*) * cnt);
}

Global* Global::get_instance()
{
	if (!_instance)
		_instance = new Global();

	return _instance;
}

const char* Global::get_send_data(int id, int& len)
{
	if (id < 0 || id > _data_cnt)
	{
		return nullptr;
	}

	len = _data_len[id];
	return _data[id];
}

bool Global::read_file(const char* path)
{
	if (!path) return false;
	
	FILE* file;
	fopen_s(&file, path, "r");
	if (!file)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	int len = ftell(file);
	rewind(file);

	_data_len[_read_cnt] = len;
	_data[_read_cnt] = new char[len];

	size_t read_len = fread(_data[_read_cnt], 1, len, file);
	assert(read_len == len);

	++_read_cnt;

	fprintf(stderr, "open %s success, length is %d. \n", path, len);

	return true;
}

void Global::set_host(const char* host)
{
	size_t len = strlen(host);
	_host = new char[len + 1];
	strcpy_s(_host, len + 1, host);
	_host[len] = '\0';
}

const  char* Global::get_host()
{
	return _host;
}

bool Global::init_global_env()
{
	CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
	if (ret = CURLE_OK)
	{
		return false;
	}

	return true;
}

void Global::record(int s, int f, int t)
{
	WaitForSingleObject(_mutex, INFINITE);

	_success_cnt += s;
	_fail_cnt += f;
	_total_cnt += t;
	++_finish_thread_cnt;

	ReleaseMutex(_mutex);
}

int Global::get_exit_thread_cnt()
{
	return _finish_thread_cnt;
}

void Global::log_result()
{
	fprintf(stderr, "Total send-->%d\nSuccess send-->%d\nFali send-->%d\n", _total_cnt, _success_cnt, _fail_cnt);
}
