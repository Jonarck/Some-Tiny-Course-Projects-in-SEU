%% 可视化网络生成 
% 全连接耦合网络
subplot(2,2,1)
GCN15 = GCN(15);
% 最近邻耦合网络
subplot(2,2,2)
NNCN15 = NNCN(15,2);
% 星形网络
subplot(2,2,3)
SCN15 = SCN(15,4);
% 随机网络
subplot(2,2,4)
ERN15 = ERN(15,0.1);
% WS小世界网络网络
figure
WS(15,2,0.2)
% NW小世界网络网络
figure
NW(15,2,0.1)