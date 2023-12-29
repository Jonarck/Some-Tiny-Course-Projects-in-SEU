### 说明

#### 文件结构

    ├─main
    │  ├─java
    │  │  ├─drools
    │  │  │      Drools_tutorial.java // Drools示例程序
    │  │  │      Triple.java // 三元组类
    │  │  │      
    │  │  ├─jena
    │  │  │      Jena_tutorial.java // Jena示例程序
    │  │  │      Jena_turorial_2.java // Jena示例程序2，与PPT上对应
    │  │  │      
    │  │  └─rdfox
    │  │          RDFox_tutorial.java // RDFox示例程序
    │  │          
    │  └─resources
    │      ├─data
    │      │      finance-data.nt // 实例数据
    │      │      finance-onto.nt // 本体数据（格式为N-TRIPLE）
    │      │      finance-onto.owl // 本体数据（格式为OWL，内容与上一个相同）
    │      │      finance.drl // Drools规则文件
    │      │      
    │      └─META-INF
    │          │  kmodule.xml // Drools配置文件
    │          │  
    │          └─maven
    │                  pom.properties
    │                  
    └─test
        └─java
#### 备注

1. Drools和Jena的依赖包均由MAVEN管理，RDFox的依赖包位于lib目录下，需手动添加依赖
2. 本示例程序使用的IDE：IntelliJ IDEA 2017.2.6
3. 本示例程序使用JDK版本：1.8.0_51