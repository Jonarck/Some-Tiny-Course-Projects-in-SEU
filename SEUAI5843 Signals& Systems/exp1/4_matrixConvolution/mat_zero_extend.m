function y = mat_zero_extend(x)
y(1,:) = zeros(1,length(x)+2);%��������
y(length(x)+2,:) = zeros(1,length(x)+2);%β������
for i = 1:length(x)
    y(i+1,:) = [0,x(i,:),0];%ԭ�����Ӧ������
end


