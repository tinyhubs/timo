


/*
timo lang 的设计核心

面向接口开发：     提供接口能力

面向对象开发：     提供灵活的对象组合

类型安全：         const，modifiable，nullable，notnull，mutable

半自动内存管理：   支持对象生命周期依赖

提供匿名函数：     函数是第一类值

语言内DSL能力：    函数的最后一个匿名函数参数可以放到函数体外面

自动类型推导：     var

运算符重载：       提供基于接口的运算符重载机制

模板：             还在考虑中

异常机制：         还在考虑中

类型系统：         所有类型都是对象，所有类型都通过库来提供


chan
defer

if
else
for
continue
break
goto

// switch
// case
// default

func
return

interface
struct
type
map

const
var

import
package

fallthrough
go

select

1.对象模型
（1）只提供interface座位唯一的抽象机制
（2）需要提供mixin技术方便构造对象
（3）对象组合技术是否需要提供？

2.内存模型 




*/

