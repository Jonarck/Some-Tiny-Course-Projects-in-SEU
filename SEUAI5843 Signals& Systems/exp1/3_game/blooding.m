%血量计算函数
function blooding()
x = [5,4,3,2,1];
y = x;
%按照规则，利用自定义的零扩展函数计算
for i = 1:2
    y = [y,0,0] + zero_extend(x,i*2,1);
end
%画图
t = [1:length(y)];%横坐标
stem(t,y);



