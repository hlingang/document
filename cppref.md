## cpp header ##

### \<bitset\> ###
1. *bitset\<n\> bitset* **n位的bitset, 每个bit代表true和false
2. *bitset.reset()*
3. *bitset.set(/*position, true*/)*
4. *bitset.flip(/*position*/)*
5. *支持与或非操作*
6. *支持索引访问*
7. *bitset.all()/any()/size()/count()*
8. *bitset.to_string()/to_ulong()*
```
int main() {
    bitset<8> bset;
    bset.reset();
    bset.set(0, true);
    bset.set(1, true);
    cout << "count: " << bset.count() << endl;
    for (int i = 0; i < bset.size(); i++) {
        cout << bset[i] << endl;
    }
    cout << "string: " << bset.to_string() << endl;
    cout << "ulong : " << bset.to_ullong() << endl;
}
```

### \<chrono\> ###
#### duration ###
##### 通用时间长度 ###
1. *std::ratio\<a, b\>* **单位时间刻度(有理数)**
2. *std::chrono::duration\<int, ratio\>* **时间长度**
    1) 刻度单位
    2) 刻度计数
##### 常用的时间长度 #####
3. *std::chrono::minutes*
4. *std::chrono::seconds*
##### 时间长度的转换 #####
5. *std::chrono::duration_cast\<target_duration\>(source_duration)*
6. *std::chrono::system_clock::now()* **获取当前系统时间**
    ```
    int main() {
        chrono::duration<int, ratio<60, 1>> s(1); // 1 mins
        cout << chrono::duration_cast<chrono::seconds>(s).count() << endl;
    }
    ```

#### time_point ####
1. system_clock
2. steady_clock
3. high_resolution_clock
##### system_clock #####
1. *now* 
2. *from_time_t* 
3. *to_time_t* 
##### steady_clock #####
1. *now*
##### high_resolution_clock #####
1. *now*
##### 时间点转换成时间段 #####
1. *time_point.since_time_epoch()*
```
int main() {
    chrono::time_point ts_point = chrono::system_clock::now();
    chrono::nanoseconds ts_duration_ns = ts_point.time_since_epoch();
    chrono::milliseconds ts_duration_ms =
        chrono::duration_cast<chrono::milliseconds>(ts_duration_ns);
    uint64_t ms_count = ts_duration_ms.count();
    cout << "ms_count: " << ms_count << endl;
}
```

### \<csignal\> ###
1. signal 宏定义
2. signal 操作函数
#### 信号注册函数 ####
1. *signal(signo, void func(int signo))*
#### 发送信号 ####
1. *raise(signo)*

### \<cstdarg\> ###
1. 可变长参数操作
2. *va_list args;*
3. *va_start()*
4. *va_arg()*
5. *va_end()*
```
#include <iostream>
#include <cstdarg>
int add_nums(int count, ...) 
{
    int result = 0;
    std::va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        result += va_arg(args, int);
    }
    va_end(args);
    return result;
}
int main() 
{
    std::cout << add_nums(4, 25, 25, 50, 50) << '\n';
}
```
### \<cstddef\> ###
1. size_t long unsigned int
2. byte (byte 类) *等效于unsigned char(0-255)*
3. *std::to_integer(byte)* **实现byte 到 integer 的转换**
```
int main() {
    byte a{10};
    byte b{0b100011};
    byte c = a & b;
    cout << "change byte to integer: " << to_integer<int>(c) << endl;
}
```

### \<cstdlib\> ###

#### 宏定义 ####
1. *size_t*  **long unsigned int**
2. *NULL*    **0 or nullptr**

