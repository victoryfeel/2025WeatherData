# 项目框架及工具库 (Libs) API 接口文档

本文档汇总并导出了项目框架基础库及工具库（包括 `public/_public.h`、`public/_ftp.h`、`public/db/oracle/_ooci.h`、`public/ftplib.h`、`tools/cpp/_tools.h` 以及 `idc/cpp/idcapp.h`）中声明的所有命名空间、类、结构体、宏定义和全局 API 函数。

---

## 目录

1. [公共基础工具库 (`_public.h`)](#1-公共基础工具库-_publich)
   - [1.1 字符串处理函数](#11-字符串处理函数)
   - [1.2 字符串拆分类 (`ccmdstr`)](#12-字符串拆分类-ccmdstr)
   - [1.3 XML 格式字符串解析函数族](#13-xml-格式字符串解析函数族)
   - [1.4 格式化输出模板函数](#14-格式化输出模板函数)
   - [1.5 时间操作函数与计时器](#15-时间操作函数与计时器)
   - [1.6 目录与文件操作](#16-目录与文件操作)
   - [1.7 进程同步与日志类](#17-进程同步与日志类)
   - [1.8 循环队列模板类 (`squeue`)](#18-循环队列模板类-squeue)
   - [1.9 信号量与进程心跳保活](#19-信号量与进程心跳保活)
   - [1.10 Socket 网络通讯库](#110-socket-网络通讯库)
2. [FTP 客户端 C++ 封装库 (`_ftp.h`)](#2-ftp-客户端-c-封装库-_ftph)
   - [2.1 `cftpclient` 类成员与方法说明](#21-cftpclient-类成员与方法说明)
3. [Oracle 数据库 OCI 接口封装库 (`_ooci.h`)](#3-oracle-数据库-oci-接口封装库-_oocih)
   - [3.1 OCI 结构体与底层函数](#31-oci-结构体与底层函数)
   - [3.2 数据库连接类 (`connection`)](#32-数据库连接类-connection)
   - [3.3 SQL 语句操作类 (`sqlstatement`)](#33-sql-语句操作类-sqlstatement)
4. [数据库表结构工具库 (`_tools.h`)](#4-数据库表结构工具库-_toolsh)
   - [4.1 列信息结构体 (`TableColumn::st_column`)](#41-列信息结构体-tablecolumnst_column)
   - [4.2 表列与主键获取类 (`TableColumn`)](#42-表列与主键获取类-tablecolumn)
5. [IDC 业务应用接口库 (`idcapp.h`)](#5-idc-业务应用接口库-idcapph)
   - [5.1 气象观测数据结构体 (`CZHOBTMIND::st_zhobtmind`)](#51-气象观测数据结构体-czhobtmindst_zhobtmind)
   - [5.2 全国气象分钟数据入库类 (`CZHOBTMIND`)](#52-全国气象分钟数据入库类-czhobtmind)
6. [C 语言开源 FTP 基础库 (`ftplib.h`)](#6-c-语言开源-ftp-基础库-ftplibh)
   - [6.1 核心数据结构与宏](#61-核心数据结构与宏)
   - [6.2 C API 函数列表](#62-c-api-函数列表)

---

## 1. 公共基础工具库 (`_public.h`)

命名空间: `idc`

### 1.1 字符串处理函数

| 函数签名                                                                                                   | 功能描述                                                   | 参数说明                                                             |
| :--------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------- | :------------------------------------------------------------------- |
| `char* deletelchr(char* str, const int cc = ' ')`                                                          | 删除 C 风格字符串左边指定的字符                            | `str`: 待处理字符串；`cc`: 需删除的字符（缺省空格）                  |
| `string& deletelchr(string& str, const int cc = ' ')`                                                      | 删除 C++ string 左边指定的字符                             | 同上                                                                 |
| `char* deleterchr(char* str, const int cc = ' ')`                                                          | 删除 C 风格字符串右边指定的字符                            | 同上                                                                 |
| `string& deleterchr(string& str, const int cc = ' ')`                                                      | 删除 C++ string 右边指定的字符                             | 同上                                                                 |
| `char* deletelrchr(char* str, const int cc = ' ')`                                                         | 删除 C 风格字符串左右两边指定的字符                        | 同上                                                                 |
| `string& deletelrchr(string& str, const int cc = ' ')`                                                     | 删除 C++ string 左右两边指定的字符                         | 同上                                                                 |
| `char* toupper(char* str)`                                                                                 | 把字符串中的小写字母转换成大写                             | `str`: 待转换字符串                                                  |
| `string& toupper(string& str)`                                                                             | 把字符串中的小写字母转换成大写                             | 同上                                                                 |
| `char* tolower(char* str)`                                                                                 | 把字符串中的大写字母转换成小写                             | `str`: 待转换字符串                                                  |
| `string& tolower(string& str)`                                                                             | 把字符串中的大写字母转换成小写                             | 同上                                                                 |
| `bool replacestr(char* str, const string& str1, const string& str2, const bool bloop = false)`             | 字符串替换函数                                             | `str`: 原串；`str1`: 旧内容；`str2`: 新内容；`bloop`: 是否循环替换   |
| `bool replacestr(string& str, const string& str1, const string& str2, const bool bloop = false)`           | 字符串替换函数（C++ 风格）                                 | 同上                                                                 |
| `char* picknumber(const string& src, char* dest, const bool bsigned = false, const bool bdot = false)`     | 提取数字、正负号和小数点                                   | `src`: 源串；`dest`: 目标串；`bsigned`: 提取符号；`bdot`: 提取小数点 |
| `string& picknumber(const string& src, string& dest, const bool bsigned = false, const bool bdot = false)` | 提取数字、正负号和小数点                                   | 同上                                                                 |
| `string picknumber(const string& src, const bool bsigned = false, const bool bdot = false)`                | 提取数字并返回新字符串                                     | 同上                                                                 |
| `bool matchstr(const string& str, const string& rules)`                                                    | 正则通配符匹配（忽略大小写，`rules` 支持 `*`，以逗号分隔） | `str`: 精确文件名；`rules`: 规则串（如 `"*.h,*.cpp"`）               |

---

### 1.2 字符串拆分类 (`ccmdstr`)

用于拆分带分隔符的字符串（如 CSV 或自定义分隔符格式）。

#### 方法列表

```cpp
class ccmdstr {
public:
  ccmdstr();
  ccmdstr(const string& buffer, const string& sepstr, const bool bdelspace = false);

  // 把字符串拆分到 m_cmdstr 容器中
  void splittocmd(const string& buffer, const string& sepstr, const bool bdelspace = false);

  // 获取拆分后字段个数
  int size() const;
  int cmdcount() const;

  // 下标访问操作符
  const string& operator[](int ii) const;

  // 获取指定索引处的字段内容，自动转换为对应的类型
  bool getvalue(const int ii, string& value, const int ilen = 0) const;
  bool getvalue(const int ii, char* value, const int ilen = 0) const;
  bool getvalue(const int ii, int& value) const;
  bool getvalue(const int ii, unsigned int& value) const;
  bool getvalue(const int ii, long& value) const;
  bool getvalue(const int ii, unsigned long& value) const;
  bool getvalue(const int ii, double& value) const;
  bool getvalue(const int ii, float& value) const;
  bool getvalue(const int ii, bool& value) const;

  ~ccmdstr();
};

// 输出流重载（调试用）
ostream& operator<<(ostream& out, const ccmdstr& cc);
```

---

### 1.3 XML 格式字符串解析函数族

解析形如 `<filename>/tmp/_public.h</filename><size>18348</size>` 的 XML 字符串。

```cpp
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, string& value, const int ilen = 0);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, char* value, const int ilen = 0);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, bool& value);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, int& value);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, unsigned int& value);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, long& value);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, unsigned long& value);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, double& value);
bool getxmlbuffer(const string& xmlbuffer, const string& fieldname, float& value);
```

---

### 1.4 格式化输出模板函数

```cpp
template <typename... Args>
bool sformat(string& str, const char* fmt, Args... args);

template <typename... Args>
string sformat(const char* fmt, Args... args);
```

---

### 1.5 时间操作函数与计时器

#### 时间转换函数

- `string& ltime(string& strtime, const string& fmt = "", const int timetvl = 0)`：获取系统时间串。`fmt` 格式支持 `"yyyy-mm-dd hh24:mi:ss"`、`"yyyymmddhh24miss"` 等；`timetvl` 为时间秒数偏移量。
- `char* ltime(char* strtime, const string& fmt = "", const int timetvl = 0)`
- `string ltime1(const string& fmt = "", const int timetvl = 0)`
- `string& timetostr(const time_t ttime, string& strtime, const string& fmt = "")`：将 `time_t` 整数时间转为格式化时间串。
- `char* timetostr(const time_t ttime, char* strtime, const string& fmt = "")`
- `string timetostr1(const time_t ttime, const string& fmt = "")`
- `time_t strtotime(const string& strtime)`：把字符串表示的时间转换为 `time_t` 整数时间。
- `bool addtime(const string& in_stime, char* out_stime, const int timetvl, const string& fmt = "")`：时间串偏移运算。
- `bool addtime(const string& in_stime, string& out_stime, const int timetvl, const string& fmt = "")`

#### 计时器类 (`ctimer`)

```cpp
class ctimer {
public:
  ctimer();        // 构造函数，自动开始计时
  void start();    // 开始/重新开始计时
  double elapsed();// 获取已逝去的时间（单位：秒，精确到微秒），并重新开始计时
};
```

---

### 1.6 目录与文件操作

#### 文件与目录独立函数

- `bool newdir(const string& pathorfilename, bool bisfilename = true)`：逐级递归创建目录。
- `bool renamefile(const string& srcfilename, const string& dstfilename)`：重命名/移动文件（自动创建目标目录）。
- `bool copyfile(const string& srcfilename, const string& dstfilename)`：复制文件（使用临时文件写入并保持原时间属性）。
- `int filesize(const string& filename)`：获取文件大小（字节）。
- `bool filemtime(const string& filename, char* mtime, const string& fmt = "yyyymmddhh24miss")`：获取文件修改时间。
- `bool filemtime(const string& filename, string& mtime, const string& fmt = "yyyymmddhh24miss")`
- `bool setmtime(const string& filename, const string& mtime)`：重置文件的修改时间属性。

#### 目录遍历类 (`cdir`)

```cpp
class cdir {
public:
  string m_dirname;   // 目录名
  string m_filename;  // 文件名（不含目录）
  string m_ffilename; // 文件绝对路径
  int m_filesize;     // 文件大小（字节）
  string m_mtime;     // 修改时间
  string m_ctime;     // 创建时间
  string m_atime;     // 访问时间

  cdir();
  void setfmt(const string& fmt); // 设置时间格式
  bool opendir(const string& dirname, const string& rules, const int maxfiles = 10000, const bool bandchild = false, bool bsort = false);
  bool readdir();                 // 循环获取下一条文件信息
  unsigned int size();            // 获取匹配的文件总数
  ~cdir();
};
```

#### 写文件类 (`cofile`)

```cpp
class cofile {
public:
  cofile();
  bool isopen() const;
  bool open(const string& filename, const bool btmp = true, const ios::openmode mode = ios::out, const bool benbuffer = true);

  template <typename... Args>
  bool writeline(const char* fmt, Args... args);

  template <typename T>
  cofile& operator<<(const T& value);

  bool write(void* buf, int bufsize);
  bool closeandrename(); // 关闭并将临时文件改名为正式文件名
  void close();          // 关闭（若存在临时文件则删除）
  ~cofile();
};
```

#### 读文件类 (`cifile`)

```cpp
class cifile {
public:
  cifile();
  bool isopen() const;
  bool open(const string& filename, const ios::openmode mode = ios::in);
  bool readline(string& buf, const string& endbz = "");
  int read(void* buf, const int bufsize);
  bool closeandremove(); // 关闭并删除文件
  void close();
  ~cifile();
};
```

---

### 1.7 进程同步与日志类

#### 自旋锁 (`spinlock_mutex`)

```cpp
class spinlock_mutex {
public:
  spinlock_mutex();
  void lock();  // 加锁
  void unlock();// 解锁
};
```

#### 日志操作类 (`clogfile`)

```cpp
class clogfile {
public:
  clogfile(int maxsize = 100); // 日志自动切分最大体积 (MB)
  bool open(const string& filename, const ios::openmode mode = ios::app, const bool bbackup = true, const bool benbuffer = false);

  template <typename... Args>
  bool write(const char* fmt, Args... args); // 自动带时间戳的写日志方法

  template <typename T>
  clogfile& operator<<(const T& value);      // 不带时间戳直接输出

  void close();
  ~clogfile();
};
```

---

### 1.8 循环队列模板类 (`squeue`)

固定大小的循环队列，可直接用于共享内存。

```cpp
template <class TT, int MaxLength>
class squeue {
public:
  squeue();
  void init();             // 初始化队列
  bool push(const TT& ee); // 入队
  int size();              // 队列当前大小
  bool empty();            // 是否为空
  bool full();             // 是否已满
  TT& front();             // 查看队头
  bool pop();              // 出队
  void printqueue();       // 打印队列（调试）
};
```

---

### 1.9 信号量与进程心跳保活

#### System V 信号量封装类 (`csemp`)

```cpp
class csemp {
public:
  csemp();
  bool init(key_t key, unsigned short value = 1, short sem_flg = SEM_UNDO);
  bool wait(short value = -1); // P 操作
  bool post(short value = 1);  // V 操作
  int getvalue();              // 获取信号量值
  bool destroy();              // 销毁信号量
  ~csemp();
};
```

#### 进程心跳数据结构与保活类 (`cpactive`)

```cpp
struct st_procinfo {
  int pid;            // 进程 ID
  char pname[51];     // 进程名称
  int timeout;        // 超时检测阈值 (秒)
  time_t atime;       // 最后一次心跳时间戳
};

#define MAXNUMP 1000   // 最大监控进程数
#define SHMKEYP 0x5095 // 共享内存 Key
#define SEMKEYP 0x5095 // 信号量 Key

class cpactive {
public:
  cpactive();
  bool addpinfo(const int timeout, const string& pname = "", clogfile* logfile = nullptr); // 将进程注册到共享内存
  bool uptatime(); // 更新心跳时间戳
  ~cpactive();     // 退出时自动注销记录
};
```

#### 系统环境清理

- `void closeioandsignal(bool bcloseio = false)`：忽略信号并选择性关闭全盘 IO 文件描述符。

---

### 1.10 Socket 网络通讯库

#### TCP 客户端类 (`ctcpclient`)

```cpp
class ctcpclient {
public:
  ctcpclient();
  bool connect(const string& ip, const int port);
  bool read(string& buffer, const int itimeout = 0);
  bool read(void* buffer, const int ibuflen, const int itimeout = 0);
  bool write(const string& buffer);
  bool write(const void* buffer, const int ibuflen);
  void close();
  ~ctcpclient();
};
```

#### TCP 服务端类 (`ctcpserver`)

```cpp
class ctcpserver {
public:
  ctcpserver();
  bool initserver(const unsigned int port, const int backlog = 5);
  bool accept();
  char* getip();
  bool read(string& buffer, const int itimeout = 0);
  bool read(void* buffer, const int ibuflen, const int itimeout = 0);
  bool write(const string& buffer);
  bool write(const void* buffer, const int ibuflen);
  void closelisten(); // 关闭监听 socket
  void closeclient(); // 关闭客户端 socket
  ~ctcpserver();
};
```

#### 通用 Socket 传输函数

```cpp
bool tcpread(const int sockfd, string& buffer, const int itimeout = 0);
bool tcpread(const int sockfd, void* buffer, const int ibuflen, const int itimeout = 0);
bool tcpwrite(const int sockfd, const string& buffer);
bool tcpwrite(const int sockfd, const void* buffer, const int ibuflen);
bool readn(const int sockfd, char* buffer, const size_t n);
bool writen(const int sockfd, const char* buffer, const size_t n);
```

---

## 2. FTP 客户端 C++ 封装库 (`_ftp.h`)

头文件位置: `public/_ftp.h`  
命名空间: `idc`

### 2.1 `cftpclient` 类成员与方法说明

```cpp
class cftpclient {
public:
  unsigned int m_size;  // 远程文件大小（字节）
  string m_mtime;       // 远程文件最后修改时间 (格式: yyyymmddhh24miss)

  // 状态与错误原因成员变量
  bool m_connectfailed; // 网络连接失败标志（true 表示连接建立失败）
  bool m_loginfailed;   // 登录失败标志（true 表示用户名、密码错误或无权限）
  bool m_optionfailed;  // 设置传输模式失败标志（true 表示主动/被动模式切换失败）

  cftpclient();         // 构造函数，初始化成员变量并调用 FtpInit()
  ~cftpclient();        // 析构函数，自动调用 logout() 释放资源

  cftpclient(const cftpclient&) = delete;            // 禁用拷贝构造函数
  cftpclient& operator=(const cftpclient) = delete; // 禁用赋值函数

  void initdata();      // 初始化/重置 m_size 和 m_mtime 成员变量

  // 登录 FTP 服务器
  // host: 服务器 IP 地址和端口，如 "192.168.1.1:21"
  // username: 登录用户名
  // password: 登录密码
  // imode: 传输模式，1-FTPLIB_PASSIVE 被动模式 (缺省)，2-FTPLIB_PORT 主动模式
  // 返回值: true-成功，false-失败
  bool login(const string& host, const string& username, const string& password, const int imode = FTPLIB_PASSIVE);

  // 注销登录并断开连接
  // 返回值: true-成功，false-未连接
  bool logout();

  // 获取远程 FTP 服务器上指定文件的时间，保存于 m_mtime
  bool mtime(const string& remotefilename);

  // 获取远程 FTP 服务器上指定文件的大小，保存于 m_size
  bool size(const string& remotefilename);

  // 改变远程 FTP 服务器的当前工作目录
  bool chdir(const string& remotedir);

  // 在远程 FTP 服务器上创建目录
  bool mkdir(const string& remotedir);

  // 删除远程 FTP 服务器上的目录
  bool rmdir(const string& remotedir);

  // 列出远程 FTP 服务器指定目录中的文件和子目录列表，结果写入本地文件 listfilename
  bool nlist(const string& remotedir, const string& listfilename);

  // 从 FTP 服务器获取/下载文件
  // remotefilename: 远程待获取的文件名
  // localfilename: 保存到本地的文件名
  // bcheckmtime: 是否核对传输前后远程文件修改时间，保证完整性
  // 说明: 传输过程中采用临时文件名 (localfilename + ".tmp")，完成后更名
  bool get(const string& remotefilename, const string& localfilename, const bool bcheckmtime = true);

  // 向 FTP 服务器发送/上传文件
  // localfilename: 本地待发送的文件名
  // remotefilename: 发送到远程的文件名
  // bchecksize: 是否核对本地与远程文件大小，保证完整性
  // 说明: 传输过程中采用临时文件名 (remotefilename + ".tmp")，完成后更名
  bool put(const string& localfilename, const string& remotefilename, const bool bchecksize = true);

  // 删除远程 FTP 服务器上的文件
  bool ftpdelete(const string& remotefilename);

  // 重命名远程 FTP 服务器上的文件
  bool ftprename(const string& srcremotefilename, const string& dstremotefilename);

  // 向 FTP 服务器发送自定义 SITE 命令
  bool site(const string& command);

  // 获取服务器返回信息的最后一条文本指针
  char* response();
};
```

---

## 3. Oracle 数据库 OCI 接口封装库 (`_ooci.h`)

头文件位置: `public/db/oracle/_ooci.h`  
命名空间: `idc`

### 3.1 OCI 结构体与底层函数

#### 数据结构

```cpp
struct LOGINENV {
  char user[31];    // 用户名
  char pass[31];    // 密码
  char tnsname[51]; // tnsname (配于 tnsnames.ora)
  OCIEnv* envhp;    // OCI 环境变量句柄
};

struct OCI_CXT {
  OCISvcCtx* svchp; // 服务上下文句柄
  OCIError* errhp;  // 错误句柄
  OCIEnv* envhp;    // 环境变量句柄
};

struct OCI_HANDLE {
  OCISvcCtx* svchp; // 服务器上下文句柄引用
  OCIStmt* smthp;   // SQL 语句句柄
  OCIBind* bindhp;  // 绑定输入句柄
  OCIDefine* defhp; // 绑定输出句柄
  OCIError* errhp;  // 错误句柄
  OCIEnv* envhp;    // 环境变量句柄
};

struct CDA_DEF {
  int rc;             // 返回码 (0-成功)
  unsigned long rpc;  // 影响记录的行数或结果集行数
  char message[2048]; // SQL 执行失败时的错误描述
};
```

#### 底层 C 函数

```cpp
int oci_init(LOGINENV* env);
int oci_close(LOGINENV* env);
int oci_context_create(LOGINENV* env, OCI_CXT* cxt);
int oci_context_close(OCI_CXT* cxt);
int oci_stmt_create(OCI_CXT* cxt, OCI_HANDLE* handle);
int oci_stmt_close(OCI_HANDLE* handle);
```

---

### 3.2 数据库连接类 (`connection`)

```cpp
class connection {
public:
  connection();
  ~connection();

  // 登录并连接数据库 (connstr: username/password@tnsname)
  int connecttodb(const string& connstr, const string& charset, bool autocommitopt = false);
  bool isopen();
  int commit();
  int rollback();
  int disconnect();

  // 直接执行无绑定变量的 SQL 语句 (可变参数)
  int execute(const char* fmt, ...);

  int rc();           // 获取错误代码
  unsigned long rpc();// 获取影响行数
  const char* message(); // 获取错误描述
};
```

---

### 3.3 SQL 语句操作类 (`sqlstatement`)

```cpp
class sqlstatement {
public:
  sqlstatement();
  sqlstatement(connection* conn);
  ~sqlstatement();

  int connect(connection* conn);
  bool isopen();
  int disconnect();

  int prepare(const string& strsql);
  int prepare(const char* fmt, ...);

  // 绑定输入变量
  int bindin(const unsigned int position, int& value);
  int bindin(const unsigned int position, long& value);
  int bindin(const unsigned int position, unsigned int& value);
  int bindin(const unsigned int position, unsigned long& value);
  int bindin(const unsigned int position, float& value);
  int bindin(const unsigned int position, double& value);
  int bindin(const unsigned int position, char* value, unsigned int len = 2000);
  int bindin(const unsigned int position, string& value, unsigned int len = 2000);
  int bindin1(const unsigned int position, string& value);

  // 绑定输出变量
  int bindout(const unsigned int position, int& value);
  int bindout(const unsigned int position, long& value);
  int bindout(const unsigned int position, unsigned int& value);
  int bindout(const unsigned int position, unsigned long& value);
  int bindout(const unsigned int position, float& value);
  int bindout(const unsigned int position, double& value);
  int bindout(const unsigned int position, char* value, unsigned int len = 2000);
  int bindout(const unsigned int position, string& value, unsigned int len = 2000);

  int execute();
  int execute(const char* fmt, ...);
  int next(); // 提取下一条结果 (0-成功, 1403-无记录)

  int bindblob();
  int bindclob();
  int filetolob(const string& filename);
  int lobtofile(const string& filename);

  const char* sql();
  int rc();
  unsigned long rpc();
  const char* message();
};
```

---

## 4. 数据库表结构工具库 (`_tools.h`)

头文件位置: `tools/cpp/_tools.h`  
命名空间: 无 (全局) / 使用 `using namespace idc;`

### 4.1 列信息结构体 (`TableColumn::st_column`)

用于存放从数据库数据字典中查询到的列或主键属性。

```cpp
struct st_column {
  char column_name[31];       // 列名/字段名
  char data_type[31];         // 数据类型 (如 NUMBER, VARCHAR2, DATE 等)
  int column_length;          // 列长度 (字节数)
  int primary_key_sequence;   // 主键序号 (若不是主键列则为 0)
};
```

### 4.2 表列与主键获取类 (`TableColumn`)

自动查询 Oracle 数据库数据字典 (`USER_TAB_COLUMNS`, `USER_CONS_COLUMNS`, `USER_CONSTRAINTS`) 并解析表字段结构。

```cpp
class TableColumn {
public:
  vector<struct st_column> m_column_info;       // 存放表中所有列信息的容器
  vector<struct st_column> m_primary_key_info;  // 存放表主键列信息的容器
  string m_all_columns;                         // 格式化的所有列名串 (以逗号分隔)
  string m_all_primary_key;                     // 格式化的所有主键列名串 (以逗号分隔)

  TableColumn();

  // 清空并初始化 m_column_info, m_primary_key_info, m_all_columns 和 m_all_primary_key
  void init_data();

  // 从 Oracle 数据库字典获取指定表名的所有列信息
  // conn: 已连接的 connection 对象引用
  // table_name: 表名 (大写)
  // 返回值: true-成功，false-失败
  bool get_column_info(connection& conn, char* table_name);

  // 从 Oracle 数据库字典获取指定表名的主键列信息
  // conn: 已连接的 connection 对象引用
  // table_name: 表名 (大写)
  // 返回值: true-成功，false-失败
  bool get_primary_key_info(connection& conn, char* table_name);
};
```

---

## 5. IDC 业务应用接口库 (`idcapp.h`)

头文件位置: `idc/cpp/idcapp.h`  
包含依赖: `_ooci.h`, `_public.h` (使用 `using namespace idc;`)

### 5.1 气象观测数据结构体 (`CZHOBTMIND::st_zhobtmind`)

```cpp
struct st_zhobtmind {
  char obtid[6];      // 站点代码 (如 "58362")
  char ddatetime[21]; // 数据时间，精确到分钟 (格式 "yyyy-mm-dd hh24:mi:ss")
  char t[11];         // 温度，单位：0.1 摄氏度
  char p[11];         // 气压，单位：0.1 百帕
  char u[11];         // 相对湿度，0-100
  char wd[11];        // 风向，0-360
  char wf[11];        // 风速，单位：0.1 m/s
  char r[11];         // 降雨量，单位：0.1 mm
  char vis[11];       // 能见度，单位：0.1 米
};
```

### 5.2 全国气象分钟数据入库类 (`CZHOBTMIND`)

负责将解压抽取的气象分钟数据文件拆分解析并批量/单条高效写入数据库表 `T_ZHOBTMIND` 中。

```cpp
class CZHOBTMIND {
private:
  clogfile& m_logfile;               // 日志对象引用
  connection& m_conn;                // 数据库连接引用
  sqlstatement m_stmt;               // 数据库 SQL 绑定语句
  string m_buffer;                   // 数据缓冲区
  struct st_zhobtmind m_stzhobtmind; // 单条解析后的结构体数据

public:
  CZHOBTMIND(connection& conn, clogfile& logfile);
  ~CZHOBTMIND();

  // 拆分字符串/XML 记录，并将提取出的属性填充到成员结构体 m_stzhobtmind 中
  // strbuffer: 一条气象数据记录报文
  // bisxml: 是否为 XML 格式报文 (true-XML, false-逗号分隔符串)
  // 返回值: true-成功，false-失败
  bool splitbuffer(const string& strbuffer, const bool bisxml);

  // 将 m_stzhobtmind 中的气象数据绑定并插入到数据库表中
  // 返回值: true-成功，false-失败
  bool inserttable();
};
```

---

## 6. C 语言开源 FTP 基础库 (`ftplib.h`)

头文件位置: `public/ftplib.h`

### 6.1 核心数据结构与宏

#### 控制宏定义

- 传输类型: `FTPLIB_DIR`, `FTPLIB_DIR_VERBOSE`, `FTPLIB_FILE_READ`, `FTPLIB_FILE_WRITE`
- 传输模式: `FTPLIB_ASCII` / `FTPLIB_TEXT` ('A'), `FTPLIB_IMAGE` / `FTPLIB_BINARY` ('I')
- 连接模式: `FTPLIB_PASSIVE` (1), `FTPLIB_PORT` (2)
- 选项名称: `FTPLIB_CONNMODE`, `FTPLIB_CALLBACK`, `FTPLIB_IDLETIME`, `FTPLIB_CALLBACKARG`, `FTPLIB_CALLBACKBYTES`

#### 数据类型

```c
typedef struct NetBuf netbuf;
typedef int (*FtpCallback)(netbuf *nControl, fsz_t xfered, void *arg);

typedef struct FtpCallbackOptions {
    FtpCallback cbFunc;        /* 回调函数指针 */
    void *cbArg;               /* 回调传递参数 */
    unsigned int bytesXferred; /* 每传输指定字节数触发回调 */
    unsigned int idleTime;     /* 空闲指定毫秒数触发回调 */
} FtpCallbackOptions;
```

---

### 6.2 C API 函数列表

```c
void FtpInit(void);
char *FtpLastResponse(netbuf *nControl);
int FtpConnect(const char *host, netbuf **nControl);
int FtpOptions(int opt, long val, netbuf *nControl);
int FtpSetCallback(const FtpCallbackOptions *opt, netbuf *nControl);
int FtpClearCallback(netbuf *nControl);
int FtpLogin(const char *user, const char *pass, netbuf *nControl);
int FtpAccess(const char *path, int typ, int mode, netbuf *nControl, netbuf **nData);
int FtpRead(void *buf, int max, netbuf *nData);
int FtpWrite(const void *buf, int len, netbuf *nData);
int FtpClose(netbuf *nData);
int FtpSite(const char *cmd, netbuf *nControl);
int FtpSysType(char *buf, int max, netbuf *nControl);
int FtpMkdir(const char *path, netbuf *nControl);
int FtpChdir(const char *path, netbuf *nControl);
int FtpCDUp(netbuf *nControl);
int FtpRmdir(const char *path, netbuf *nControl);
int FtpPwd(char *path, int max, netbuf *nControl);
int FtpNlst(const char *output, const char *path, netbuf *nControl);
int FtpDir(const char *output, const char *path, netbuf *nControl);
int FtpSize(const char *path, unsigned int *size, char mode, netbuf *nControl);
int FtpSizeLong(const char *path, fsz_t *size, char mode, netbuf *nControl);
int FtpModDate(const char *path, char *dt, int max, netbuf *nControl);
int FtpGet(const char *output, const char *path, char mode, netbuf *nControl);
int FtpPut(const char *input, const char *path, char mode, netbuf *nControl);
int FtpRename(const char *src, const char *dst, netbuf *nControl);
int FtpDelete(const char *fnm, netbuf *nControl);
void FtpQuit(netbuf *nControl);
```
