function y = mat_zero_extend(x)
y(1,:) = zeros(1,length(x)+2);%首行扩充
y(length(x)+2,:) = zeros(1,length(x)+2);%尾行扩充
for i = 1:length(x)
    y(i+1,:) = [0,x(i,:),0];%原矩阵对应行扩充
end