#### 函数 ####
##### 系统相关的函数 #####
1. *abort()*
2. *exit()*
3. *atexit()*
4. *system()*
5. *getenv()*
6. *malloc()*
7. *calloc()*
8. *realloc()*
9. *free()*
```
void _exit() { cout << "exit" << endl; }
int main() {
    atexit(_exit);
    system("ls");
    char *path = getenv("PATH");
    cout << "PATH: " << path << endl;
    exit(-1);
}
```
##### 类型转换相关的函数 #####
1. *atoi*
2. *atod*
3. *atof*
4. *strtol*
5. *strtod*
6. *strtof*
```
int main() {
    cout << "int    value: " << strtol("12345", NULL, 10) << endl;
    cout << "double value: " << strtod("123.45", NULL) << endl;
}
```

### \<ctime\> ###
#### 时间相关数据结构 ####
1. time_t **long int**
2. struct tm **日历时间结构体**
3. struct timespc **纳秒精度时间戳**

#### 时间相关操作 ####
1. time **获取当前时间戳**
2. localtime **时间戳转换成日历时间**
3. gmtime    **时间戳转换成日历时间**
4. mktime **日历时间转换成时间戳**
5. strftime **时间格式化, input必须是tm**
6. *ctime(time_t \*)* **快速格式化时间**
7. *asctime(tm \*)* **快速格式化时间**
```
int main() 
{
    time_t t0; //基本时间
    time(&t0);
    printf("t0 = %ld\n", t0);
    struct tm *tmPtr = localtime(&t0);
    printf("%d/%d/%d\n", tmPtr->tm_year, tmPtr->tm_mon, tmPtr->tm_mday);
    time_t t1 = mktime(tmPtr);
    printf("t1 = %ld\n", t1);
    struct tm *tmPtr01 = gmtime(&t1);
    printf("%d/%d/%d\n", tmPtr->tm_year, tmPtr->tm_mon, tmPtr->tm_mday);
    char tbuffer[512] = {0};
    strftime(tbuffer, sizeof(tbuffer), "%Y-%m-%d %H:%M:%S\n", tmPtr01);
    printf("%s", tbuffer);
    //快速格式化时间函数
    char *p01 = ctime(&t1);
    char *p02 = asctime(tmPtr01);
    printf("01:%s\n", p01);
    printf("02:%s\n", p02);
}
```

### \<functional\> 函数对象 ###
1. *function* **通用函数对象的模板类**
2. *plus*     **具体函数对象的模板类**
3. *minus*
4. *multipies*
5. *divides*
6. *modules*
7. *negate*
8. *equal_to*
9. *not_equal_to*
10. *greater*
11. *less*
12. *greater_equal*
13. *less_equal*
```
int main() {
    function<bool(int, int)> callback = greater<int>();
    vector<int> mvec{1, 2, 3, 4, 5};
    sort(mvec.begin(), mvec.end(), callback);
    for_each(mvec.begin(), mvec.end(),
             [](int val) { cout << "val: " << val << endl; });
}
```

#### 高阶函数 ####
1. *bind* **绑定参数函数 (参数为可调用对象的引用)**
2. *ref()* **向高阶函数传递引用参数**
3. *cref()* **向高阶函数传递常引用参数**
4. *invok*  **调用函数 (参数为可调用对象的引用)**
5. *invok_r* **调用函数 (控制返回类型)**
```
int main() {
    function<bool(int, int)> callback = greater<int>();
    function<bool(int)> mcallback = bind(callback, placeholders::_1, 10);
    cout << invoke(callback, 1, 2) << endl;
    cout << invoke(mcallback, 11) << endl;
}
```

### \<initializer_list\> ###
1. 用于自定义类的列表初始化

### \<optional\> ###
1. 提供可选的target 值

### \<memory\> ###
#### 智能指针 ####
1. *unique_ptr*
2. *shared_ptr*
3. *make_shared*
4. *make_unique*
```
void fdeleter(int *ptr) {
    cout << "delete " << *ptr << endl;
    delete[] ptr;
};
struct mdeleter {
    void operator()(int *ptr) {
        cout << "delete " << *ptr << endl;
        delete[] ptr;
    }
};
int main() {
    shared_ptr<int> intPtr(new int[10], fdeleter);
    shared_ptr<int> intPtr(new int[10], mdeleter());
    return 0;
}
```

