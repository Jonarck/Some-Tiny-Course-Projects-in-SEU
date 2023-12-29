
package rdfox;

import uk.ac.ox.cs.JRDFox.JRDFoxException;
import uk.ac.ox.cs.JRDFox.Prefixes;
import uk.ac.ox.cs.JRDFox.store.DataStore;
import uk.ac.ox.cs.JRDFox.store.Resource;
import uk.ac.ox.cs.JRDFox.store.TupleIterator;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class Demo_bus {
    public static void main(String[] args) throws Exception {
        // 从本体与实例数据文件创建各自的文件对象

        File dataFile = new File(rdfox.Demo_bus.class.getResource("/data/bus_data.nt").toURI());
        File ruleFile = new File(rdfox.Demo_bus.class.getResource("/data/bus_rule.txt").toURI());
        // 创建数据存储
        DataStore store = new DataStore(DataStore.StoreType.ParallelSimpleNN);
        store.setNumberOfThreads(2);
        // 导入本体、实例数据与自定义规则
        store.importFiles(new File[] {dataFile});
        store.importFiles(new File[] {ruleFile});
//        store.importText(userDefinedRules);
        Prefixes prefixes = Prefixes.DEFAULT_IMMUTABLE_INSTANCE;
        // 创建SPARQL语句用于查询所有三元组
        TupleIterator tupleIterator = store.compileQuery("SELECT DISTINCT ?x ?y ?z WHERE{ ?x ?y ?z }", prefixes);

        System.out.println("开始程序运行！");
        ArrayList<String> oldTuple = evaluateAndPrintResults(prefixes, tupleIterator);

        System.out.println(String.format("原有的三元组总数量为%d个", oldTuple.size()));
        // 进行推理（Materialization + 规则推理）
        long startTime=System.currentTimeMillis();   //获取开始时间
        System.out.println("开始进行推理...............");
        store.applyReasoning();
        long tic=System.currentTimeMillis();   //获取开始时间
        ArrayList<String> newTuple = evaluateAndPrintResults(prefixes, tupleIterator);
        long toc=System.currentTimeMillis();   //获取结束时间
        System.out.println("查询时间： "+(toc-tic)+"ms");
        System.out.println(String.format("原有的三元组总数量为%d个", oldTuple.size()));
        System.out.println(String.format("推理过后的三元组总数量为%d个", newTuple.size()));
        System.out.println(String.format("推理出来的三元组总数量为%d个", newTuple.size()-oldTuple.size()));
//         打印出所有的推理出来的三元组
//        printReasoningTuple(oldTuple, newTuple);
        long endTime=System.currentTimeMillis(); //获取结束时间
        System.out.println("程序运行时间： "+(endTime-startTime)+"ms");

        System.out.println("查询时间： "+(toc-tic)+"ms");

        //当前JVM占用的内存总数(M)
        double total = (Runtime.getRuntime().totalMemory()) / (1024.0 * 1024);
        System.out.println("当前JVM占用的内存总数： "+total+"M");

        //JVM最大可用内存总数(M)

        double max = (Runtime.getRuntime().maxMemory()) / (1024.0 * 1024);

        //JVM空闲内存(M)

        double free = (Runtime.getRuntime().freeMemory()) / (1024.0 * 1024);

        //可用内存内存(M)

        double mayuse=(max - total + free);

        //已经使用内存(M)

        double used=(total-free);
        System.out.println("已经使用内存： "+used+"M");
    }

    /**
     * 执行SPARQL语句并输出查询结果
     * @param prefixes：前缀
     * @param tupleIterator: 三元组迭代器
     * @throws JRDFoxException : JRDFox异常
     */
    public static ArrayList<String> evaluateAndPrintResults(Prefixes prefixes, TupleIterator tupleIterator) throws JRDFoxException {
        int numberOfRows = 0;
        ArrayList<String> tuple = new ArrayList<String>();
        int arity = tupleIterator.getArity();
        // We iterate trough the result tuples
        for (long multiplicity = tupleIterator.open(); multiplicity != 0; multiplicity = tupleIterator.advance()) {
            // We iterate trough the terms of each tuple
            StringBuilder builder = new StringBuilder();
            for (int termIndex = 0; termIndex < arity; ++termIndex) {
                if (termIndex != 0) {
//                    System.out.print(" ");
                    builder.append(" ");
                }
                Resource resource = tupleIterator.getResource(termIndex);
                builder.append(resource.toString(prefixes));
                System.out.print(resource.toString(prefixes));
                System.out.print("\n");
            }
//            System.out.println();
//            System.out.println(builder.toString());
            tuple.add(builder.toString());
            ++numberOfRows;
        }
        return tuple;
    }

    /**
     * 打印出所有推理得到的三元组
     * @param oldTuple: 旧有的所有的三元组
     * @param newTuple: 新的所有的三元组
     */
    public static void printReasoningTuple(ArrayList<String> oldTuple, ArrayList<String> newTuple){
        Pattern pattern = Pattern.compile("#(.*?)>");
        //方便打印格式输出
        HashMap<String, String> map = new HashMap<String, String>();
        //去掉原本存在的三元组，保留推理得到的三元组
        boolean removeFlag = newTuple.removeAll(oldTuple);
        if (removeFlag){
            System.out.println("成功计算出推理的三元组！");
            Iterator ite = newTuple.iterator(); //迭代器迭代
            while (ite.hasNext()){
                String iteString = ite.next().toString();
                String[] strings = iteString.split(" ");
                String subject=strings[0], predict=strings[1], object=strings[2];
                ArrayList<String> tempList = convertStrings(pattern, subject, predict, object);
                String commonSubject = tempList.get(0);
                String commonObject = tempList.get(1);
                if (!map.containsKey(commonSubject)) {
                    map.put(commonSubject, commonObject);
                }else{
                    String s = map.get(commonSubject);
                    s = s + "," + commonObject;
                    map.put(commonSubject, s);
                }
                System.out.println(iteString);
            }
//            System.out.println("推理结果如下：");
//            for (String key: map.keySet()){
//                System.out.println(String.format("%s: %s", key, map.get(key)));
//            }
//            System.out.println(String.format("推理出来的三元组总数量为%d", newTuple.size()));
        }else{
            System.out.println("列表清除失败！");
        }
    }

    /**
     * 将nt格式的三元组转换为普通格式
     * @param pattern: 字符串匹配模式
     * @param s1: 传入的三元组主语
     * @param s2: 传入的三元组谓语
     * @param s3: 传入的三元组宾语
     * @return: 主语和宾语组成的普通字符串列表
     */
    public static ArrayList<String> convertStrings(Pattern pattern, String s1, String s2, String s3){
        ArrayList<String> strings = new ArrayList<String>();
        Matcher matcher = pattern.matcher(s1);
        if (matcher.find()){
            String s = matcher.group(1);
            strings.add(s);
        }
        Matcher matcher1 = pattern.matcher(s3);
        if (matcher1.find()){
            String s = matcher1.group(1);
            strings.add(s);
        }
        return strings;
    }
}
