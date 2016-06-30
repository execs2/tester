#include "curl/curl.h"
#include "global.h"
#include "tester.h"

void show_usage()
{
	fprintf(stderr, "Usage:\ntester -t 1 -h http://127.0.0.1 -c 2 -p D:\\1.txt \n");
}

void main(int argc, const char* argv[])
{
	if (argc < 8)
	{
		show_usage();
		return;
	}

	int thread_cnt = 0;
	int post_cnt = 0;
	const char* host = nullptr;
	
	Global* g = Global::get_instance();
	for (int i = 1; i < argc; ++i)
	{
		if (i % 2 == 0 || i + 1 >= argc) continue;

		if (strncmp(argv[i], "-t", 2) == 0)
		{
			thread_cnt = atoi(argv[i + 1]);
			continue;
		}

		if (strncmp(argv[i], "-h", 2) == 0)
		{
			host = argv[i + 1];
			continue;
		}

		if (strncmp(argv[i], "-c", 2) == 0)
		{
			post_cnt = atoi(argv[i + 1]);
			continue;
		}
	}

	if (post_cnt < 0) exit(-1);
	g->set_data_cnt(post_cnt);
	g->set_host(host);

	if (post_cnt > 0)
	{
		int pos = argc - 1 - post_cnt;
		if (strncmp(argv[pos], "-p", 2) != 0)
		{
			show_usage();
			exit(-1);
		}

		int read_cnt = 0;
		while (read_cnt < post_cnt)
		{
			++read_cnt;
			if (!g->read_file(argv[pos + read_cnt]))
			{
				fprintf(stderr, "open %s error. \n", argv[pos + post_cnt]);
				exit(-1);
			}
		}
	}

	if (thread_cnt <= 0 || !g->init_global_env())
	{
		exit(-1);
	}
	
	for (int i = 0; i < thread_cnt; ++i)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Tester::thread_proc, NULL, 0, NULL);
	}

	fprintf(stderr, "Press any key to stop...\n");
	getchar();
	getchar();

	g->set_terminate();
	while (g->get_exit_thread_cnt() != thread_cnt)
	{
		fprintf(stderr, "Stoping, please wait...\n");
		Sleep(1000);
	}

	g->log_result();
	fprintf(stderr, "Press any key to exit...\n");
	getchar();
	return;
}
