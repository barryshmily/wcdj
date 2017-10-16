

Maven是基于项目对象模型（POM），可以通过一小段描述信息来管理项目的构建，报告和文档的软件项目管理工具。其他类似的工具还有Ant，gradle等。

安装：
可以在官网maven.apache.org下载，有各种OS的版本，本文选择OSX的二进制包，并根据下载安装的路径配置PATH环境变量。完成后通过`mvn -version`命令可以查看到如下信息。

PS: jdk需要事先下载，本文使用的是Java SE 9，JDK版本是1.8。
$java -version
java version "9"
Java(TM) SE Runtime Environment (build 9+181)
Java HotSpot(TM) 64-Bit Server VM (build 9+181, mixed mode)
http://www.oracle.com/technetwork/java/javase/downloads/index.html

Maven的目录结构：
```
src
     main
          java
               package
     test
          java
               package
     resources
```

## 基本概念

1. 坐标
任何一个依赖，插件等称为`构件`。`构件`通过`坐标`作为其**唯一的标识**。
坐标 = groupId + artifactId + version

2. 仓库
用于管理项目的依赖。分为本地仓库和远程仓库。

远程仓库地址：
maven/lib/maven-model-builder-3.3.3.jar
pom-4.0.0.xml   所有的pom都会继承这个pom
https://repo.maven.apache.org/maven2

The Central Repository （包含了绝大多数的java的开源项目，比如，spring）
http://search.maven.org/#browse

3. 镜像仓库
镜像就是指，a和b提供了一样的功能。
maven的远程仓库通常都在国外，可能需要翻墙或者访问速度过慢，因此可以改为访问国内的镜像仓库，更快更稳定一些。
maven/conf/settings.xml，找到`mirrors`节点，修改为：

``` xml
  <mirrors>
    <!-- mirror
     | Specifies a repository mirror site to use instead of a given repository. The repository that
     | this mirror serves has an ID that matches the mirrorOf element of this mirror. IDs are used
     | for inheritance and direct lookup purposes, and must be unique across the set of mirrors.
     |
    <mirror>
      <id>mirrorId</id>
      <mirrorOf>repositoryId</mirrorOf>
      <name>Human Readable Name for this Mirror.</name>
      <url>http://my.repository.com/repo/path</url>
    </mirror>
     -->

	<mirror>
      <id>maven.net.cn</id>
      <mirrorOf>central</mirrorOf>
      <name>central mirror in China</name>
      <url>http://maven.net.cn/content/groups/public</url>
    </mirror>

  </mirrors>
```

4. 更改本地仓库的位置
默认位置：~/.m2/repository
修改settings.xml，修改`localRepository`节点：
``` xml
 <!-- localRepository
   | The path to the local repository maven will use to store artifacts.
   |
   | Default: ${user.home}/.m2/repository
  <localRepository>/path/to/local/repo</localRepository>
  -->
```

## 常用命令

编译
mvn compile

单元测试
mvn test

打包
mvn package

删除maven生成的目标文件（字节码）
mvn clean

例如：
```
[INFO] Scanning for projects...
[INFO] 
[INFO] ------------------------------------------------------------------------
[INFO] Building maven02-model 0.0.1-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven02-model ---
[INFO] Deleting /Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven02/target
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 0.481 s
[INFO] Finished at: 2017-10-16T22:28:49+08:00
[INFO] Final Memory: 6M/23M
[INFO] ------------------------------------------------------------------------
```

安装jar包到本地仓库
mvn install

自动创建maven规定的目录骨架（使用archetype插件）
mvn archetype:generate
mvn archetype:generate -DgroupId=com.imooc.maven03 -DartifactId=maven03-service -Dversion=1.0.0-SNAPSHOT -Dpackage=com.imooc.maven03.service

其中：
-DgroupId=组织名，一般是公司网址的反写+项目名
-DartifactId=项目名-模块名
-Dversion=版本号
-Dpackage=代码所存在的包名

