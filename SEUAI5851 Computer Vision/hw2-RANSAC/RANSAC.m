%% 1.Randomly generate the set of data points to be fitted: 

% 1.1.Generate 900 points on a specific plane defined by ourselves.
% 1) Define the parameter of the plane.
    actual_a = -3; actual_b = -2; actual_c = 2; actual_d = 5;
% 2) Randomly define the 2-D coordinate of 900 points within [0,30] in both direction .
    x = 0+(30-0)*rand(1,900);
    y = 0+(30-0)*rand(1,900);
% 3) Generate their function the model parameter according to the definition of the plane.
    z = -(actual_a/actual_c)*x - (actual_b/actual_c)*y + (actual_d/actual_c);
    actual_model = [-(actual_a/actual_c); -(actual_b/actual_c); actual_d/actual_c];
% 4) Show the points
    figure
    p0 = plot3(x,y,z,'.');
    p0.MarkerSize = 12;
    
% 1.2.Generate linear random points (inlier):
% 1) Give all 900 points a white Gaussian noise walue to get inliers matrix
    inliers = [x;y;z] + wgn(3,900,5); %sigma = 5
    % Another definition: inliers = [x;y;z] + [a;b;c]*wgn(1,900,0.1);
% 2) Show the inliers.
    figure
    p1_inlier = plot3(inliers(1,:),inliers(2,:),inliers(3,:),'.','Color','g');
    p1_inlier.MarkerSize = 12;
    hold on
    p1_x = -5:40:35;
    p1_y = p1_x;
    [p1_X,p1_Y] = meshgrid(p1_x);
    p1_Z = -(actual_a/actual_c)*p1_X - (actual_b/actual_c)*p1_Y + (actual_d/actual_c);
    p1_plane = surf(p1_X,p1_Y,p1_Z,'FaceAlpha',0.5);
    p1_plane.FaceColor = '#4DBEEE';
    
% 1.3.Generaye pure random points (outlier):
% 1) Randomly generate 100 points with in [-5,35] regarded as outliers.
    o_x = randi([0,30],1,100);
    o_y = randi([0,30],1,100);
    o_z = randi([3,77],1,100);
    outliers = [o_x;o_y;o_z];
% 2) Show the outliers:
    figure
    p1_outliers = plot3(outliers(1,:),outliers(2,:),outliers(3,:),'.','Color','r');
    p1_outliers.MarkerSize = 12;   
% 3) Show the whole set model.
    figure
    p1_inlier = plot3(inliers(1,:),inliers(2,:),inliers(3,:),'.','Color','g');
    p1_inlier.MarkerSize = 12;

    hold on
    p1_x = -5:1:35;
    p1_y = p1_x;
    [p1_X,p1_Y] = meshgrid(p1_x);
    p1_Z = -(actual_a/actual_c)*p1_X - (actual_b/actual_c)*p1_Y + (actual_d/actual_c);
    p1_plane = surf(p1_X,p1_Y,p1_Z,'FaceAlpha',0.5);
    p1_plane.FaceColor = '#4DBEEE';
    
    hold on
    p1_outliers = plot3(outliers(1,:),outliers(2,:),outliers(3,:),'.','Color','r');
    p1_outliers.MarkerSize = 12;  
% 1.4.Obtain the whole point set:
% 1) Merge the matrix of inliers and outliers.
    points = [inliers outliers];
% 2) Show the point set:
    figure
    p2 = plot3(points(1,:),points(2,:),points(3,:),'.','Color','b');
    p2.MarkerSize = 12;   
    
%% 2.Using RANSAC algorithm to obtain the inlier set:

% 2.1 Define the hyper-parameters of RANSAC(Adaptive procedure):
% 1)Min-model-fit-set-size: s
    % Min-model-fit-set-size: The minimum nuber needed to fit the model.
    s = 3; % According to the model's complexity.
% 2)Not-all-lose-probability: p
    % Not-all-lose-probability: The probability that in N iterative processes, we obtain a Min-model-fit-set whose points is all inliers in at least one iterative processes.
    % 1-p: The probability that in all of processes in N iterative , the point set we obtained has outliers.
    p = 0.99; % According to our own demand.
% 3)Outlier-ratio: e
    % Outlier-ratio: Probability that we get an outlier in one random selection.
    % e = number_of_outliers/number_of_points
    % Here we assume we do not know it previously, and will calculate it in iteration with the number of outliers of models we obtained every time.
% 4)Model-obtain-times(iterative times): N
    % Model-sample-number(iterative times): The times of iterative processes(In one process, we randomly obtaind a model to judge if we accept it), also the times of iteration in the algorithm.
    % So that we have the equation of N, e and p which can be used to calculate N in iteration: (1-(1-e)^s)^N = 1-p.
    N = Inf; % We initial it with Inf and it will be renewed adaptively .
