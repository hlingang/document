## STL Algorithm

- __search
- 在序列1 中查找序列2 首次出现的位置。
```
__search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
        _ForwardIterator2 __first2, _ForwardIterator2 __last2,
        _BinaryPredicate  __predicate)
{
    // Test for empty ranges
    if (__first1 == __last1 || __first2 == __last2)
return __first1;

    // Test for a pattern of length 1.
    _ForwardIterator2 __p1(__first2);
    if (++__p1 == __last2)
return std::__find_if(__first1, __last1,
    __gnu_cxx::__ops::__iter_comp_iter(__predicate, __first2));

    // General case.
    _ForwardIterator1 __current = __first1;

    for (;;)
{
    __first1 =
    std::__find_if(__first1, __last1,
    __gnu_cxx::__ops::__iter_comp_iter(__predicate, __first2));

    if (__first1 == __last1)
    return __last1;

    _ForwardIterator2 __p = __p1;
    __current = __first1;
    if (++__current == __last1)
    return __last1;

    while (__predicate(__current, __p))
    {
        if (++__p == __last2)
    return __first1;
        if (++__current == __last1)
    return __last1;
    }
    ++__first1;
}
    return __first1;
}

search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
	   _ForwardIterator2 __first2, _ForwardIterator2 __last2)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator1>)
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator2>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_ForwardIterator1>::value_type,
    typename iterator_traits<_ForwardIterator2>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    return std::__search(__first1, __last1, __first2, __last2,
            __gnu_cxx::__ops::__iter_equal_to_iter());
}

search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
	   _ForwardIterator2 __first2, _ForwardIterator2 __last2,
	   _BinaryPredicate  __predicate)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator1>)
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator2>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_BinaryPredicate,
    typename iterator_traits<_ForwardIterator1>::value_type,
    typename iterator_traits<_ForwardIterator2>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    return std::__search(__first1, __last1, __first2, __last2,
            __gnu_cxx::__ops::__iter_comp_iter(__predicate));
}
```
- __search_n_aux 查找连续出现n个满足条件的元素位置
```
__search_n_aux(_ForwardIterator __first, _ForwardIterator __last,
		   _Integer __count, _UnaryPredicate __unary_pred,
		   std::forward_iterator_tag)
{
    __first = std::__find_if(__first, __last, __unary_pred);
    while (__first != __last)
{
    typename iterator_traits<_ForwardIterator>::difference_type
    __n = __count;
    _ForwardIterator __i = __first;
    ++__i;
    while (__i != __last && __n != 1 && __unary_pred(__i))
    {
        ++__i;
        --__n;
    }
    if (__n == 1)
    return __first;
    if (__i == __last)
    return __last;
    __first = std::__find_if(++__i, __last, __unary_pred);
}
    return __last;
}

__search_n_aux(_RandomAccessIter __first, _RandomAccessIter __last,
		   _Integer __count, _UnaryPredicate __unary_pred,
		   std::random_access_iterator_tag)
{
    typedef typename std::iterator_traits<_RandomAccessIter>::difference_type
_DistanceType;

    _DistanceType __tailSize = __last - __first;
    _DistanceType __remainder = __count;

    while (__remainder <= __tailSize) // the main loop...
{
    __first += __remainder;
    __tailSize -= __remainder;
    // __first here is always pointing to one past the last element of
    // next possible match.
    _RandomAccessIter __backTrack = __first; 
    while (__unary_pred(--__backTrack))
    {
        if (--__remainder == 0)
    return (__first - __count); // Success
    }
    __remainder = __count + 1 - (__first - __backTrack);
}
    return __last; // Failure
}
__search_n(_ForwardIterator __first, _ForwardIterator __last,
	       _Integer __count,
	       _UnaryPredicate __unary_pred)
{
    if (__count <= 0)
return __first;

    if (__count == 1)
return std::__find_if(__first, __last, __unary_pred);

    return std::__search_n_aux(__first, __last, __count, __unary_pred,
                std::__iterator_category(__first));
}
```

