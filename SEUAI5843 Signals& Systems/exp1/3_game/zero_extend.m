%����չ������ʵ������������չ
function y = zero_extend(x,n,judge)
y = x;
%����չ
if judge == 0
    for i = 1:n
        y = [y,0];
    end
end
%����չ
if judge == 1
    for i = 1:n
        y = [0,y];
    end
end