例如：自动创建后的提示信息。
```
[INFO] Using property: groupId = com.imooc.maven03
[INFO] Using property: artifactId = maven03-service
[INFO] Using property: version = 1.0.0-SNAPSHOT
[INFO] Using property: package = com.imooc.maven03.service
Confirm properties configuration:
groupId: com.imooc.maven03
artifactId: maven03-service
version: 1.0.0-SNAPSHOT
package: com.imooc.maven03.service
 Y: : y
[INFO] ----------------------------------------------------------------------------
[INFO] Using following parameters for creating project from Old (1.x) Archetype: maven-archetype-quickstart:1.1
[INFO] ----------------------------------------------------------------------------
[INFO] Parameter: basedir, Value: /Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven03
[INFO] Parameter: package, Value: com.imooc.maven03.service
[INFO] Parameter: groupId, Value: com.imooc.maven03
[INFO] Parameter: artifactId, Value: maven03-service
[INFO] Parameter: packageName, Value: com.imooc.maven03.service
[INFO] Parameter: version, Value: 1.0.0-SNAPSHOT
[INFO] project created from Old (1.x) Archetype in dir: /Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven03/maven03-service
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 02:12 min
[INFO] Finished at: 2017-10-16T22:50:17+08:00
[INFO] Final Memory: 16M/55M
[INFO] ------------------------------------------------------------------------
```

自动创建的目录结构：

```
gerryyang@mba:maven03$pwd
/Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven03
gerryyang@mba:maven03$ls
maven03-service/
gerryyang@mba:maven03$tree
.
└── maven03-service
    ├── pom.xml
    └── src
        ├── main
        │   └── java
        │       └── com
        │           └── imooc
        │               └── maven03
        │                   └── service
        │                       └── App.java
        └── test
            └── java
                └── com
                    └── imooc
                        └── maven03
                            └── service
                                └── AppTest.java

14 directories, 3 files
```


## 注意事项

Maven安装完成后默认使用JDK 1.5，导致mvn compile时报错，然后通过下面方法将其修改为使用JDK 1.8可以解决问题。

1. 在Maven的用户配置文件`~/.m2/settings.xml`中指定使用jdk 1.8版本。

``` xml
<?xml version="1.0" encoding="UTF-8"?>

<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0 http://maven.apache.org/xsd/settings-1.0.0.xsd">

	<profiles>
		<profile>    
			<id>jdk-1.8</id>    
			<activation>    
				<activeByDefault>true</activeByDefault>    
				<jdk>1.8</jdk>    
			</activation>    
			<properties>    
				<maven.compiler.source>1.8</maven.compiler.source>    
				<maven.compiler.target>1.8</maven.compiler.target>    
				<maven.compiler.compilerVersion>1.8</maven.compiler.compilerVersion>    
			</properties>    
		</profile>
	</profiles>
</settings>
```

2. 在Maven所创建项目的`pom.xml`中指定要使用的JDK版本。
``` xml
	<build>  
		<plugins>  
			<plugin>  
				<groupId>org.apache.maven.plugins</groupId>  
				<artifactId>maven-compiler-plugin</artifactId>  
				<version>3.1</version>  
				<configuration>  
					<source>1.8</source>  
					<target>1.8</target>  
					<encoding>UTF-8</encoding>  
				</configuration>  
			</plugin>  
		</plugins>  
	</build>  

```

补充说明：

关于Maven的配置使用说明，在Maven安装目录conf下settings.xml中有说明：
``` xml
<!--
 | This is the configuration file for Maven. It can be specified at two levels:
 |
 |  1. User Level. This settings.xml file provides configuration for a single user,
 |                 and is normally provided in ${user.home}/.m2/settings.xml.
 |
 |                 NOTE: This location can be overridden with the CLI option:
 |
 |                 -s /path/to/user/settings.xml
 |
 |  2. Global Level. This settings.xml file provides configuration for all Maven
 |                 users on a machine (assuming they're all using the same Maven
 |                 installation). It's normally provided in
 |                 ${maven.conf}/settings.xml.
 |
 |                 NOTE: This location can be overridden with the CLI option:
 |
 |                 -gs /path/to/global/settings.xml
 |
 | The sections in this sample file are intended to give you a running start at
 | getting the most out of your Maven installation. Where appropriate, the default
 | values (values used when the setting is not specified) are provided.
 |
 |-->

```

