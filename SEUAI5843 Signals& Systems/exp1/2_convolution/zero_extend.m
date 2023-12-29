%零扩展函数：实现向量的零扩展
function y = zero_extend(x,n,judge)
y = x;
%右扩展
if judge == 0
    for i = 1:n
        y = [y,0];
    end
end
%左扩展
if judge == 1
    for i = 1:n
        y = [0,y];
    end
end