### \<cstdint\> ###
1. *int8_t*
2. *int16_t*
3. *int32_t*
4. *int64_t*
5. *uint8_t*
6. *uint16_t*
7. *uint32_t*
8. *uint64_t*
9. *intptr_t*
10. *uintptr_t
11. *intmax_t*
12. *uintmax_t*

### \<csassert\> ###
1. assert((condition)) **宏函数**

### \<cerrno\> ###
1. *errno* **线程安全的全局变量**
2. *error 宏定义* 
3. E2BIG
4. EACCES
5. EAGAIN
6. EBUSY
7. EDEADLK
8. EEXIST
9. EFAULT
10. EINTR
11. EINVAL

### \<exception\> 基本异常类 ###
#### logic error ####
1. *invalid_argument*
2. *domain_error*
3. *length_error*
4. *out_of_range*
5. *future_error*

#### runtime error ####
1. *range_error*
2. *overflow_error*
3. *underflow_error*
4. *regex_error*
5. *system_error*

#### 支持方法 ####
1. *e.what()* **返回异常描述**

### \<stdexcept\> 由基本异常类继承而来的标准异常类 ###
#### logic error ####
1. *invalid_argument*
2. *domain_error*
3. *length_error*
4. *out_of_range*
5. *future_error*

#### runtime error ####
1. *range_error*
2. *overflow_error*
3. *underflow_error*
4. *regex_error*
5. *system_error*

## 字符/字符串相关 ##
### \<cctype\> 单字符处理 ###
1. *isalnum* **checks if a character is alphanumeric**
2. *isalpha* **checks if a character is alphabetic**
3. *islower* **checks if a character is lowercase**
4. *isupper* **checks if a character is an uppercase character**
5. *isdigit* **checks if a character is a digit**
6. *isxdigit* **checks if a character is a hexadecimal character**
7. *iscntrl* **checks if a character is a control character**
8. *isgraph* **checks if a character is a graphical character**
9. *isspace* **checks if a character is a space character**
10. *isblank* **checks if a character is a blank character**
11. *isprint* **checks if a character is a printing character**
12. *ispunct* **checks if a character is a punctuation character**
13. *tolower* **converts a character to lowercase**
14. *toupper* **converts a character to uppercase**

### \<string\> c++字符串处理 ###
#### basic_string 类 ####
1. *assign(count, c)* **支持 assign 动态赋值**
2. *assign(string str)* **支持 assign 动态赋值**
3. *at(index)*
4. *front*
5. *back*
6. *data*
7. *c_str*
8. *empty*
9.  *size/length*
10. *capacity*
11. *reverse*
12. *begin(end)/cbegin(cend)/rbegin(rend)*
13. *shrink_to_fit*
#### 通用字符串处理函数 ####
1. *stoi*
2. *stol*
3. *stoll*
4. *stoul*
5. *stoull*
6. *stof*
7. *stod*
8. *stold*
9. *to_string(/\*int, float, double \*/)*

### \<cstring\> C字符串处理 ###
#### 类型和宏定义 ####
1. *NULL* 
2. *size_t* **long unsiged int**

#### C字符处理函数(null-terminated 字符数组) ####
1. *strcpy*
2. *strncpy*
3. *strcat*
4. *strncat*
5. *strcmp*
6. *strncmp*
7. *strlen*
8. *strerror* **获取错误信息**

#### memory 操作函数(byte为单位) ####
1. *memcpy*
2. *memset*
3. *memcmp*
4. *memmove*
5. *memchr*

## C++ 容器 ##
### \<array\> ###
1. 声明时候必须确定类型和大小
2. 支持复制, 不支持动态扩展
#### 方法 ####
1. *size()*
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *data* **指向数据的指针**

### \<vector\> ###
1. 支持在尾部添加和删除元素
2. 支持 assign 动态赋值
#### 方法 ####
1. *size()*
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *data* **指向数据的指针**
6. *insert*
7. *erase*
8. *push_back*
9. *pop_back*

