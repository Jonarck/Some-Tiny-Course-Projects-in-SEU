%���о�����㺯��
function y = my_cov(x,h)
%����ѭ��������¼����
num_p = 1;
%����ѭ�������������ź����еĲ���λ�˷�
for i = flip(h)
    if num_p == 1
        y = i * x;
        num_p = num_p+1;
        continue;
    end
    %������ӣ������Զ������λ����������λ
    y = [0,y] + zero_extend(i*x,num_p-1,0);
    num_p = num_p+1;
end
%��ʾ���
disp(y);
%����ͼ��
stem(y);