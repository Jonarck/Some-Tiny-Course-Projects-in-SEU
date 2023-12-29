function yhTriangel(n)
yh=1;
for k=1:n
    if k==1
        %一阶杨辉三角
        disp(yh);
        continue;
    end
    %杨辉三角行迭代公式
    yh=[yh,0]+[0,yh];
    %每行依次打印
    disp(yh);
end
