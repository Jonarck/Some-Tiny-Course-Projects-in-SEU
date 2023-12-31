%% 邻接矩阵的网络可视化函数定义
function matrix2graph(matrix)
    nodeNum = size(matrix,1);
    %节点坐标生成
    angle=0:2*pi/nodeNum:2*pi-2*pi/nodeNum;
    x=100*sin(angle);
    y=100*cos(angle);
    XYCoords = [x;y]';
    %邻接矩阵绘制拓扑图
    gplot(matrix,XYCoords,'-*')
end

