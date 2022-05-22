## cpp header ##

### <bitset> ###
1. *bitset<n> bitset* **n位的bitset, 每个bit代表true和false
2. *bitset.reset()*
3. *bitset.set(/*position, true*/)*
4. *bitset.flip(/*position*/)*
5. *支持与或非操作*
6. *支持索引访问*

### <chrone> ###
1. *std::ratio<a, b>* **单位时间刻度**
2. *std::chrone::duration<int, ratio>* **时间长度**
    1) 刻度单位
    2) 刻度计数
3. *std::chrone::minutes*
4. *std::chrone::seconds*
5. *std::chrone::duration_cast<target_duration>(source_duration)*
6. *std::chrone::system_clock::now()* **获取当前系统时间**

### <csignal> ###
1. signal 宏定义
2. signal 操作函数

### <cstdarg> ###
1. 可变长参数操作
2. *va_list args;*
3. *va_start()*
4. *va_arg()*
5. *va_end()*

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

### <cstddef> ###
1. size_t long unsigned int
2. byte (byte 类)
3. *std::to_integer(byte)* **实现byte 到 integer 的转换**

### <cstdlib> ###

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

##### 类型转换相关的函数 #####
1. *atoi*
2. *atod*
3. *atof*
4. *strtol*
5. *strtod*
6. *strtof*

### <ctime> ###
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


### <functional> 函数对象 ###
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

#### 高阶函数 ####
1. *bind* **绑定参数函数**
2. *ref()* **向高阶函数传递引用**
3. *cref()* **向高阶函数传递常引用**
4. *invok*  **调用函数**
5. *invok_r* **调用函数(控制返回类型)**

### <initializer_list> ###
1. 用于自定义类的列表初始化

### <optional> ###
1. 提供可选的target 值

### <memory> ###
#### 职能指针 ####
1. *unique_ptr*
2. *shared_ptr*
3. *make_shared*
4. *make_unique*

### <cstdint> ###
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

### <csassert> ###
1. assert((condition)) **宏函数**

### <cerrno> ###
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

### <exception> 基本异常类 ###
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

#### member function ####
1. *e.what()* **返回异常描述**

### <stdexcept> 由基本异常类继承而来的标准异常类 ###
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
### <cctype> 单字符处理 ###
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

### <string> c++字符串处理 ###
1. *stoi*
2. *stol*
3. *stoll*
4. *stoul*
5. *stoull*
6. *stof*
7. *stod*
8. *stold*
9. *to_string(/\*int, float, double \*/)*

### <cstring> C字符串处理 ###
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
### <array> ###
1. 声明时候必须确定类型和大小
2. 支持复制, 不支持动态扩展
#### 方法 ####
1. *size()*
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *data* **指向数据的指针**

### <vector> ###
1. 支持在尾部添加和删除元素
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

### <deque> 双端队列 ###
1. 支持双端元素的添加和删除
2. 容量支持动态扩展

#### 方法 ####
1. *size()*
2. *empty()*
3. *begin/cbegin/rbegin*
4. *end/cend/rend*
5. *erase*

### <forward_list> 单向链表 ###
1. 单向链表只保存 header 信息
2. 原生单向链表, 不包含*size方法*
3. 支持前端添加和删除元素
4. 普通插入元素的时候只能在指定位置的后面插入元素

#### 方法 ####
2. *empty()*
3. *begin/cbegin*
4. *end/cend*
5. *insert_after* **只能往后插入元素**
6. *erase_after* 
7. *push_front*
8. *pop_front*
9. *remove*      **删除特定值元素(不会真正删除)**
10. *remove_if*  **删除满足条件的特定元素(不会真正删除)**
11. *reverse*
12. *unique*
13. *sort*

### <list> 双向链表 ###
1. 双向链表保存header和tail,以及size信息
2. 支持 *size 方法*
3. 支持两端插入和删除元素
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

### <map> 字典(自动排序) ###
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

### <set> 集合(自动排序) ###
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

### <unoredered_map> 字典(无序) ###
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

### <unordered_set> 集合(无序) ###
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

### <algorithm>  ###
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

### <numeric> 数值计算 ###
1. *reduce*
2. *accumulate*

### <ratio> 有理分数 ###
1. *ration<1, 1000>*

### <numbers> 数学常量 ###
1. *e*
2. *pi*

### <ios> ###
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

### <istream> (basic istream) ###
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

### <ostream> (basic ostream) ###
1. *endl* **\n**
2. *ends* **\0**
3. *flush* **刷新输出流**

### <iostream> ###
1. 标准输入输出流对象


### <fstream> (继承自basic_iostream) ###
1. 文件输入输出流(上层流对象)
2. *ofstream/ifstream/fstream*
#### 支持方法 ####
1. *open* **绑定关联文件**
2. *close* **解除关联文件绑定**
3. *is_open* **判断是否绑定到文件**

### <sstream> (继承自basic_iostream) 字符串流 ###
#### 三类字符串流 ####
1. *istingstream*
2. *ostringstream*
3. *stringstream*
#### 支持方法 ####
1. *str()* **从流对象返回字符串**
2. *str("abc")* **设置流对象字符串**
3. *rdbuf()* **返回底层字符对象的指针**

### <iomanip> ###
1. 拓展的输出格式化<setxxx>

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

### <cstdio> 缓冲I/O ###
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

















