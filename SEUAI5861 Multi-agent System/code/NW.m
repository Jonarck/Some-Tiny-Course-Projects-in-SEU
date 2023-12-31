%% NW小世界模型
function NW(N,K,p)
%N个节点，每个节点与相邻节点共有2K条连边，随机加边的概率为p
    A = NNCN(N,K);
    % 生成规则网络图像
    subplot(2,1,1)
    matrix2graph(A)
    title('规则网络');

% 随机加边
    kk=(rand(N,N)<p);%生成随机加边矩阵
    A = logical(A + kk);%原本无边，则用随机加边矩阵加边
    A = A -diag(diag(A));%去除自环
    
    % 生成NW小世界网络图像
    subplot(2,1,2)
    matrix2graph(A)
    title(['NW小世界网络 ','  聚类系数：',num2str(clusteringCoefficient(A)),'  平均路径长度：',num2str(pathLength(A))]);
end

