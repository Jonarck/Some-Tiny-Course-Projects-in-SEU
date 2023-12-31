%% 随机网络
function randomNetwork = ERN(N,p)
    % 生成邻接矩阵
    originNetwork = zeros(N);
    coords = zeros(N,2);
    for i = 1:N
        for j = 1:N
            if(rand(1,1)<p)
               originNetwork(i,j) = 1;
               originNetwork(j,i) = 1;
            end
        end
    end
    randomNetwork = originNetwork;
    % 计算
    CCern = num2str(clusteringCoefficient(randomNetwork));
    PLern = num2str(pathLength(randomNetwork));
    % 作图
    Tern = '随机网络';
    CC = '聚类系数：';
    PL = ' 平均路径长度：';    
    matrix2graph(randomNetwork)
    title({Tern;[CC,CCern];[PL,PLern]})
end