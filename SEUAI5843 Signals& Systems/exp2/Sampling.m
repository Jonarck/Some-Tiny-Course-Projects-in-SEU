function x = Sampling(f,N,T,Zero_fill)
%��������������Ŀ�������������в���
%���룺1.N������������2.T���������3.f���ź�Ƶ��
%1.������������
if Zero_fill == 0 %���貹0
    for n = 1:N
        x(n) = sin(2*pi*f*T*(n-1));
    end
else %�貹0
    for n = 1:N
        x(n) = sin(2*pi*f*T*(n-1));
    end
    for n = N+1:2*N %��0
        x(n) = 0;
    end
end   




