%% NWС����ģ��
function NW(N,K,p)
%N���ڵ㣬ÿ���ڵ������ڽڵ㹲��2K�����ߣ�����ӱߵĸ���Ϊp
    A = NNCN(N,K);
    % ���ɹ�������ͼ��
    subplot(2,1,1)
    matrix2graph(A)
    title('��������');

% ����ӱ�
    kk=(rand(N,N)<p);%��������ӱ߾���
    A = logical(A + kk);%ԭ���ޱߣ���������ӱ߾���ӱ�
    A = A -diag(diag(A));%ȥ���Ի�
    
    % ����NWС��������ͼ��
    subplot(2,1,2)
    matrix2graph(A)
    title(['NWС�������� ','  ����ϵ����',num2str(clusteringCoefficient(A)),'  ƽ��·�����ȣ�',num2str(pathLength(A))]);
end

