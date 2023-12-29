N=33;
absH=[ones(1,9),zeros(1,16),ones(1,8)];
n=[];
for i=1:N
    n(i)=i-1;
end
angH=-2*pi/N*n*(N-1)/2;
H=absH.*exp(i*angH);
b=ifft(H,N);
[H1,w]=freqz(b,1,500);
plot(w/pi,20*log10(abs(H1)));
ylabel('20log|H|');
xlabel('w/pi');
grid;
axis([0 1 -100 10]);
hold on
%增加过渡带进行优化
absH(10)=0.5;
absH(25)=0.5;
n=[];
for i=1:N
    n(i)=i-1;
end
angH=-2*pi/N*n*(N-1)/2;
H=absH.*exp(i*angH);
b=ifft(H,N);
[H1,w]=freqz(b,1,500);
plot(w/pi,20*log10(abs(H1)));
ylabel('20log|H|');
xlabel('w/pi');
grid;
axis([0 1 -100 10]);
hold on
%N=65，加俩点过渡带
N=63;
absH=[ones(1,16),zeros(1,32),ones(1,15)];
absH(17)=0.5886;
absH(18)=0.1065;
absH(47)=0.1065;
absH(48)=0.5886;
n=[];
for i=1:N
    n(i)=i-1;
end
angH=-2*pi/N*n*(N-1)/2;
H=absH.*exp(i*angH);
b=ifft(H,N);
[H1,w]=freqz(b,1,500);
plot(w/pi,20*log10(abs(H1)));
ylabel('20log|H|');
xlabel('w/pi');
grid;
axis([0 1 -100 10]);
legend('N=33优化','N=63优化')