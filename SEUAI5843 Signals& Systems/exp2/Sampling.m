function x = Sampling(f,N,T,Zero_fill)
%采样函数：对题目中所给函数进行采样
%输入：1.N：采样点数；2.T采样间隔；3.f：信号频率
%1.采样生成序列
if Zero_fill == 0 %不需补0
    for n = 1:N
        x(n) = sin(2*pi*f*T*(n-1));
    end
else %需补0
    for n = 1:N
        x(n) = sin(2*pi*f*T*(n-1));
    end
    for n = N+1:2*N %补0
        x(n) = 0;
    end
end   




