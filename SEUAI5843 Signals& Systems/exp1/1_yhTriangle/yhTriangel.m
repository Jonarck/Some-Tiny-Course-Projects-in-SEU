function yhTriangel(n)
yh=1;
for k=1:n
    if k==1
        %һ���������
        disp(yh);
        continue;
    end
    %��������е�����ʽ
    yh=[yh,0]+[0,yh];
    %ÿ�����δ�ӡ
    disp(yh);
end