- __find_end
- 在序列1 中查找序列2 最后一次出现的位置
```
[实现方法]: 多次匹配直到找到最后一次匹配
__find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
	       _ForwardIterator2 __first2, _ForwardIterator2 __last2,
	       forward_iterator_tag, forward_iterator_tag,
	       _BinaryPredicate __comp)
{
    if (__first2 == __last2)
return __last1;

    _ForwardIterator1 __result = __last1;
    while (1)
{
    _ForwardIterator1 __new_result
    = std::__search(__first1, __last1, __first2, __last2, __comp);
    if (__new_result == __last1)
    return __result;
    else
    {
        __result = __new_result;
        __first1 = __new_result;
        ++__first1;
    }
}
}
[实现方法]: 构造反向迭代器 反向查找第一个匹配的位置
__find_end(_BidirectionalIterator1 __first1,
	       _BidirectionalIterator1 __last1,
	       _BidirectionalIterator2 __first2,
	       _BidirectionalIterator2 __last2,
	       bidirectional_iterator_tag, bidirectional_iterator_tag,
	       _BinaryPredicate __comp)
{
    // concept requirements
    __glibcxx_function_requires(_BidirectionalIteratorConcept<
                _BidirectionalIterator1>)
    __glibcxx_function_requires(_BidirectionalIteratorConcept<
                _BidirectionalIterator2>)

    typedef reverse_iterator<_BidirectionalIterator1> _RevIterator1;
    typedef reverse_iterator<_BidirectionalIterator2> _RevIterator2;

    _RevIterator1 __rlast1(__first1);
    _RevIterator2 __rlast2(__first2);
    _RevIterator1 __rresult = std::__search(_RevIterator1(__last1), __rlast1,
                        _RevIterator2(__last2), __rlast2,
                        __comp);

    if (__rresult == __rlast1)
return __last1;
    else
{
    _BidirectionalIterator1 __result = __rresult.base();
    std::advance(__result, -std::distance(__first2, __last2));
    return __result;
}
}
```
- __find_if
- 在序列中查找第一个满足条件的元素位置
```
__find_if(_InputIterator __first, _InputIterator __last,
        _Predicate __pred, input_iterator_tag)
{
    while (__first != __last && !__pred(__first))
++__first;
    return __first;
}
```

- __find_if_not
 - 在序列中查找第一个不满足条件的元素位置
 ```
__find_if_not(_InputIterator __first, _InputIterator __last,
        _Predicate __pred)
{
    return std::__find_if(__first, __last,
            __gnu_cxx::__ops::__negate(__pred),
            std::__iterator_category(__first));
}
 ```
 - __find_if_not_n
 - 在序列的前n个元素里面查找第一个不满足条件的元素
 ```
__find_if_not_n(_InputIterator __first, _Distance& __len, _Predicate __pred)
{
    for (; __len; --__len,  (void) ++__first)
if (!__pred(__first))
    break;
    return __first;
}
```
- all_of / any_of / none_of
- 判断序列中元素是不是满足特定条件
```
all_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    { return __last == std::find_if_not(__first, __last, __pred); }

none_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
{ return __last == std::find_if(__first, __last, __pred); }

any_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    { return !std::none_of(__first, __last, __pred); }

```
- __remove_copy_if
- 删除匹配的元素，并且将新序列进行拷贝

```
__remove_copy_if(_InputIterator __first, _InputIterator __last,
		     _OutputIterator __result, _Predicate __pred)
{
    for (; __first != __last; ++__first)
if (!__pred(__first))
    {
    *__result = *__first;
    ++__result;
    }
    return __result;
}
```
- copy_if
- 拷贝符合条件的元素到新序列
```
copy_if(_InputIterator __first, _InputIterator __last,
	    _OutputIterator __result, _Predicate __pred)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_function_requires(_UnaryPredicateConcept<_Predicate,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    for (; __first != __last; ++__first)
if (__pred(*__first))
    {
    *__result = *__first;
    ++__result;
    }
    return __result;
}
```
- __remove_if 在原序列删除符合条件的元素
```
__remove_if(_ForwardIterator __first, _ForwardIterator __last,
		_Predicate __pred)
{
    __first = std::__find_if(__first, __last, __pred);
    if (__first == __last)
return __first;
    _ForwardIterator __result = __first;
    ++__first;
    for (; __first != __last; ++__first)
if (!__pred(__first))
    {
    *__result = _GLIBCXX_MOVE(*__first);
    ++__result;
    }
    return __result;
}

remove(_ForwardIterator __first, _ForwardIterator __last,
	   const _Tp& __value)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<
                _ForwardIterator>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_ForwardIterator>::value_type, _Tp>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__remove_if(__first, __last,
    __gnu_cxx::__ops::__iter_equals_val(__value));
}
```

