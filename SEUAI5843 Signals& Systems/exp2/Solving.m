function Solving()
f = input('�������ź�Ƶ�ʣ�');
N = input('���������������');
T = input('��������������');
Zero_fill = input('�Ƿ�0����������1����������0����');
if Zero_fill == 0
    myFFT(N,Sampling(f,N,T,Zero_fill));
else
    N = 2*N;
    myFFT(N,Sampling(f,N/2,T,Zero_fill));
end