### \<deque\> 双端队列 ###
1. 支持双端元素的添加和删除
2. 容量支持动态扩展
3. 支持 assign 动态赋值

#### 方法 ####
1. *size()*
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *erase*

### \<forward_list\> 单向链表 ###
1. 单向链表只保存 header 信息
2. 原生单向链表, 不包含*size方法*
3. 支持前端添加和删除元素
4. 普通插入元素的时候只能在指定位置的后面插入元素
5. 支持 assign 动态赋值

#### 方法 ####
2. *empty()*
3. *begin/cbegin*
4. *before_begin/cbefor_begin* **返回首元素前面位置的迭代器**
5. *end/cend*
6. *insert_after* **只能往后插入元素**
7. *erase_after* **删除指定位置后面的元素**
8. *push_front*
9.  *pop_front*
10. *remove*      **删除特定值元素(不会真正删除)**
11. *remove_if*  **删除满足条件的特定元素(不会真正删除)**
12. *reverse*
13. *unique*
14. *sort*

### \<list\> 双向链表 ###
1. 双向链表保存header和tail,以及size信息
2. 支持 *size 方法*
3. 支持两端插入和删除元素
4. 支持 assign 动态赋值
#### 方法 ####
1. *size()* 
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *insert* 
6. *erase* 
7. *push_front*
8. *pop_front*
9. *push_back*
10. *pop_back*
11. *remove*      
12. *remove_if*  
13. *reverse*
14. *unique*
15. *sort*

### \<map\> 字典(自动排序) ###
1. 支持 *size 方法*
2. 元素自动排序
3. 基于元素比较器排序, 或者基于容器比较器实现自动排序
#### 方法 ####
1. *size()* 
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *insert* **只能往后插入元素**
6. *erase* 
7. *count*
8. *find*

### \<set\> 集合(自动排序) ###
1. 支持 *size 方法*
2. 元素自动排序
3. 基于元素比较器排序, 或者基于容器比较器实现自动排序
#### 方法 ####
1. *size()* 
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *insert* **只能往后插入元素**
6. *erase* 
7. *count*
8. *find*

### \<unoredered_map\> 字典(无序) ###
1. 支持 *size 方法*
#### 方法 ####
1. *size()* 
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *insert* **只能往后插入元素**
6. *erase* 
7. *count*
8. *find*

### \<unordered_set\> 集合(无序) ###
1. 支持 *size 方法*
#### 方法 ####
1. *size()* 
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *insert* **只能往后插入元素**
6. *erase* 
7. *count*
8. *find*

### \<algorithm\>  ###
#### 帅选 ####
1. *all_of* **刷选器**
2. *any_of* **刷选器**
3. *none_of* **刷选器**
4. *for_each*
5. *for_each_n*
#### 查找 ####
6. *count* **查找具体值**
7. *count_if* **查找条件**
8. *find* **查找具体值**
9. *find_if*  **查找条件**
10. *find_if_not*  **查找条件**
11. *find_first_of* **查找集合中的任意一个元素**
12. *copy*
13. *copy_if*
14. *copy_n*
15. *fill*
16. *fill_n*
17. *remove*
18. *remove_if*
19. *replace*
20. *replace_if*
21. *transform*
22. *reverse*
23. *shuffle*
24. *unique*
25. *sort*
26. *stable_sort*

### \<numeric\> 数值计算 ###
1. *reduce*
2. *accumulate*

### \<ratio\> 有理分数 ###
1. *ration<1, 1000>*

### \<numbers\> 数学常量 ###
1. *e*
2. *pi*

### \<ios\> ###
1. ios 基类定义
2. 提供ios类最基本的管理接口
#### 流状态管理 ####
1. *good*
2. *fail*
3. *bad*
4. *eof*
#### opcode 管理 ###
1. *ios::out*
2. *ios::in*
3. *ios::app*
4. *trunc*
5. *binary*