- __adjacent_find
- 查找首次相邻元素出现重复的位置
```
__adjacent_find(_ForwardIterator __first, _ForwardIterator __last,
		    _BinaryPredicate __binary_pred)
{
    if (__first == __last)
return __last;
    _ForwardIterator __next = __first;
    while (++__next != __last)
{
    if (__binary_pred(__first, __next))
    return __first;
    __first = __next;
}
    return __last;
}
adjacent_find(_ForwardIterator __first, _ForwardIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_EqualityComparableConcept<
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__adjacent_find(__first, __last,
                __gnu_cxx::__ops::__iter_equal_to_iter());
}
adjacent_find(_ForwardIterator __first, _ForwardIterator __last,
		  _BinaryPredicate __binary_pred)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_BinaryPredicate,
    typename iterator_traits<_ForwardIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__adjacent_find(__first, __last,
        __gnu_cxx::__ops::__iter_comp_iter(__binary_pred));
}
```
- __unique
- 移除相邻位置的重复元素
```
__unique(_ForwardIterator __first, _ForwardIterator __last,
	     _BinaryPredicate __binary_pred)
{
    // Skip the beginning, if already unique.
    __first = std::__adjacent_find(__first, __last, __binary_pred);
    if (__first == __last)
return __last;

    // Do the real copy work.
    _ForwardIterator __dest = __first;
    ++__first;
    while (++__first != __last)
if (!__binary_pred(__dest, __first))
    *++__dest = _GLIBCXX_MOVE(*__first);
    return ++__dest;
}

unique(_ForwardIterator __first, _ForwardIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<
                _ForwardIterator>)
    __glibcxx_function_requires(_EqualityComparableConcept<
            typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__unique(__first, __last,
            __gnu_cxx::__ops::__iter_equal_to_iter());
}
unique(_ForwardIterator __first, _ForwardIterator __last,
	   _BinaryPredicate __binary_pred)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<
                _ForwardIterator>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_BinaryPredicate,
    typename iterator_traits<_ForwardIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__unique(__first, __last,
            __gnu_cxx::__ops::__iter_comp_iter(__binary_pred));
}
```
- __unique_copy
- 移除相邻的重复元素，并将序列进行拷贝
```
__unique_copy(_ForwardIterator __first, _ForwardIterator __last,
		  _OutputIterator __result, _BinaryPredicate __binary_pred,
		  forward_iterator_tag, output_iterator_tag)
{
    // concept requirements -- iterators already checked
    __glibcxx_function_requires(_BinaryPredicateConcept<_BinaryPredicate,
    typename iterator_traits<_ForwardIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)

    _ForwardIterator __next = __first;
    *__result = *__first;
    while (++__next != __last)
if (!__binary_pred(__first, __next))
    {
    __first = __next;
    *++__result = *__first;
    }
    return ++__result;
}

unique_copy(_InputIterator __first, _InputIterator __last,
		_OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_function_requires(_EqualityComparableConcept<
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    if (__first == __last)
return __result;
    return std::__unique_copy(__first, __last, __result,
            __gnu_cxx::__ops::__iter_equal_to_iter(),
            std::__iterator_category(__first),
            std::__iterator_category(__result));
}
unique_copy(_InputIterator __first, _InputIterator __last,
		_OutputIterator __result,
		_BinaryPredicate __binary_pred)
{
    // concept requirements -- predicates checked later
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    if (__first == __last)
return __result;
    return std::__unique_copy(__first, __last, __result,
        __gnu_cxx::__ops::__iter_comp_iter(__binary_pred),
            std::__iterator_category(__first),
            std::__iterator_category(__result));
}
```
- __reverse
- 反转序列
```
__reverse(_BidirectionalIterator __first, _BidirectionalIterator __last,
	      bidirectional_iterator_tag)
{
    while (true)
if (__first == __last || __first == --__last)
    return;
else
    {
    std::iter_swap(__first, __last);
    ++__first;
    }
}

__reverse(_RandomAccessIterator __first, _RandomAccessIterator __last,
	      random_access_iterator_tag)
{
    if (__first == __last)
return;
    --__last;
    while (__first < __last)
{
    std::iter_swap(__first, __last);
    ++__first;
    --__last;
}
}

reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_BidirectionalIteratorConcept<
                _BidirectionalIterator>)
    __glibcxx_requires_valid_range(__first, __last);
    std::__reverse(__first, __last, std::__iterator_category(__first));
}

reverse_copy(_BidirectionalIterator __first, _BidirectionalIterator __last,
		 _OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_BidirectionalIteratorConcept<
                _BidirectionalIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_BidirectionalIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    while (__first != __last)
{
    --__last;
    *__result = *__last;
    ++__result;
}
    return __result;
}
```
- __includes
- 判断序列是否包含某个子序列（序列均已排序）
```
__includes(_InputIterator1 __first1, _InputIterator1 __last1,
	       _InputIterator2 __first2, _InputIterator2 __last2,
	       _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
{
    if (__comp(__first2, __first1))
    return false;
    if (!__comp(__first1, __first2))
    ++__first2;
    ++__first1;
}
    return __first2 == __last2;
}

includes(_InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _InputIterator2 __last2)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set(__first1, __last1, __first2);
    __glibcxx_requires_sorted_set(__first2, __last2, __first1);
    __glibcxx_requires_irreflexive2(__first1, __last1);
    __glibcxx_requires_irreflexive2(__first2, __last2);

    return std::__includes(__first1, __last1, __first2, __last2,
                __gnu_cxx::__ops::__iter_less_iter());
}

includes(_InputIterator1 __first1, _InputIterator1 __last1,
	     _InputIterator2 __first2, _InputIterator2 __last2,
	     _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set_pred(__first1, __last1, __first2, __comp);
    __glibcxx_requires_sorted_set_pred(__first2, __last2, __first1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first1, __last1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first2, __last2, __comp);

    return std::__includes(__first1, __last1, __first2, __last2,
                __gnu_cxx::__ops::__iter_comp_iter(__comp));
}
```
- __replace_copy_if
- 拷贝序列，并且对符合条件的元素进行替换
```
__replace_copy_if(_InputIterator __first, _InputIterator __last,
		      _OutputIterator __result,
		      _Predicate __pred, const _Tp& __new_value)
{
    for (; __first != __last; ++__first, (void)++__result)
if (__pred(__first))
    *__result = __new_value;
else
    *__result = *__first;
    return __result;
}

replace_copy(_InputIterator __first, _InputIterator __last,
		 _OutputIterator __result,
		 const _Tp& __old_value, const _Tp& __new_value)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_InputIterator>::value_type, _Tp>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__replace_copy_if(__first, __last, __result,
        __gnu_cxx::__ops::__iter_equals_val(__old_value),
                        __new_value);
}

replace_copy_if(_InputIterator __first, _InputIterator __last,
		    _OutputIterator __result,
		    _Predicate __pred, const _Tp& __new_value)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_function_requires(_UnaryPredicateConcept<_Predicate,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__replace_copy_if(__first, __last, __result,
            __gnu_cxx::__ops::__pred_iter(__pred),
                        __new_value);
}
```
- __is_sorted_until / is_sorted
- 判断排序出现的最后的位置/判断序列是否排序
```
__is_sorted_until(_ForwardIterator __first, _ForwardIterator __last,
		      _Compare __comp)
{
    if (__first == __last)
return __last;

    _ForwardIterator __next = __first;
    for (++__next; __next != __last; __first = __next, (void)++__next)
if (__comp(__next, __first))
    return __next;
    return __next;
}
is_sorted_until(_ForwardIterator __first, _ForwardIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_LessThanComparableConcept<
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    __glibcxx_requires_irreflexive(__first, __last);

    return std::__is_sorted_until(__first, __last,
                __gnu_cxx::__ops::__iter_less_iter());
}
is_sorted_until(_ForwardIterator __first, _ForwardIterator __last,
		    _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_ForwardIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    __glibcxx_requires_irreflexive_pred(__first, __last, __comp);

    return std::__is_sorted_until(__first, __last,
                __gnu_cxx::__ops::__iter_comp_iter(__comp));
}

is_sorted(_ForwardIterator __first, _ForwardIterator __last)
{ return std::is_sorted_until(__first, __last) == __last; }

is_sorted(_ForwardIterator __first, _ForwardIterator __last,
	      _Compare __comp)
{ return std::is_sorted_until(__first, __last, __comp) == __last; }
```
- for_each/transform
- 对序列中的每个元素执行一次操作/对序列中的每个元素执行一次操作并进行拷贝
```
for_each(_InputIterator __first, _InputIterator __last, _Function __f)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_requires_valid_range(__first, __last);
    for (; __first != __last; ++__first)
__f(*__first);
    return __f; // N.B. [alg.foreach] says std::move(f) but it's redundant.
}
transform(_InputIterator __first, _InputIterator __last,
	      _OutputIterator __result, _UnaryOperation __unary_op)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    // "the type returned by a _UnaryOperation"
    __typeof__(__unary_op(*__first))>)
    __glibcxx_requires_valid_range(__first, __last);

    for (; __first != __last; ++__first, (void)++__result)
*__result = __unary_op(*__first);
    return __result;
}
transform(_InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _OutputIterator __result,
	      _BinaryOperation __binary_op)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    // "the type returned by a _BinaryOperation"
    __typeof__(__binary_op(*__first1,*__first2))>)
    __glibcxx_requires_valid_range(__first1, __last1);

    for (; __first1 != __last1; ++__first1, (void)++__first2, ++__result)
*__result = __binary_op(*__first1, *__first2);
    return __result;
}
```
- __find_if/__count_if
- 查找符合条件的元素首次出现的位置/ 统计符合条件的元素的数量
```
__find_if(_InputIterator __first, _InputIterator __last,
	      _Predicate __pred, input_iterator_tag)
{
    while (__first != __last && !__pred(__first))
++__first;
    return __first;
}
find(_InputIterator __first, _InputIterator __last,
	 const _Tp& __val)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_InputIterator>::value_type, _Tp>)
    __glibcxx_requires_valid_range(__first, __last);
    return std::__find_if(__first, __last,
            __gnu_cxx::__ops::__iter_equals_val(__val));
}

find_if(_InputIterator __first, _InputIterator __last,
	    _Predicate __pred)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_UnaryPredicateConcept<_Predicate,
        typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__find_if(__first, __last,
            __gnu_cxx::__ops::__pred_iter(__pred));
}
__count_if(_InputIterator __first, _InputIterator __last, _Predicate __pred)
{
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    for (; __first != __last; ++__first)
if (__pred(__first))
    ++__n;
    return __n;
}
count(_InputIterator __first, _InputIterator __last, const _Tp& __value)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_InputIterator>::value_type, _Tp>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__count_if(__first, __last,
                __gnu_cxx::__ops::__iter_equals_val(__value));
}
count_if(_InputIterator __first, _InputIterator __last, _Predicate __pred)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_UnaryPredicateConcept<_Predicate,
    typename iterator_traits<_InputIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    return std::__count_if(__first, __last,
                __gnu_cxx::__ops::__pred_iter(__pred));
}
```
- find_first_of
- 判断序列2 中任意一个元素在序列1 中首次出现的位置
```
find_first_of(_InputIterator __first1, _InputIterator __last1,
		  _ForwardIterator __first2, _ForwardIterator __last2)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_InputIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    for (; __first1 != __last1; ++__first1)
for (_ForwardIterator __iter = __first2; __iter != __last2; ++__iter)
    if (*__first1 == *__iter)
    return __first1;
    return __last1;
}

find_first_of(_InputIterator __first1, _InputIterator __last1,
		  _ForwardIterator __first2, _ForwardIterator __last2,
		  _BinaryPredicate __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_BinaryPredicate,
    typename iterator_traits<_InputIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first1, __last1);
    __glibcxx_requires_valid_range(__first2, __last2);

    for (; __first1 != __last1; ++__first1)
for (_ForwardIterator __iter = __first2; __iter != __last2; ++__iter)
    if (__comp(*__first1, *__iter))
    return __first1;
    return __last1;
}
```
- replace 替换指定元素
```
replace(_ForwardIterator __first, _ForwardIterator __last,
	    const _Tp& __old_value, const _Tp& __new_value)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<
                _ForwardIterator>)
    __glibcxx_function_requires(_EqualOpConcept<
    typename iterator_traits<_ForwardIterator>::value_type, _Tp>)
    __glibcxx_function_requires(_ConvertibleConcept<_Tp,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    for (; __first != __last; ++__first)
if (*__first == __old_value)
    *__first = __new_value;
}

replace_if(_ForwardIterator __first, _ForwardIterator __last,
	       _Predicate __pred, const _Tp& __new_value)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_ForwardIteratorConcept<
                _ForwardIterator>)
    __glibcxx_function_requires(_ConvertibleConcept<_Tp,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_function_requires(_UnaryPredicateConcept<_Predicate,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    for (; __first != __last; ++__first)
if (__pred(*__first))
    *__first = __new_value;
}
```
- generate
- 填充序列
```
generate(_ForwardIterator __first, _ForwardIterator __last,
	     _Generator __gen)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_GeneratorConcept<_Generator,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);

    for (; __first != __last; ++__first)
*__first = __gen();
}

generate_n(_OutputIterator __first, _Size __n, _Generator __gen)
{
    // concept requirements
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    // "the type returned by a _Generator"
    __typeof__(__gen())>)

    typedef __decltype(std::__size_to_integer(__n)) _IntSize;
    for (_IntSize __niter = std::__size_to_integer(__n);
    __niter > 0; --__niter, (void) ++__first)
*__first = __gen();
    return __first;
}
```
- random_shuffle
- 随机化序列顺序
```
random_shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_RandomAccessIteratorConcept<
    _RandomAccessIterator>)
    __glibcxx_requires_valid_range(__first, __last);

    if (__first != __last)
for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
    {
    // XXX rand() % N is not uniformly distributed
    _RandomAccessIterator __j = __first
                + std::rand() % ((__i - __first) + 1);
    if (__i != __j)
        std::iter_swap(__i, __j);
    }
}

random_shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last,
		   _RandomNumberGenerator&& __rand)
{
    // concept requirements
    __glibcxx_function_requires(_Mutable_RandomAccessIteratorConcept<
    _RandomAccessIterator>)
    __glibcxx_requires_valid_range(__first, __last);

    if (__first == __last)
return;
    for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
{
    _RandomAccessIterator __j = __first + __rand((__i - __first) + 1);
    if (__i != __j)
    std::iter_swap(__i, __j);
}
}
```
- __merge
- 合并2个序列，并且保持顺序
```
__merge(_InputIterator1 __first1, _InputIterator1 __last1,
	    _InputIterator2 __first2, _InputIterator2 __last2,
	    _OutputIterator __result, _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
{
    if (__comp(__first2, __first1))
    {
        *__result = *__first2;
        ++__first2;
    }
    else
    {
        *__result = *__first1;
        ++__first1;
    }
    ++__result;
}
    return std::copy(__first2, __last2,
            std::copy(__first1, __last1, __result));
}

merge(_InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _InputIterator2 __last2,
	  _OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)	
    __glibcxx_requires_sorted_set(__first1, __last1, __first2);
    __glibcxx_requires_sorted_set(__first2, __last2, __first1);
    __glibcxx_requires_irreflexive2(__first1, __last1);
    __glibcxx_requires_irreflexive2(__first2, __last2);

    return _GLIBCXX_STD_A::__merge(__first1, __last1,
                    __first2, __last2, __result,
                    __gnu_cxx::__ops::__iter_less_iter());
}

merge(_InputIterator1 __first1, _InputIterator1 __last1,
	  _InputIterator2 __first2, _InputIterator2 __last2,
	  _OutputIterator __result, _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set_pred(__first1, __last1, __first2, __comp);
    __glibcxx_requires_sorted_set_pred(__first2, __last2, __first1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first1, __last1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first2, __last2, __comp);

    return _GLIBCXX_STD_A::__merge(__first1, __last1,
            __first2, __last2, __result,
            __gnu_cxx::__ops::__iter_comp_iter(__comp));
}
```
- __set_union/set_intersection/set_difference
- 两个序列的集合操作，并且保持原来的顺序
__set_union(_InputIterator1 __first1, _InputIterator1 __last1,
		_InputIterator2 __first2, _InputIterator2 __last2,
		_OutputIterator __result, _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
{
    if (__comp(__first1, __first2))
    {
        *__result = *__first1;
        ++__first1;
    }
    else if (__comp(__first2, __first1))
    {
        *__result = *__first2;
        ++__first2;
    }
    else
    {
        *__result = *__first1;
        ++__first1;
        ++__first2;
    }
    ++__result;
}
    return std::copy(__first2, __last2,
            std::copy(__first1, __last1, __result));
}

