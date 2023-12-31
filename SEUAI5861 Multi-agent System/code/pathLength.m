%% 平均路径长度计算
function avpath = pathLength(A)
    %初始化
    N=length(A);
    B=sparse([],[],[],N,N,N^2);% 创建一个由 N 个非零值构成的稀疏矩阵，但实际上为此矩阵分配可以存储 N^2 个非零值的空间。
    %预定义
    L=0;
    i=1;
    B=B+A^i;
    M(i)=nnz(B)-nnz(diag(B)); %忽略自环，返回矩阵非零位置个数，即边总数
    %求取所有最短路径
    while M(i)>L 
       L=M(i);
       i=i+1;
       B=B+A^i;
       M(i)=nnz(B)-nnz(diag(B)); %忽略自环
    end
    %最长的最短路径数量
    ls=i-1; 
    %代入公式，求最短路径和并除以可能的路径总数
    avpath= sum((1:ls).*[M(1) M(2:ls)-M(1:ls-1)]) / (N*(N-1)); 
