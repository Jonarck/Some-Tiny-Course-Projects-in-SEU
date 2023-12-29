function Hill_breakkey()
%HILL密码破译函数
%完成者：58119125蒋卓洋
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤1：请求录入已知的密钥形式
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.请求录入已知的密钥形式
disp('请输入希尔密码维数：');
n = input('');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤2：请求录入获取的一段密文和明文，编码
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.请求输入截取密文并编码
%%1.1输入
Cipher_piece = input('输入大写的截获密文：\n','s');
m1 = size(Cipher_piece,2);
if m1 ~= n^2
    error('输入长度不满足密钥破译需求');
end
%%1.2编码
for i = 1:m1
    if Cipher_piece(i) >= 'A' && Cipher_piece(i) <= 'Z'
        Cipher_piece(i) = Cipher_piece(i) - 64;
    else
        error('输入错误，应该输入大写英文字母');
    end
end
%2.请求输入截取明文并编码
%%2.1输入
Plain_piece = input('输入大写的对应明文：\n','s');
m2 = size(Plain_piece,2);
if m2 ~= n^2
    error('输入长度不满足密钥破译需求');
end
%%2.2编码
for i = 1:m2
    if Plain_piece(i) >= 'A' && Plain_piece(i) <= 'Z'
        Plain_piece(i) = Plain_piece(i) - 64;
    else
        error('输入错误，应该输入大写英文字母');
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤3：构建明文密文矩阵
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.构建密文矩阵
Cipher_metrix = [];
for i = 1:n
    for j = 1:n
        Cipher_metrix(i,j) = Cipher_piece((i-1)*n+j);
    end
end
Cipher_metrix = Cipher_metrix';
%2.构建明文矩阵
Plain_metrix = [];
for i = 1:n
    for j = 1:n
        Plain_metrix(i,j) = Plain_piece((i-1)*n+j);
    end
end
Plain_metrix = Plain_metrix';
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤4：求密文矩阵的模26逆
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1求密文矩阵行列式的模26倒数
%%1.1行列式求值取模
d = round(det(Cipher_metrix));
if d > 26
    d = round(mod(d,26));
end
%%1.2枚举法求行列式摸26倒数
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
        error('密文矩阵不模26可逆（其行列式无模26倒数）');
end
%2求密文矩阵的伴随矩阵
Cipher_adjMetrix = inv(Cipher_metrix) * det(Cipher_metrix);
%3求明文矩阵的模26逆矩阵
Cipher_invMetrix = mod(d_inverse * Cipher_adjMetrix,26);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤5：求密钥矩阵(的模26逆)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.求密钥矩阵的模26逆
Key_invMetrix = round(mod((Plain_metrix * Cipher_invMetrix),26));
%2.输出密钥矩阵的模26逆
disp('密钥矩阵的模26逆为：');
disp(Key_invMetrix);

