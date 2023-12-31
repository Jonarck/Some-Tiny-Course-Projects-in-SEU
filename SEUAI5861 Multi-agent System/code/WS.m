%% WS小世界网络
function WS(N,K,p)
%N个节点，每个节点与相邻节点共有2K条连边，断边重连概率为p
    A = NNCN(N,K);
    % 生成规则网络图像
    subplot(2,1,1)
    matrix2graph(A)
    title('规则网络');
    % 断边重连
    for i=1:N 
        for j=1:K %邻接点遍历（后一半）
            x=i;y=i+K; %邻接点在矩阵中的位置
            if y>N
                y=mod(y,N);
            end
            % 随机断边
            p_r=rand();
            if p_r<=p
                A(x,y)=0;A(y,x)=0;
            % 随机重连
                k=0;
                while k==0
                    y=unidrnd(N);%随机选点
                    if A(x,y)==0 && x~=y %防止自环
                        A(x,y)=1; A(y,x)=1;%重连
                        k=1;
                    end
                end
            end  
   
        end
    end
    % 生成WS小世界网络图像
    subplot(2,1,2)
    matrix2graph(A)
    title(['WS小世界网络 ','  聚类系数：',num2str(clusteringCoefficient(A)),'  平均路径长度：',num2str(pathLength(A))]);
end