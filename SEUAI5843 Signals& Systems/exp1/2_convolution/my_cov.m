%序列卷积计算函数
function y = my_cov(x,h)
%定义循环次数记录变量
num_p = 1;
%进入循环，计算两个信号序列的不进位乘法
for i = flip(h)
    if num_p == 1
        y = i * x;
        num_p = num_p+1;
        continue;
    end
    %迭代相加，并用自定义的扩位函数向右扩位
    y = [0,y] + zero_extend(i*x,num_p-1,0);
    num_p = num_p+1;
end
%显示结果
disp(y);
%绘制图像
stem(y);