## 一些错误解决方案

1. `mvn compile`时提示找不到某个类。

问题描述：

```
[INFO] ------------------------------------------------------------------------
[INFO] BUILD FAILURE
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 2.632 s
[INFO] Finished at: 2017-10-16T22:13:02+08:00
[INFO] Final Memory: 11M/38M
[INFO] ------------------------------------------------------------------------
[ERROR] Failed to execute goal org.apache.maven.plugins:maven-compiler-plugin:3.1:compile (default-compile) on project maven02-model: Compilation failure: Compilation failure: 
[ERROR] /Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven02/src/main/java/com/imooc/maven02/util/Speak.java:[3,31] 程序包com.imooc.maven01.model不存在
[ERROR] /Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven02/src/main/java/com/imooc/maven02/util/Speak.java:[7,28] 找不到符号
[ERROR]   符号:   类 HelloWorld
[ERROR]   位置: 类 com.imooc.maven02.util.Speak
[ERROR] -> [Help 1]
[ERROR] 
[ERROR] To see the full stack trace of the errors, re-run Maven with the -e switch.
[ERROR] Re-run Maven using the -X switch to enable full debug logging.
[ERROR] 
[ERROR] For more information about the errors and possible solutions, please read the following articles:
[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoFailureException
```

通常的做法是将包含这个class的jar包加入到classpath中，而maven提供了更简便的方式，通过`mvn install`将依赖的jar包安装到本地仓库中，同时再在主项目的`pom.xml`中添加引用依赖。在引用的时候，首先查找本地仓库，如果不存在则从远程仓库中下载到本地仓库。

2. 单元测试，断言错误

例子：
```
[INFO] ------------------------------------------------------------------------
[INFO] BUILD FAILURE
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 3.439 s
[INFO] Finished at: 2017-10-16T22:24:03+08:00
[INFO] Final Memory: 13M/43M
[INFO] ------------------------------------------------------------------------
[ERROR] Failed to execute goal org.apache.maven.plugins:maven-surefire-plugin:2.12.4:test (default-test) on project maven02-model: There are test failures.
[ERROR] 
[ERROR] Please refer to /Users/gerryyang/github_project/wcdj/tools/SpringBoot/maven/maven02/target/surefire-reports for the individual test results.
[ERROR] -> [Help 1]
[ERROR] 
[ERROR] To see the full stack trace of the errors, re-run Maven with the -e switch.
[ERROR] Re-run Maven using the -X switch to enable full debug logging.
[ERROR] 
[ERROR] For more information about the errors and possible solutions, please read the following articles:
[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoFailureException
```

此错误可以通过上述提示在`target`目录下找到错误原因。

```
-------------------------------------------------------------------------------
Test set: com.imooc.maven02.util.SpeakTest
-------------------------------------------------------------------------------
Tests run: 1, Failures: 1, Errors: 0, Skipped: 0, Time elapsed: 0.123 sec <<< FAILURE!
testSayHi(com.imooc.maven02.util.SpeakTest)  Time elapsed: 0.02 sec  <<< FAILURE!
org.junit.ComparisonFailure: expected:<Hello World[]> but was:<Hello World[, from gerry]>
	at org.junit.Assert.assertEquals(Assert.java:125)
	at org.junit.Assert.assertEquals(Assert.java:147)
	at com.imooc.maven02.util.SpeakTest.testSayHi(SpeakTest.java:9)
	at java.base/jdk.internal.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at java.base/jdk.internal.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)

省略
```

## 参考

Spring Boot入门

http://www.imooc.com/learn/767

Maven
http://www.imooc.com/learn/443
