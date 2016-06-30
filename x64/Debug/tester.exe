#pragma once

class Tester
{
public:
	Tester();
	virtual ~Tester();

	void init_header();

	void perform();

	void on_thread_end();

	static DWORD thread_proc(LPVOID lpdwThreadParam);

private:
	struct curl_slist* _headers;
	int _success_cnt;
	int _fail_cnt;
	int _total_cnt;
};
