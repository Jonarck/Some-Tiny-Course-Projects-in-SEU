%Ѫ�����㺯��
function blooding()
x = [5,4,3,2,1];
y = x;
%���չ��������Զ��������չ��������
for i = 1:2
    y = [y,0,0] + zero_extend(x,i*2,1);
end
%��ͼ
t = [1:length(y)];%������
stem(t,y);