set_union(_InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _InputIterator2 __last2,
	      _OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set(__first1, __last1, __first2);
    __glibcxx_requires_sorted_set(__first2, __last2, __first1);
    __glibcxx_requires_irreflexive2(__first1, __last1);
    __glibcxx_requires_irreflexive2(__first2, __last2);

    return _GLIBCXX_STD_A::__set_union(__first1, __last1,
            __first2, __last2, __result,
            __gnu_cxx::__ops::__iter_less_iter());
}
set_union(_InputIterator1 __first1, _InputIterator1 __last1,
	      _InputIterator2 __first2, _InputIterator2 __last2,
	      _OutputIterator __result, _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set_pred(__first1, __last1, __first2, __comp);
    __glibcxx_requires_sorted_set_pred(__first2, __last2, __first1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first1, __last1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first2, __last2, __comp);

    return _GLIBCXX_STD_A::__set_union(__first1, __last1,
            __first2, __last2, __result,
            __gnu_cxx::__ops::__iter_comp_iter(__comp));
}

__set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
		       _InputIterator2 __first2, _InputIterator2 __last2,
		       _OutputIterator __result, _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
if (__comp(__first1, __first2))
    ++__first1;
else if (__comp(__first2, __first1))
    ++__first2;
else
    {
    *__result = *__first1;
    ++__first1;
    ++__first2;
    ++__result;
    }
    return __result;
}
set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
		     _InputIterator2 __first2, _InputIterator2 __last2,
		     _OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set(__first1, __last1, __first2);
    __glibcxx_requires_sorted_set(__first2, __last2, __first1);
    __glibcxx_requires_irreflexive2(__first1, __last1);
    __glibcxx_requires_irreflexive2(__first2, __last2);

    return _GLIBCXX_STD_A::__set_intersection(__first1, __last1,
                    __first2, __last2, __result,
                    __gnu_cxx::__ops::__iter_less_iter());
}

