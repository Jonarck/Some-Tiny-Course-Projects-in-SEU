function Hill_encode()
%HILL������ܺ���
%����ߣ�58119125��׿��
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����1������¼����Կ���󣬲��Խ��л����ж�
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.�������
disp('������ϣ������ά����');
n = input('');
disp('��������ܷ��󣨰���[a b;c d]��ʽ���룩��');
Key_metrix = input('');
%2.�жϾ����Ƿ����
%%2.1�жϾ����Ƿ�����ά��
if size(Key_metrix,1) ~= size(Key_metrix,2) || size(Key_metrix,1) ~= n
    error('�����ʽ��������')
end
%%2.2�жϾ����Ƿ����
if det(Key_metrix) == 0%�жϾ����Ƿ����
    error('��Կ���󲻿���,�޷�����hill����');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����2�������������ģ��������Ľ��д���
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.�������ģ���ȡ�����ܳ���
Plain = input('�������дӢ�����ģ�\n','s');
m = size(Plain,2);

%2.���ı���
for i=1:m
    if Plain(i)>='A' && Plain(i)<='Z'       
        Plain(i)= Plain(i) - 64; %ASCAII��
    else
        error('�������Ӧ�������дӢ����ĸ');
    end
end
%3.����m��n�Ĺ�ϵ���������Ĳ�λ����1
if mod(m,n)~=0
    Append = ones(1,mod(m,n));%��λ1��������������
    Plain = [Plain Append];
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����3������
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.�����������
for i = 1:n:size(Plain,2)-1
    Cipher_i = Key_metrix * Plain(i:i+n-1)';
    Cipher(i:i+n-1) = Cipher_i';
end
%2.ģ26����
for i = 1:m
    if Cipher(i) > 26
        Cipher(i) = mod(Cipher(i),26);
    end
    Cipher(i)= Cipher(i) + 64;
end
%3.��������
Cipher = char(Cipher);
fprintf('����Ϊ��\n%s',Cipher);