### \<istream\> (basic istream) ###
#### 单字符操作 ####
1. *get* **获取单个字符**
2. *peek* **获取单个字符, 但是保留指针位置**
3. *unget* **撤销取走的字符**
4. *putback(c)* **向流中添加元素**

#### 行操作 ####
5. *getline* **获取一行**
6. *read* **读取指定数量的字符**

### 流定位 ###
1. *seekg(offset, whence)*
2. *tellg*

### \<ostream\> (basic ostream) ###
1. *endl* **\n**
2. *ends* **\0**
3. *flush* **刷新输出流**

### \<iostream\> ###
1. 标准输入输出流对象


### \<fstream\> (继承自basic_iostream) ###
1. 文件输入输出流(上层流对象)
2. *ofstream/ifstream/fstream*
#### 支持方法 ####
1. *open* **绑定关联文件**
2. *close* **解除关联文件绑定**
3. *is_open* **判断是否绑定到文件**

### \<sstream\> (继承自basic_iostream) 字符串流 ###
#### 三类字符串流 ####
1. *istingstream*
2. *ostringstream*
3. *stringstream*
#### 支持方法 ####
1. *str()* **从流对象返回字符串**
2. *str("abc")* **设置流对象字符串**
3. *rdbuf()* **返回底层字符对象的指针**

### \<iomanip\> ###
1. 拓展的输出格式化\<setxxx\>

#### 支持方法 ####
1. *setw*
2. *setfill*
3. *setbase*
4. *setprecision* **有效位数**

#### I/O格式化 (ios_basic方法) ####
5. *std::left*
6. *std::right*
7. *fixed* **设置精度为小数点后的保留位数**
8. *dec/otc/hex*
9. *scientific*
10. *hexfloat*
11. *defaultfloat*

### \<cstdio\> 缓冲I/O ###
#### 类型定义 ####
1. *fpos_t*
2. *size_t*
3. *NULL*
4. *EOF*
5. *BUFSIZ*  **默认缓冲区大小(8192)**
6. *_IOFBF/_IOLBF/_IONBF* **三种缓冲类型**

#### I/O 操作 ####
1. *fopen*
2. *freopen* **在已经打开的流上重新打开**
3. *fclose*
4. *fflush* **刷新对应的流**
5. *setbuf* **z只能设置全缓冲或者无缓冲,并且缓冲区大小必须是BUFSZ**
5. *setvbuf* **设置对应流缓冲类型和缓冲区大小**
6. *fread/fwrite* **读写二进制数据块**
7.  *fgetc/fputc* **单字节的读写**
8. *getc/putc* **单字节读写(宏函数)*
9. *fgets/fputs* **按行读写**
10. *scanf/fscanf*
11. *printf/fprintf*
12. *ftell* **返回偏移量**
13. *fseek* **设置偏移量**
14. *rewind* **重置偏移量**
15. *ferror* **错误检查**
16. *feof* **文件尾端检查**
17. *perror* **打印errno错误信息**

#### 文件操作 ####
1. *tmpnam* **获取一个临时文件名(随机文件名,部分随机或者完全随机)**
2. *tmpfile* **由临时文件名生成临时文件**
3. *mkdtemp* **替代接口, 直接按pattern创建临时目录**
4. *mkstmp*  **替代接口, 创建按pattern创建临时文件**
5. *remove* **删除文件**
6. *rename* **重命名文件**

### <filesystem> ###
#### path 类 ####
1. path **路径描述类**
2. path **类对象可以通过字符串创建**
3. path 类对象可以处理目录、文件、扩展名的操作
##### path 类支持的方法 #####
1. root_name
2. *root_directory*
3. *root_path* **返回根路径**
4. *relative_path* **返回相对根路径的相对路径**
5. *parent_path* **返回当前路径的上一级路径**
6. *filename* **返回文件名**
7. *stem* **返回主干名(除扩展外的其他部分)**
8. *extension* **返回扩展名**