set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
		     _InputIterator2 __first2, _InputIterator2 __last2,
		     _OutputIterator __result, _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set_pred(__first1, __last1, __first2, __comp);
    __glibcxx_requires_sorted_set_pred(__first2, __last2, __first1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first1, __last1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first2, __last2, __comp);

    return _GLIBCXX_STD_A::__set_intersection(__first1, __last1,
            __first2, __last2, __result,
            __gnu_cxx::__ops::__iter_comp_iter(__comp));
}

__set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
		     _InputIterator2 __first2, _InputIterator2 __last2,
		     _OutputIterator __result, _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
if (__comp(__first1, __first2))
    {
    *__result = *__first1;
    ++__first1;
    ++__result;
    }
else if (__comp(__first2, __first1))
    ++__first2;
else
    {
    ++__first1;
    ++__first2;
    }
    return std::copy(__first1, __last1, __result);
}

set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
		   _InputIterator2 __first2, _InputIterator2 __last2,
		   _OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)	
    __glibcxx_requires_sorted_set(__first1, __last1, __first2);
    __glibcxx_requires_sorted_set(__first2, __last2, __first1);
    __glibcxx_requires_irreflexive2(__first1, __last1);
    __glibcxx_requires_irreflexive2(__first2, __last2);

    return _GLIBCXX_STD_A::__set_difference(__first1, __last1,
                __first2, __last2, __result,
                __gnu_cxx::__ops::__iter_less_iter());
}
set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
		   _InputIterator2 __first2, _InputIterator2 __last2,
		   _OutputIterator __result, _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set_pred(__first1, __last1, __first2, __comp);
    __glibcxx_requires_sorted_set_pred(__first2, __last2, __first1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first1, __last1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first2, __last2, __comp);

    return _GLIBCXX_STD_A::__set_difference(__first1, __last1,
                __first2, __last2, __result,
                __gnu_cxx::__ops::__iter_comp_iter(__comp));
}

