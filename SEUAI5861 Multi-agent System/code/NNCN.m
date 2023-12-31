%% 最近邻耦合网络
function nearistNeighborNetwork = NNCN(N,K)
    % 生成邻接矩阵
    originNetwork = sparse(N, N);
    for i = 1 : K
        originNetwork = originNetwork + diag(ones(1,N-i),i);
        originNetwork = originNetwork + diag(ones(1, i), N-i);
    end
    originNetwork = originNetwork + originNetwork';
    nearistNeighborNetwork = originNetwork;
    % 计算
    CCnncn = num2str(clusteringCoefficient(nearistNeighborNetwork));
    PLnncn = num2str(pathLength(nearistNeighborNetwork));
    % 作图
    Tnncn = '最近邻耦合网络';
    CC = '聚类系数：';
    PL = ' 平均路径长度：';
    matrix2graph(nearistNeighborNetwork)
    title({Tnncn;[CC,CCnncn];[PL,PLnncn]})
end