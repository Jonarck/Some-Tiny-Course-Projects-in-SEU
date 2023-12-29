function X = myFFT(N,x)
%按时间抽取法FFT-基2算法函数
%输入：1.x:时域序列；2.N：序列长度
%1.码位倒置，对输入序列进行重排
 %1.1获取倒置的二进制码位
bi_n_new = fliplr(dec2bin([0:N-1]));
 %1.2将倒置码位转化为十进制从而得到重排输入序列的下标
n_new = bin2dec(bi_n_new);
 %1.3重排输入序列与输出的变换后序列建立对应关系
for i = 1:N
    X(i) = x(n_new(i)+1);%将序列直接存入X作为其初始化，从而建立对应关系
end

%2.蝶形结运算：计算FFT后的序列
 %2.1获取蝶形结运算层数
Level_num = log2(N);
 %2.2进入循环对每层做蝶形结运算
for level = 1:Level_num
    %2.2.1获取每层中进行蝶形结运算的组数（一对奇偶序列为一组）
    Group_num = N/(2^(level));
    %2.2.2获取两组之间对应元素下标间距
    Group_spacing = 2^(level);
    %2.2.3在本组中做单个蝶形结运算
     %2.2.3.1获取组内两计算元素的下标间距
    Factor_spacing = 2^(level-1);
     %2.2.3.2定义Wn
    N_of_W = Group_spacing;%不同层N为组距
    Wn = exp(-i*(2*pi/N_of_W));%Wn定义
     %2.2.3.3进入循环对每组进行计算 
    for group = 1:Group_num 
       %（1）每组中定义下标
       n_1=(group-1)*Group_spacing;%每组中第一个元素下标
       n_2=(group-1)*Group_spacing+Factor_spacing;%每组中第二个元素下标
       %（2）进入循环计算每组的序列 
       for k=1:(2^(level-1))
           X(k+n_1)=X(k+n_1)+Wn^(k-1)*X(k+n_2);%第level层，第group组的前半序列计算
           X(k+n_2)=X(k+n_1)-Wn^(k-1)*X(k+n_2)-Wn^(k-1)*X(k+n_2);%第level层，第group组的后半序列计算
       end
    end
end

%3.记录和画图
disp(X);
k = [0:length(X)-1];%序列下标
M = abs(X)/sum(abs(X));%取模归一化
stem(k,M);%归一化作图














