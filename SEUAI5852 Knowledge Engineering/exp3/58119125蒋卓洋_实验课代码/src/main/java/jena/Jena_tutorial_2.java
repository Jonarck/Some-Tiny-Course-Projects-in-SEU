package jena;

import org.apache.jena.rdf.model.*;
import org.apache.jena.reasoner.Reasoner;
import org.apache.jena.reasoner.ReasonerRegistry;
import org.apache.jena.reasoner.ValidityReport;
import org.apache.jena.util.PrintUtil;
import org.apache.jena.vocabulary.OWL;
import org.apache.jena.vocabulary.RDF;
import org.apache.jena.vocabulary.RDFS;

import java.util.Iterator;

public class Jena_tutorial_2 {
    public static void main(String[] args) {
        Model myMod = ModelFactory.createDefaultModel();
        String finance = "http://www.example.org/kse/finance#";

        // 创建资源
        Resource shb = myMod.createResource(finance + "孙宏斌");
        Resource rczg = myMod.createResource(finance + "融创中国");
        Property control = myMod.createProperty(finance + "执掌");

        // 向模型中加入三元组
        myMod.add(shb, control, rczg);
        myMod.add(myMod.createResource(finance+"贾跃亭"), control, myMod.createResource(finance+"乐视网"));
        myMod.add(myMod.createResource(finance+"融创中国"), RDF.type, myMod.createResource(finance+"地产公司"));
        myMod.add(myMod.createResource(finance+"地产公司"), RDFS.subClassOf, myMod.createResource(finance+"公司"));
        myMod.add(myMod.createResource(finance+"公司"), RDFS.subClassOf, myMod.createResource(finance+"法人实体"));
        myMod.add(myMod.createResource(finance+"孙宏斌"), RDF.type, myMod.createResource(finance+"公司"));
        myMod.add(myMod.createResource(finance+"孙宏斌"), RDF.type, myMod.createResource(finance+"人"));
        myMod.add(myMod.createResource(finance+"人"), OWL.disjointWith, myMod.createResource(finance+"公司"));

        // 创建RDFS推理机
        InfModel inf_rdfs = ModelFactory.createRDFSModel(myMod);

        // 创建OWL推理机
        Reasoner reasoner = ReasonerRegistry.getOWLReasoner();
        InfModel inf_owl = ModelFactory.createInfModel(reasoner, myMod);

        // 查询原模型中 地产公司 与 法人实体 之间是否存在上下位关系
        Jena_tutorial_2.subClassOf(inf_rdfs, myMod.getResource(finance+"地产公司"), myMod.getResource(finance+"法人实体"));

        // 类别补全，输出推理后 融创中国 的类型
        printStatements(inf_owl, rczg, RDF.type, null);

        // 不一致检测
        ValidityReport validity = inf_owl.validate();
        if (validity.isValid()) {
            System.out.println("OK");
        } else {
            System.out.println("Conflicts");
            for (Iterator i = validity.getReports(); i.hasNext(); ) {
                ValidityReport.Report report = (ValidityReport.Report)i.next();
                System.out.println(" - " + report);
            }
        }

    }

    /**
     * 查询输入资源s与o之间是否存在上下位关系，存在则输出 yes，否则输出 no
     * @param m - 模型
     * @param s - 资源1
     * @param o - 资源2
     */
    public static void subClassOf(Model m, Resource s, Resource o) {
        // 遍历模型中的三元组
        for (StmtIterator i = m.listStatements(s, RDFS.subClassOf, o); i.hasNext(); ) {
            Statement stmt = i.nextStatement();
            System.out.println("yes!" );
            return;
        }
        System.out.println("no!");
    }

    /**
     * 输出模型m中满足给定模式的三元组
     * @param m
     * @param s
     * @param p
     * @param o
     */
    public static void printStatements(Model m, Resource s, Property p, Resource o) {
        for (StmtIterator i = m.listStatements(s,p,o); i.hasNext(); ) {
            Statement stmt = i.nextStatement();
            System.out.println(" - " + PrintUtil.print(stmt));
        }
    }
}
