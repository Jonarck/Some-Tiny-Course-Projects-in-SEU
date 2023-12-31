%% 星形网络
function starNetwork = SCN(N,nodeIndex)
    % 生成邻接矩阵
    originNetwork = zeros(N);
    for i = 1 : N
        originNetwork(nodeIndex,i) = 1;
    end
    starNetwork = originNetwork;
    % 计算
    CCscn = num2str(clusteringCoefficient(starNetwork));
    PLscn = num2str(pathLength(starNetwork));
    % 作图
    Tscn = '星形网络网络';
    CC = '聚类系数：';
    PL = ' 平均路径长度：';
    matrix2graph(starNetwork)
    title({Tscn;[CC,CCscn];[PL,PLscn]})
end