function Hill_decode()
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
Cipher = input('�������дӢ�����ģ�\n','s');
m = size(Cipher,2);
%2.���ı���
for i = 1:m  
    Cipher(i) = Cipher(i) - 64; %ASCAII��
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����3������
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.����Կ�����ģ26��
%%1.1����Կ��������ʽ��ģ26����
%%%(1)����ʽ��ֵȡģ
d = ceil(det(Key_metrix));
if d > 26
    d = ceil(mod(d,26));
end
%%%(2)ö�ٷ�������ʽ��26����
switch d
    case 1
        d_inverse = 1;
    case 3
        d_inverse = 9;
    case 5
        d_inverse = 21;
    case 7
        d_inverse = 15;
    case 9
        d_inverse = 3;
    case 11
        d_inverse = 19;
    case 15
        d_inverse = 7;
    case 17
        d_inverse = 23;
    case 19
        d_inverse = 11;
    case 21
        d_inverse = 5;
    case 23
        d_inverse = 17;
    case 25
        d_inverse = 25;
    otherwise
        disp('��Կ����ģ26���棨������ʽ��ģ26������');
end
%%1.2����Կ����İ������
Key_adjMetrix = inv(Key_metrix) * det(Key_metrix);
%%1.3����Կ�����ģ26�����
Key_invMetrix = mod((d_inverse * Key_adjMetrix),26);

%2.�����������
for i = 1:n:size(Cipher,2)-1
    Plain_i = Key_invMetrix * Cipher(i:i+n-1)';
    Plain(i:i+n-1) = Plain_i';
end
%3.ģ26����
for i = 1:m
    if Plain(i) > 26
        Plain(i) = mod(Plain(i),26);
    end
    if Plain(i) == 0
        Plain(i) = 26;
    end
    Plain(i) = Plain(i) + 64;
end
%3.��������
Plain = char(Plain);
fprintf('\n�����Ľ��ܺ�����Ϊ��\n%s\n',Plain);

