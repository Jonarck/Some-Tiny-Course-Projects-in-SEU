%��ĳ�����׷����������ľ��
function self_mat_cov(x)
%1.��x����õ����������
h = mat_zero_extend(mat_zero_extend(x));
%1.����ת
x = rot90(x,2);
%3.����
for i = 1:length(mat_zero_extend(x))
    for j = 1:length(mat_zero_extend(x))
        y(i,j) =sum(sum(h(i:i+length(x)-1,j:j+length(x)-1).*x));%���ζ�ԭ����ÿ��Ԫ������
    end
end
%��ʾ
disp(y);