#### C++ 通用的文件操作方法(基于path对象) ###
##### 文件操作 #####
1. *current_path* **返回当前路径**
2. *exists*       **判断文件是否存在**
3. *file_size*    **返回文件大小**
4. *read_symlink* **获取符号链接指向的目标值**
5. *remove*       **删除文件**
6. *remove_all*   **递归删除文件**
7. *rename*       **重命名文件**
8. *copy*         **复制文件/目录**
9. *copy_file     **复制文件内容**
10. *copy_symlink **复制符号链接**
11. create_directory **创建目录**
12. create_directories **创建多级目录**

##### 文件类型判断 #####
1. *is_block_file*
2. *is_character_file*
3. *is_directory*
4. *is_empty*
5. *is_fifo*
6. *is_other*
7. *is_regular_file*
8. *is_socket*
9. *is_symlink*
10. *status_known*

##### 文件的遍历 #####
1. *directory_iterator(path&)* **获取指定路径下的文件列表对象**

        #include<iostream>
        #include<fstream>
        #include<filesystem>
        int main()
        {
            namespace fs = filesystem;
            fs::create_directories("sandbox/a/b");
            std::ofstream("sandbox/file1.txt");
            std::ofstream("sandbox/file2.txt");
            fs::directory_iterator it("sandbox");
            for (auto &p : it)
                std::cout << p.path() << endl;
            fs::remove_all("sandbox");
        }

### \<regex\> ###
#### 这则匹配相关的类 ####
1. regex
2. smatch/cmatch **match_results<string>**
3. ssub_match **sub_match<string>**
#### 正则匹配相关的函数 ####
1. *regex_match*
2. *regex_search*
3. *regex_replace*

### \<atomic\> ###
1. 原子数据类型
2. 数据维度的原子操作锁

#### 常用的原子数据类型 ####
1. atomic_bool
2. atomic_int
3. atomic_char
4. atomic_long
5. atomic_uint
6. atomic_ulong

### \<thread\> ###
#### thread 类 ####
1. *id* **共有成员**
2. *get_id*
3. *joinable*
4. *join*
5. *detach*
6. *swap* **交换两个thread对象**
7. *yield* **交出CPU,由运行状态转换成就绪状态**
#### this_thread ####
1. *get_id()*
2. *sleep_for(duration)*
3. *sleep_until(time_point)*

### \<mutex\> ###
#### 互斥量 ####
1. mutex **普通锁**
2. timed_mutex **限时锁**
3. recursive_mutex **递归锁**

#### 高级锁对象 ####
1. lock_guard **通用锁**
2. unique_lock **用于条件变量** 

#### 锁对象的操作 ####
1. *try_lock(lock0&, lock1&, lock2,...)*
**依次加锁, 直到完成或加锁失败返回,成功返回-1，否则返回对应加锁失败的锁对象下标**
2. *lock(lock0&, lock1&, lock2,...)*
**依次加锁, 并且阻塞, 同时避免死锁**
3. *call_once(once_flag& flag, func, args)*
**仅调用一次**
#### 限时锁提供额外操作方法 ####
1. *try_lock_for*
2. *try_lock_until*

### semaphore ### 
**信号量，通过许可的生产<release>和消费<acquire>来实现线程同步**
**release和acquire可以在不同的线程独立进行,实现更精细的同步控制**
1. *counting_semaphore*
2. *binary_semaphore*

#### 操作方法 ####
1. *release* 
2. *acquire* **block**
3. *try_acquire* **no block**
4. *try_acquire_for* **timed block**
5. *try_acquire_until* **timed block**

### \<shared_mutex\> ###
**共享锁可以提供多种加锁状态**

#### 两种共享锁 ####
1. shared_mutex
2. shared_timed_mutex*

#### 操作方法 ####
1. *lock*
2. *try_lock*
3. *unlock*
4. *lock_shared*
5. *try_lock_shared*
6. *unlock_shared*



