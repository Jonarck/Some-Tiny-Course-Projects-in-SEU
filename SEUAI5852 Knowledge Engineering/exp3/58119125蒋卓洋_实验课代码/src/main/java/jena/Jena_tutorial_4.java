package jena;

import org.apache.jena.rdf.model.*;
import org.apache.jena.reasoner.Reasoner;
import org.apache.jena.reasoner.rulesys.GenericRuleReasonerFactory;
import org.apache.jena.util.FileManager;
import org.apache.jena.util.PrintUtil;
import org.apache.jena.vocabulary.ReasonerVocabulary;

public class Jena_tutorial_4 {
    public static void main(String[] args){
        // 为使用demo创建一个名空间
        String demoURI = "http://jena.hpl.hp.com/demo#";
        PrintUtil.registerPrefix("demo", demoURI);

        //创建一个混合推理机（RDF）规范，它从外部文件加载数据
        Model m = ModelFactory.createDefaultModel();
        Resource configuration =  m.createResource();
        configuration.addProperty(ReasonerVocabulary.PROPruleMode, "hybrid");
        configuration.addProperty(ReasonerVocabulary.PROPruleSet,  "data/demo.rules");

        // 创建一个推理机的实例
        Reasoner reasoner = GenericRuleReasonerFactory.theInstance().create(configuration);


        // 载入测试数据
        Model data = FileManager.get().loadModel("data/demoData.rdf");
        InfModel infmodel = ModelFactory.createInfModel(reasoner, data);


        // 用“P”查询与“A”有关的所有事物
        Property p = data.getProperty(demoURI, "p");
        Resource a = data.getResource(demoURI + "a");
        StmtIterator i = infmodel.listStatements(a, p, (RDFNode)null);
        while (i.hasNext()) {
            System.out.println(" - " + PrintUtil.print(i.nextStatement()));
        }
    }

}
