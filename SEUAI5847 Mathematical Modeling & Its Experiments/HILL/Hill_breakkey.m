function Hill_breakkey()
%HILL�������뺯��
%����ߣ�58119125��׿��
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����1������¼����֪����Կ��ʽ
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.����¼����֪����Կ��ʽ
disp('������ϣ������ά����');
n = input('');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����2������¼���ȡ��һ�����ĺ����ģ�����
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.���������ȡ���Ĳ�����
%%1.1����
Cipher_piece = input('�����д�Ľػ����ģ�\n','s');
m1 = size(Cipher_piece,2);
if m1 ~= n^2
    error('���볤�Ȳ�������Կ��������');
end
%%1.2����
for i = 1:m1
    if Cipher_piece(i) >= 'A' && Cipher_piece(i) <= 'Z'
        Cipher_piece(i) = Cipher_piece(i) - 64;
    else
        error('�������Ӧ�������дӢ����ĸ');
    end
end
%2.���������ȡ���Ĳ�����
%%2.1����
Plain_piece = input('�����д�Ķ�Ӧ���ģ�\n','s');
m2 = size(Plain_piece,2);
if m2 ~= n^2
    error('���볤�Ȳ�������Կ��������');
end
%%2.2����
for i = 1:m2
    if Plain_piece(i) >= 'A' && Plain_piece(i) <= 'Z'
        Plain_piece(i) = Plain_piece(i) - 64;
    else
        error('�������Ӧ�������дӢ����ĸ');
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����3�������������ľ���
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.�������ľ���
Cipher_metrix = [];
for i = 1:n
    for j = 1:n
        Cipher_metrix(i,j) = Cipher_piece((i-1)*n+j);
    end
end
Cipher_metrix = Cipher_metrix';
%2.�������ľ���
Plain_metrix = [];
for i = 1:n
    for j = 1:n
        Plain_metrix(i,j) = Plain_piece((i-1)*n+j);
    end
end
Plain_metrix = Plain_metrix';
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����4�������ľ����ģ26��
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1�����ľ�������ʽ��ģ26����
%%1.1����ʽ��ֵȡģ
d = round(det(Cipher_metrix));
if d > 26
    d = round(mod(d,26));
end
%%1.2ö�ٷ�������ʽ��26����
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
        error('���ľ���ģ26���棨������ʽ��ģ26������');
end
%2�����ľ���İ������
Cipher_adjMetrix = inv(Cipher_metrix) * det(Cipher_metrix);
%3�����ľ����ģ26�����
Cipher_invMetrix = mod(d_inverse * Cipher_adjMetrix,26);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%����5������Կ����(��ģ26��)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.����Կ�����ģ26��
Key_invMetrix = round(mod((Plain_metrix * Cipher_invMetrix),26));
%2.�����Կ�����ģ26��
disp('��Կ�����ģ26��Ϊ��');
disp(Key_invMetrix);

