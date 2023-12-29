%求某奇数阶方阵进行自身的卷积
function self_mat_cov(x)
%1.将x扩充得到被卷积矩阵
h = mat_zero_extend(mat_zero_extend(x));
%1.矩阵翻转
x = rot90(x,2);
%3.求卷积
for i = 1:length(mat_zero_extend(x))
    for j = 1:length(mat_zero_extend(x))
        y(i,j) =sum(sum(h(i:i+length(x)-1,j:j+length(x)-1).*x));%依次对原矩阵每个元素求卷积
    end
end
%显示
disp(y);

