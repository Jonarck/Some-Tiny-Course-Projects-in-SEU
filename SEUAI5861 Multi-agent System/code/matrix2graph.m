%% �ڽӾ����������ӻ���������
function matrix2graph(matrix)
    nodeNum = size(matrix,1);
    %�ڵ���������
    angle=0:2*pi/nodeNum:2*pi-2*pi/nodeNum;
    x=100*sin(angle);
    y=100*cos(angle);
    XYCoords = [x;y]';
    %�ڽӾ����������ͼ
    gplot(matrix,XYCoords,'-*')
end

