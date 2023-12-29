package jena;

import org.apache.jena.rdf.model.InfModel;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.rdf.model.StmtIterator;
import org.apache.jena.reasoner.Reasoner;
import org.apache.jena.reasoner.ReasonerRegistry;

import java.io.FileNotFoundException;

/**
 * Jena使用教程
 */
public class Jena_tutorial {
    public static void main(String[] args) throws FileNotFoundException {
        // 为本体创建Model
        Model ontoModel = ModelFactory.createDefaultModel();
        ontoModel.read(Jena_tutorial.class.getResource("/data/finance-onto.owl").toString());

        // 为实例数据创建Model
        Model dataModel = ModelFactory.createDefaultModel();
        dataModel.read(Jena_tutorial.class.getResource("/data/finance-data.nt").toString());

        // 创建一个新Model将本体与实例数据进行合并
        Model fusionModel = ModelFactory.createDefaultModel();
        fusionModel.add(ontoModel);
        fusionModel.add(dataModel);

        // 输出推理前的数据
        System.out.println("Triples Before Reasoning:");
        outputAllTriples(fusionModel);

        // 在合并后的数据模型上进行OWL推理
        Reasoner reasoner = ReasonerRegistry.getOWLReasoner();
        InfModel inf = ModelFactory.createInfModel(reasoner, fusionModel);

        // 输出推理后的数据
        System.out.println("Triples After Reasoning:");
        outputAllTriples(inf.getDeductionsModel());

        ontoModel.close();
        dataModel.close();
        fusionModel.close();

    }

    /**
     * 输出当前Model里所有三元组
     * @param model
     */
    public static void outputAllTriples(Model model) {
        StmtIterator itr = model.listStatements();
        while (itr.hasNext()) {
            System.out.println(itr.nextStatement());
        }
    }
}
