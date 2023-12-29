package drools;

import org.apache.jena.base.Sys;
import org.kie.api.KieServices;
import org.kie.api.runtime.KieContainer;
import org.kie.api.runtime.KieSession;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.URISyntaxException;

/**
 * Drools使用教程
 */
public class Drools_tutorial {
    public static void main(String[] args) throws IOException, URISyntaxException {
        // 获取drools实现的 KieServices 实例
        KieServices ks = KieServices.Factory.get();
        // kieServices默认加载 classpath:META-INF/kmodule.xml 得到 KieContainer
        KieContainer kContainer = ks.getKieClasspathContainer();
        // 通过 kContainer获取 kmodule.xml 中定义的 ksession
        KieSession kSession = kContainer.newKieSession("ksession-rules");

        // 读取本体数据
        BufferedReader ontoReader = new BufferedReader(new FileReader(new File(Drools_tutorial.class.getResource("/data/finance-onto.nt").toURI())));
        String ontoLine = null;
        while((ontoLine = ontoReader.readLine()) != null){
            if(ontoLine.isEmpty())
                continue;
            else {
                String[] lineArray = ontoLine.split(" ");
                // 向WorkingMemory插入三元组
                kSession.insert(new Triple(lineArray[0], lineArray[1], lineArray[2]));
            }
        }

        // 读取实例数据
//        BufferedReader dataReader = new BufferedReader(new FileReader(new File(Drools_tutorial.class.getResource("/data/finance-data.nt").toURI())));
//        String dataLine = null;
//        while((dataLine = dataReader.readLine()) != null){
//            if(dataLine.isEmpty())
//                continue;
//            else{
//                String[] lineArray = dataLine.split(" ");
//                // 向WorkingMemory插入三元组
//                kSession.insert(new Triple(lineArray[0], lineArray[1], lineArray[2]));
//            }
//        }

        // 输出推理前的三元组及数量
        System.out.println("Triples num before reasoning: " + kSession.getObjects().toArray().length);
		System.out.println("Triples Before Reasoning:");
        outputTriples(kSession);

        long startTime = System.currentTimeMillis();
        System.out.println("Execute...");
        // 使规则引擎执行规则
        kSession.fireAllRules();
        long endTime = System.currentTimeMillis();
        long runningTime = endTime - startTime;

        // 输出推理后的三元组及数量
        System.out.println("Facts num after reasoning: " + kSession.getObjects().toArray().length);
        System.out.println("Facts After Reasoning:");
        outputTriples(kSession);

        // 输出推理时间
        System.out.println("Total time cost: " + runningTime + "ms");
    }

    /**
     * 用于输出KieSession中的所有三元组
     * @param kSession
     */
    public static void outputTriples(KieSession kSession) {
        Object[] array2 = kSession.getObjects().toArray();
        for(int i = 0; i < array2.length; i++){
            System.out.println(1+i + ": " + array2[i]);
        }
    }
}
