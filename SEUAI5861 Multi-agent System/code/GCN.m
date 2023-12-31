%% 全连接耦合网络
function globallNetwork = GCN(N)
    % 生成邻接矩阵
    originNetwork = zeros(N);
    for i = 1:N
        for j = 1:N
         originNetwork(i,j) = 1;
        end
    end
    globallNetwork = originNetwork;
    % 计算
    CCgcn = num2str(clusteringCoefficient(globallNetwork));
    PLgcn = num2str(pathLength(globallNetwork));
    % 作图
    Tgcn = '全连接耦合网络 ';
    CC = '聚类系数：';
    PL = ' 平均路径长度：';
    matrix2graph(globallNetwork)
    title({Tgcn;[CC,CCgcn];[PL,PLgcn]})
end