%% 1.图像引入与灰度处理

% 读入
img_origin = imread('HPT.png');
% 灰度处理
img_gray = double(rgb2gray(img_origin));

figure('name','Origin Image and Grizzled Image');
imshowpair(img_origin, img_gray, 'montage');
title('Origin Image and Grizzled Image');

%% 2.高斯一阶导数滤波器（Derivative of Gaussian Filter）

% 定义x,y方向的两个3*3的坐标定位矩阵(注意行列表示)
x_loc = [-1,-1,-1;
         0,0,0;
         1,1,1];
y_loc = [-1,0,1;
         -1,0,1;
         -1,0,1];

% 自定义高斯参数，使得窗宽与sigma尽量适配
sigma = 0.5;
% 将偏导步骤与高斯核构建步骤合并，用公式法计算高斯核每个元素位置（.*;./运算实现）的一阶导数
x_gdt = (-x_loc/(2*pi*(sigma^4))) .* exp(-(x_loc.*x_loc + y_loc.*y_loc)./(2*(sigma^2)));
y_gdt = (-y_loc/(2*pi*(sigma^4))) .* exp(-(x_loc.*x_loc + y_loc.*y_loc)./(2*(sigma^2)));

figure('name','Gx');
imagesc(x_gdt)
title('Gx');
figure('name','Gy');
imagesc(y_gdt)
title('Gy');

%% 3.图像梯度计算

% 用相关对图像进行滤波得到图像的梯度
img_gd_x = imfilter(double(img_gray),x_gdt,'corr');
img_gd_y = imfilter(double(img_gray),y_gdt,'corr');
% 得到各像素点梯度幅值，构建为一个矩阵
img_gd_Strength = sqrt(img_gd_x.^2 + img_gd_y.^2);

figure('name','GS');
imagesc(img_gd_Strength)
title('Gradient Strength');

% 得到各像素点梯度方向角度，构建为一个矩阵（4象限反正切）
img_gd_Direction = atan2(img_gd_y,img_gd_x);
% 将梯度方向归化至上下对角八个个方向，便于之后进行非极大值抑制操作
img_gd_Direction = round(img_gd_Direction ./ (pi/4)) .* (pi/4); %利用取整函数，将梯度方向角度大小变为最接近的pi/4的倍数

%% 4.非极大值抑制算法实现

% 定义非极大值抑制像素存储矩阵
[img_m,img_n] = size(img_gray);
NMS = zeros(img_m,img_n);
% 扫描计算得到NMS结果
for img_x = 2:(img_m - 1)
   for img_y = 2:(img_n - 1)
       % 根据归化后的梯度方向的八个取值，讨论每个像素点处，与该像素点比较的两个像素（“比较点”）的位置
       % 水平方向“比较点”位置分类讨论（比较条件简化呈现）
       if ( img_gd_Direction(img_x,img_y) >= (pi/4) && img_gd_Direction(img_x,img_y) <= (3*pi/4) )
           move_y = 1;
       elseif ( img_gd_Direction(img_x,img_y) >= (-3*pi/4) && img_gd_Direction(img_x,img_y) <= (-pi/4) )
           move_y = -1;
       else
           move_y = 0;
       end
       % 竖直方向“比较点”位置分类讨论（比较条件简化呈现）
       if( abs(img_gd_Direction(img_x,img_y)) <= (pi/4) )
           move_x = 1;
       elseif ( abs(img_gd_Direction(img_x,img_y)) >= (3*pi/4) )
           move_x = -1;
       else
           move_x = 0;
       end
       % 将本像素点与两个“比较点”进行比较
       if (img_gd_Strength(img_x,img_y) >= img_gd_Strength(img_x+move_x,img_y+move_y) && img_gd_Strength(img_x,img_y) >= img_gd_Strength(img_x-move_x,img_y-move_y) )
           NMS(img_x,img_y) = img_gd_Strength(img_x,img_y); % 若比两个比较点都大，则保留
       else
           NMS(img_x,img_y) = 0;
       end
   end    
end

figure('name','NMS');
imagesc(NMS)
title('Non-Maximum Suppression');

%% 5.阈值操作及连接

% 定义双阈值
threshould_high = 5;
threshould_low = 1;
% 构建双阈值选择矩阵（图像）
img_high = double(NMS > threshould_high);
img_low = double(NMS > threshould_low);
% 在高阈值图像上根据低阈值图像进行连接
for x = 2:(img_m - 1)
    for y = 2:(img_n - 1)
        % 若像素点与已经判定为边的高阈值点相邻，则连接
        if (img_high(x,y) == 1)
            % 根据归化后的梯度方向的八个取值，讨论每个像素点的梯度邻接点位置
            % 水平方向
            if ( img_gd_Direction(img_x,img_y) >= (pi/4) && img_gd_Direction(img_x,img_y) <= (3*pi/4) )
                move_y = 1;
            elseif ( img_gd_Direction(img_x,img_y) >= (-3*pi/4) && img_gd_Direction(img_x,img_y) <= (-pi/4) )
                move_y = -1;
            else
                move_y = 0;
            end
            % 竖直方向
            if( abs(img_gd_Direction(img_x,img_y)) <= (pi/4) )
                move_x = 1;
            elseif ( abs(img_gd_Direction(img_x,img_y)) >= (3*pi/4) )
                move_x = -1;
            else
                move_x = 0;
            end
            % 邻接像素点连接
            img_high(x+move_x,y+move_y) = img_low(x+move_x,y+move_y);
        end
    end
end

%% 6.最终生成边缘检测

my_Canny = img_high;

figure('name','GS');
imagesc(my_Canny)
title('Gradient Strength');

%% 7.直接canny边缘检测并对比

standard_Canny = edge(img_gray,'canny');
imagesc(standard_Canny)
figure('name','canny算子比较');
imshowpair(my_Canny,standard_Canny,'montage');
title('My Canny vs. Standard Canny');
