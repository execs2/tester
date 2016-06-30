#pragma once

class Global
{
public:
  Global();
  virtual ~Global();

  int get_data_cnt()
  {
    return _data_cnt;
  }

  void set_data_cnt(int cnt);

  static Global* get_instance();

  const char* get_send_data(int id, int& len);

  bool read_file(const char* path);

  void set_host(const char* host);

  const char* get_host();

  bool init_global_env();

  void record(int s, int f, int t);

  int get_exit_thread_cnt();

  void log_result();

  bool runing()
  {
    return _runing;
  }

  void set_terminate()
  {
    _runing = false;
  }

private:
  char** _data;
  int* _data_len;
  char* _host;

  int _data_cnt;
  int _finish_thread_cnt;
  int _read_cnt;

  bool _inited;
  bool _runing;

  HANDLE _mutex;  //只在最后有竞争，简单点，直接互斥锁
  int _success_cnt;
  int _fail_cnt;
  int _total_cnt;

  static Global* _instance;
};
