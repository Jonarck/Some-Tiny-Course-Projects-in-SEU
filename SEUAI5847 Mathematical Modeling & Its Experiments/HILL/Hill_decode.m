function Hill_decode()
%HILL密码解密函数
%完成者：58119125蒋卓洋
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤1：请求录入密钥矩阵，并对进行基础判断
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.输入矩阵
disp('请输入希尔密码维数：');
n = input('');
disp('请输入加密方阵（按照[a b;c d]格式输入）：');
Key_metrix = input('');
%2.判断矩阵是否可用
%%2.1判断矩阵是否满足维数
if size(Key_metrix,1) ~= size(Key_metrix,2) || size(Key_metrix,1) ~= n
    error('矩阵格式输入有误')
end
%%2.2判断矩阵是否可逆
if det(Key_metrix) == 0%判断矩阵是否可逆
    error('密钥矩阵不可逆,无法构造hill密码');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤2：请求输入密文，并对密文进行处理
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.输入密文，获取密文总长度
Cipher = input('请输入大写英文密文：\n','s');
m = size(Cipher,2);
%2.密文编码
for i = 1:m  
    Cipher(i) = Cipher(i) - 64; %ASCAII码
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤3：解密
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.求密钥矩阵的模26逆
%%1.1求密钥矩阵行列式的模26倒数
%%%(1)行列式求值取模
d = ceil(det(Key_metrix));
if d > 26
    d = ceil(mod(d,26));
end
%%%(2)枚举法求行列式摸26倒数
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
        disp('密钥矩阵不模26可逆（其行列式无模26倒数）');
end
%%1.2求密钥矩阵的伴随矩阵
Key_adjMetrix = inv(Key_metrix) * det(Key_metrix);
%%1.3求密钥矩阵的模26逆矩阵
Key_invMetrix = mod((d_inverse * Key_adjMetrix),26);

%2.分组初步解密
for i = 1:n:size(Cipher,2)-1
    Plain_i = Key_invMetrix * Cipher(i:i+n-1)';
    Plain(i:i+n-1) = Plain_i';
end
%3.模26处理
for i = 1:m
    if Plain(i) > 26
        Plain(i) = mod(Plain(i),26);
    end
    if Plain(i) == 0
        Plain(i) = 26;
    end
    Plain(i) = Plain(i) + 64;
end
%3.明文生成
Plain = char(Plain);
fprintf('\n对密文解密后明文为：\n%s\n',Plain);

