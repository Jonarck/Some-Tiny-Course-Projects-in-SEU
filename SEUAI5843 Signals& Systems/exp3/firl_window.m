%基于窗函数设计FIR低通滤波器
%wp = 0.2*pi;%性能指标
%ws = 0.4*pi;%过滤带宽度
%w_delta = ws - wp;%过滤带宽度
%N = ceil(3.11*pi/w_delta);%滤波器长度
%Nw = 2*N + 1;%窗口长度
%wc = (wp + ws)/2;%截止频率
%win = hanning(Nw);%汉宁窗的时域响应
%b = firl(Nw-1, wc/pi, win);%firl 是基于加窗的线性相位FIR数字滤波器设计函数
%freqz(b, 1, 512)%为求取频率响应，分子为b，分母为1
clc
wp=0.2*pi;
ws=0.4*pi;
tr_width=ws-wp;
N=ceil(8*pi/tr_width);
n=0:N-1;
wc=(ws+wp)/2;
alpha=(N-1)/2;
m=n-alpha;
hd=(sin(pi*m)-sin(wc*m))./(pi*m);
w_ham=(hamming(N))';
h=hd.*w_ham;
w=0:0.01:pi;
H=freqz(h,1,w);
dbH=20*log10(abs(H)/max(abs(H)));

figure(1)
subplot(221);stem(n,h,'.');
title('实际滤波器的单位响应');
xlabel('n');ylabel('h(n)');
subplot(222);stem(n,w_ham,'.');
title('海明窗');
xlabel('n');ylabel('w(n)');
subplot(223);stem(w/pi,dbH);
title('幅频响应');
xlabel('w/pi');ylabel('dB');
subplot(224);stem(w/pi,angle(H));
title('相频响应');
xlabel('w/pi');ylabel('弧度');