__set_symmetric_difference(_InputIterator1 __first1,
			       _InputIterator1 __last1,
			       _InputIterator2 __first2,
			       _InputIterator2 __last2,
			       _OutputIterator __result,
			       _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
if (__comp(__first1, __first2))
    {
    *__result = *__first1;
    ++__first1;
    ++__result;
    }
else if (__comp(__first2, __first1))
    {
    *__result = *__first2;
    ++__first2;
    ++__result;
    }
else
    {
    ++__first1;
    ++__first2;
    }
    return std::copy(__first2, __last2, 
            std::copy(__first1, __last1, __result));
}

set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
			     _InputIterator2 __first2, _InputIterator2 __last2,
			     _OutputIterator __result)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_LessThanOpConcept<
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)	
    __glibcxx_requires_sorted_set(__first1, __last1, __first2);
    __glibcxx_requires_sorted_set(__first2, __last2, __first1);
    __glibcxx_requires_irreflexive2(__first1, __last1);
    __glibcxx_requires_irreflexive2(__first2, __last2);

    return _GLIBCXX_STD_A::__set_symmetric_difference(__first1, __last1,
                __first2, __last2, __result,
                __gnu_cxx::__ops::__iter_less_iter());
}

set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
			     _InputIterator2 __first2, _InputIterator2 __last2,
			     _OutputIterator __result,
			     _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator1>)
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator2>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_function_requires(_OutputIteratorConcept<_OutputIterator,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator1>::value_type,
    typename iterator_traits<_InputIterator2>::value_type>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_InputIterator2>::value_type,
    typename iterator_traits<_InputIterator1>::value_type>)
    __glibcxx_requires_sorted_set_pred(__first1, __last1, __first2, __comp);
    __glibcxx_requires_sorted_set_pred(__first2, __last2, __first1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first1, __last1, __comp);
    __glibcxx_requires_irreflexive_pred2(__first2, __last2, __comp);

    return _GLIBCXX_STD_A::__set_symmetric_difference(__first1, __last1,
            __first2, __last2, __result,
            __gnu_cxx::__ops::__iter_comp_iter(__comp));
}
```
- __min_element/__min_element
- 查找序列最大元素和最小元素的位置
```
__min_element(_ForwardIterator __first, _ForwardIterator __last,
		  _Compare __comp)
{
    if (__first == __last)
return __first;
    _ForwardIterator __result = __first;
    while (++__first != __last)
if (__comp(__first, __result))
    __result = __first;
    return __result;
}
inline min_element(_ForwardIterator __first, _ForwardIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_LessThanComparableConcept<
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    __glibcxx_requires_irreflexive(__first, __last);

    return _GLIBCXX_STD_A::__min_element(__first, __last,
            __gnu_cxx::__ops::__iter_less_iter());
}
min_element(_ForwardIterator __first, _ForwardIterator __last,
    _Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_ForwardIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    __glibcxx_requires_irreflexive_pred(__first, __last, __comp);

    return _GLIBCXX_STD_A::__min_element(__first, __last,
            __gnu_cxx::__ops::__iter_comp_iter(__comp));
}

__max_element(_ForwardIterator __first, _ForwardIterator __last,
		  _Compare __comp)
{
    if (__first == __last) return __first;
    _ForwardIterator __result = __first;
    while (++__first != __last)
if (__comp(__result, __first))
    __result = __first;
    return __result;
}

max_element(_ForwardIterator __first, _ForwardIterator __last)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_LessThanComparableConcept<
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    __glibcxx_requires_irreflexive(__first, __last);

    return _GLIBCXX_STD_A::__max_element(__first, __last,
            __gnu_cxx::__ops::__iter_less_iter());
}

max_element(_ForwardIterator __first, _ForwardIterator __last,
		_Compare __comp)
{
    // concept requirements
    __glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
    __glibcxx_function_requires(_BinaryPredicateConcept<_Compare,
    typename iterator_traits<_ForwardIterator>::value_type,
    typename iterator_traits<_ForwardIterator>::value_type>)
    __glibcxx_requires_valid_range(__first, __last);
    __glibcxx_requires_irreflexive_pred(__first, __last, __comp);

    return _GLIBCXX_STD_A::__max_element(__first, __last,
            __gnu_cxx::__ops::__iter_comp_iter(__comp));
}

```








