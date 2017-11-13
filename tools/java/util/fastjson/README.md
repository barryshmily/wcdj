

https://github.com/alibaba/fastjson
https://github.com/alibaba/fastjson/wiki/%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98



Fastjson is a Java library that can be used to convert Java Objects into their JSON representation. It can also be used to convert a JSON string to an equivalent Java object. Fastjson can work with arbitrary Java objects including pre-existing objects that you do not have source-code of.


``` java
package com.alibaba.fastjson;
public abstract class JSON {
      public static final String toJSONString(Object object);
      public static final <T> T parseObject(String text, Class<T> clazz, Feature... features);
}

// 序列化：

String jsonString = JSON.toJSONString(obj);

// 反序列化：

VO vo = JSON.parseObject("...", VO.class);

// 泛型反序列化：

import com.alibaba.fastjson.TypeReference;
List<VO> list = JSON.parseObject("...", new TypeReference<List<VO>>() {});
```


主要：
1. 对象的非public字段如果没有实现getter，则不会进行序列化
2. 对象的非public字段如果没有实现setter，则不会进行反序列化


Demo
http://www.cnblogs.com/Jie-Jack/p/3758046.html

