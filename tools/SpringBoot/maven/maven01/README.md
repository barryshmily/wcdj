
## 测试命令

编译
mvn compile

单元测试
mvn test

打包
mvn package


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
