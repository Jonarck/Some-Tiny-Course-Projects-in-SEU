function Hill_encode()
%HILL密码加密函数
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
%步骤2：请求输入明文，并对明文进行处理
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.输入明文，获取明文总长度
Plain = input('请输入大写英文明文：\n','s');
m = size(Plain,2);

%2.明文编码
for i=1:m
    if Plain(i)>='A' && Plain(i)<='Z'       
        Plain(i)= Plain(i) - 64; %ASCAII码
    else
        error('输入错误，应该输入大写英文字母');
    end
end
%3.根据m与n的关系，进行明文补位，补1
if mod(m,n)~=0
    Append = ones(1,mod(m,n));%补位1向量（行向量）
    Plain = [Plain Append];
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%步骤3：加密
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1.分组初步加密
for i = 1:n:size(Plain,2)-1
    Cipher_i = Key_metrix * Plain(i:i+n-1)';
    Cipher(i:i+n-1) = Cipher_i';
end
%2.模26处理
for i = 1:m
    if Cipher(i) > 26
        Cipher(i) = mod(Cipher(i),26);
    end
    Cipher(i)= Cipher(i) + 64;
end
%3.密文生成
Cipher = char(Cipher);
fprintf('密文为：\n%s',Cipher);
