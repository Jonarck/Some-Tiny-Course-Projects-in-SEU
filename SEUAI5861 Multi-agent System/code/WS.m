%% WSС��������
function WS(N,K,p)
%N���ڵ㣬ÿ���ڵ������ڽڵ㹲��2K�����ߣ��ϱ���������Ϊp
    A = NNCN(N,K);
    % ���ɹ�������ͼ��
    subplot(2,1,1)
    matrix2graph(A)
    title('��������');
    % �ϱ�����
    for i=1:N 
        for j=1:K %�ڽӵ��������һ�룩
            x=i;y=i+K; %�ڽӵ��ھ����е�λ��
            if y>N
                y=mod(y,N);
            end
            % ����ϱ�
            p_r=rand();
            if p_r<=p
                A(x,y)=0;A(y,x)=0;
            % �������
                k=0;
                while k==0
                    y=unidrnd(N);%���ѡ��
                    if A(x,y)==0 && x~=y %��ֹ�Ի�
                        A(x,y)=1; A(y,x)=1;%����
                        k=1;
                    end
                end
            end  
   
        end
    end
    % ����WSС��������ͼ��
    subplot(2,1,2)
    matrix2graph(A)
    title(['WSС�������� ','  ����ϵ����',num2str(clusteringCoefficient(A)),'  ƽ��·�����ȣ�',num2str(pathLength(A))]);
end