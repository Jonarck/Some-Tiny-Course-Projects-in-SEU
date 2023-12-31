%% 聚类系数计算
function C_net_avcc=clusteringCoefficient(A)
    N=length(A);
    D_k=sum(A,2);
    %预定义
    cc=zeros(1,N);

    for i=1:N
       neighbours=setdiff(unique([find(A(i,:)) find(A(:,i))']),i);
       num_neighbours=length(neighbours);
       if num_neighbours > 1
           B=A(neighbours,neighbours);
           num_connect=nnz(B)-nnz(diag(B));
           cc(i)=num_connect/(num_neighbours*(num_neighbours-1)); 
       else
           cc(i)=0;
       end
    end

    C_one=length(find(D_k==1));
    
    C_net_avcc=(C_one+sum(cc))/N;