% 5)Inlier-threshold: t:
    % Inlier-threshold: The furthest distance weregard a point as an inlier.
    t = 5; % According to our own demand.(According to the gaussian noise strength)
% 6)Consensus-set-size-threshold: d
    % Consensus-set-size-threshold: The least num of inliers for the judement wether we accept the model we randomly obtain.
    d = 900; % According to our own demand.
    
%2.2 Iteration of the model selection:
% 1）Initial the structure we would use to judge in iteration.
% A.Number of randomly selected models:
    sample_count = 0;
% B.Absolute error of the best model:
    best_error = Inf;
    
    while(N > sample_count)
% 2) Randomly choose 3 points to fit a model
% A.Choose points:
        maybe_min_set = points(:,randi(1000,1,3)); 
% B.Fit model:
        v_p1_p2 = maybe_min_set(:,2) - maybe_min_set(:,1);
        v_p1_p3 = maybe_min_set(:,3) - maybe_min_set(:,1);
        v_n = cross(v_p1_p2, v_p1_p3);
        m_a = v_n(1);
        m_b = v_n(2);
        m_c = v_n(3);
        m_d = -m_a*maybe_min_set(1,1) - m_b*maybe_min_set(2,1) - m_c*maybe_min_set(3,1);
        maybe_model = [m_a, m_b, m_c, m_d];
% 3) Calculate the distance to judge the inliers
        distance_matrix = ( abs(maybe_model*[points;ones(1,1000)])/ sqrt(m_a*m_a + m_b*m_b +m_c*m_c) );
% 4) Count inlier number of the maybe model with the judgement of distance and Inlier-threshold.
        outlier_num = sum(distance_matrix > t);
        inlier_num = 1000 - outlier_num;           
% 5) Judge whether we accept the model with inlier-number and Consensus-set-size-threshold.
            if(inlier_num >= d)% it is a model we can accept
% 6) Constract the consensus set:
                consensus_set = repmat(((distance_matrix <= t)),3,1).*points;
                consensus_set(:,all(consensus_set==0,1)) = [];
% 7) To judge if it is a better model with a smaller error
% A. Calculate the absolute error with distance of inliers
                consensus_distance_matrix = ( abs(maybe_model*[consensus_set;ones(1,inlier_num)])/ sqrt(m_a*m_a + m_b*m_b +m_c*m_c) );
                better_error = sum(abs(consensus_distance_matrix));
% C. Judge if it is the best model by now with error-judement-conditions.
                if(better_error < best_error)
% 8) Obtain the result of this iteration: the items about the best model by now.
% A.Its consensus set and inlier number which might be the output
                    best_consensus_set = consensus_set;
                    best_inlier_num = inlier_num;
% B.Its renewed best error which will be used in next iteration to do judgement.                        
                    best_error = better_error;     
% 9) Define Outlier-ratio to renew N when N is larger than the iterations
                    e = 1 - best_inlier_num/1000;
                    N = ceil(log(1-p)/log(1-(1-e)^s));
                    disp('N is:');
                    disp(N);                    
                end 
            end
%10) Increment sample_count by 1
            sample_count = sample_count+1;
            disp('Sample obatining time is:');
            disp(sample_count);
    end
    
  
%% 3.Fit the plane-model on the inlier set with the way of least square:

% 1) Use least square method to Fit the best-model with the best consensus set we obtaind through RANSAC.
    best_C = [best_consensus_set(1:2,:); ones(1,best_inlier_num)]';
    best_D = best_consensus_set(3,:)';
    best_model = best_C\best_D;
% 2) Calculate the absolute error and display it.
    para_absolute_error = abs(best_model - actual_model);
    fprintf('The absolute error of model parameter is\n');
    disp(para_absolute_error);
% 3) Result visualization
% A.Show the actual plane we want to fit.
    figure
    p4_x = -5:1:35;
    p4_y = p4_x;
    [p4_X,p4_Y] = meshgrid(p4_x);
    p4_Z = actual_model(1)*p4_X + actual_model(2)*p4_Y + actual_model(3);
    p4_plane = surf(p4_X,p4_Y,p4_Z,'FaceAlpha',0.5);
    p4_plane.FaceColor = '#4DBEEE';
% B.Show the best consensus points we used to fit the best model.
    hold on
    p3 = plot3(best_consensus_set(1,:),best_consensus_set(2,:),best_consensus_set(3,:),'.','Color','g');
    p3.MarkerSize = 12; 
% C.Show the best plane we fitted through RANSAC.   
    hold on
    p6_x = -5:1:35;
    p6_y = p6_x;
    [p6_X,p6_Y] = meshgrid(p6_x);
    p6_Z = best_model(1)*p6_X + best_model(2)*p6_Y + best_model(3);
    p6_plane = surf(p6_X,p6_Y,p6_Z,'FaceAlpha',0.5);
    p6_plane.FaceColor = 'y';    


