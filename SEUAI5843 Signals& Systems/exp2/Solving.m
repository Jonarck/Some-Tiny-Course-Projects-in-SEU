function Solving()
f = input('请输入信号频率：');
N = input('请输入采样点数：');
T = input('请输入采样间隔：');
Zero_fill = input('是否补0（是则输入1，否则输入0）：');
if Zero_fill == 0
    myFFT(N,Sampling(f,N,T,Zero_fill));
else
    N = 2*N;
    myFFT(N,Sampling(f,N/2,T,Zero_fill));
end