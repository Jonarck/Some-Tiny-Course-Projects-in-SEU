package jena;

import org.apache.jena.rdf.model.*;
import org.apache.jena.reasoner.Reasoner;
import org.apache.jena.reasoner.rulesys.GenericRuleReasoner;
import org.apache.jena.reasoner.rulesys.Rule;
import java.util.Iterator;

public class Jena_tutorial_3 {
    public static void main(String[] args) {
        //建立模型，NS为名空间
        Model myMod = ModelFactory.createDefaultModel();
        String NS =  "urn:x-hp-jena:eg/";

        // 创建资源
        Resource A = myMod.createResource(NS + "A");
        Resource B = myMod.createResource(NS + "B");
        Resource C = myMod.createResource(NS + "C");
        Property p = myMod.createProperty(NS + "p");
        Property q = myMod.createProperty(NS + "q");
        Property r = myMod.createProperty(NS + "r");
        Property concatFirst = myMod.createProperty(NS + "concatFirst");
        Property concatSecond = myMod.createProperty(NS + "concatSecond");

        // 向模型中加入三元组
        myMod.add(A, p, B);
        myMod.add(B, q, C);
        myMod.add(r, concatFirst, p);
        myMod.add(r, concatSecond, q);

        // 创建GenericRule推理机
        String rules =
                "[r1: (?c ?concatFirst ?p), (?c ?concatSecond ?q) -> " +
                        "     [r1b: (?x ?c ?y) <- (?x ?p ?z) (?z ?q ?y)] ]";
        Reasoner reasoner = new GenericRuleReasoner(Rule.parseRules(rules));
        InfModel inf = ModelFactory.createInfModel(reasoner, myMod);
        //输出推理后由传递规则获得的三元组
        System.out.println("A *  =>");
        Iterator list = inf.listStatements(A, null, (RDFNode) null);
        while (list.hasNext()) {
            System.out.println(" - " + list.next());
        }
    }
}